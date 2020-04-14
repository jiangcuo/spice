/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2009-2015 Red Hat, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#include <config.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "red-common.h"
#include "dispatcher.h"
#include "main-dispatcher.h"
#include "red-client.h"
#include "reds.h"

/*
 * Main Dispatcher
 * ===============
 *
 * Communication channel between any non main thread and the main thread.
 *
 * The main thread is that from which spice_server_init is called.
 *
 * Messages are single sized, sent from the non-main thread to the main-thread.
 * No acknowledge is sent back. This prevents a possible deadlock with the main
 * thread already waiting on a response for the existing red_dispatcher used
 * by the worker thread.
 *
 * All events have three functions:
 * main_dispatcher_<event_name> - non static, public function
 * main_dispatcher_self_<event_name> - handler for main thread
 * main_dispatcher_handle_<event_name> - handler for callback from main thread
 *   seperate from self because it may send an ack or do other work in the future.
 */
struct MainDispatcherPrivate
{
    RedsState *reds; /* weak */
    SpiceWatch *watch;
};

G_DEFINE_TYPE_WITH_PRIVATE(MainDispatcher, main_dispatcher, TYPE_DISPATCHER)

enum {
    PROP0,
    PROP_SPICE_SERVER,
};

static void
main_dispatcher_get_property(GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    MainDispatcher *self = MAIN_DISPATCHER(object);

    switch (property_id) {
        case PROP_SPICE_SERVER:
             g_value_set_pointer(value, self->priv->reds);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
main_dispatcher_set_property(GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    MainDispatcher *self = MAIN_DISPATCHER(object);

    switch (property_id) {
        case PROP_SPICE_SERVER:
            self->priv->reds = g_value_get_pointer(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void main_dispatcher_constructed(GObject *object);
static void main_dispatcher_finalize(GObject *object);

static void
main_dispatcher_class_init(MainDispatcherClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->constructed = main_dispatcher_constructed;
    object_class->finalize = main_dispatcher_finalize;
    object_class->get_property = main_dispatcher_get_property;
    object_class->set_property = main_dispatcher_set_property;

    g_object_class_install_property(object_class,
                                    PROP_SPICE_SERVER,
                                    g_param_spec_pointer("spice-server",
                                                         "spice-server",
                                                         "The spice server associated with this dispatcher",
                                                         G_PARAM_READWRITE |
                                                         G_PARAM_CONSTRUCT_ONLY));
}

static void
main_dispatcher_init(MainDispatcher *self)
{
    self->priv = main_dispatcher_get_instance_private(self);
}

enum {
    MAIN_DISPATCHER_CHANNEL_EVENT = 0,
    MAIN_DISPATCHER_MIGRATE_SEAMLESS_DST_COMPLETE,
    MAIN_DISPATCHER_SET_MM_TIME_LATENCY,
    MAIN_DISPATCHER_CLIENT_DISCONNECT,

    MAIN_DISPATCHER_NUM_MESSAGES
};

typedef struct MainDispatcherChannelEventMessage {
    int event;
    SpiceChannelEventInfo *info;
} MainDispatcherChannelEventMessage;

typedef struct MainDispatcherMigrateSeamlessDstCompleteMessage {
    RedClient *client;
} MainDispatcherMigrateSeamlessDstCompleteMessage;

typedef struct MainDispatcherMmTimeLatencyMessage {
    RedClient *client;
    uint32_t latency;
} MainDispatcherMmTimeLatencyMessage;

typedef struct MainDispatcherClientDisconnectMessage {
    RedClient *client;
} MainDispatcherClientDisconnectMessage;

/* channel_event - calls core->channel_event, must be done in main thread */
static void main_dispatcher_handle_channel_event(void *opaque,
                                                 void *payload)
{
    RedsState *reds = opaque;
    MainDispatcherChannelEventMessage *channel_event = payload;

    reds_handle_channel_event(reds, channel_event->event, channel_event->info);
}

void main_dispatcher_channel_event(MainDispatcher *self, int event, SpiceChannelEventInfo *info)
{
    MainDispatcherChannelEventMessage msg = {0,};

    if (pthread_self() == dispatcher_get_thread_id(DISPATCHER(self))) {
        reds_handle_channel_event(self->priv->reds, event, info);
        return;
    }
    msg.event = event;
    msg.info = info;
    dispatcher_send_message(DISPATCHER(self), MAIN_DISPATCHER_CHANNEL_EVENT,
                            &msg);
}


static void main_dispatcher_handle_migrate_complete(void *opaque,
                                                    void *payload)
{
    RedsState *reds = opaque;
    MainDispatcherMigrateSeamlessDstCompleteMessage *mig_complete = payload;

    reds_on_client_seamless_migrate_complete(reds, mig_complete->client);
    g_object_unref(mig_complete->client);
}

static void main_dispatcher_handle_mm_time_latency(void *opaque,
                                                   void *payload)
{
    RedsState *reds = opaque;
    MainDispatcherMmTimeLatencyMessage *msg = payload;
    reds_set_client_mm_time_latency(reds, msg->client, msg->latency);
    g_object_unref(msg->client);
}

static void main_dispatcher_handle_client_disconnect(void *opaque,
                                                     void *payload)
{
    RedsState *reds = opaque;
    MainDispatcherClientDisconnectMessage *msg = payload;

    spice_debug("client=%p", msg->client);
    reds_client_disconnect(reds, msg->client);
    g_object_unref(msg->client);
}

void main_dispatcher_seamless_migrate_dst_complete(MainDispatcher *self,
                                                   RedClient *client)
{
    MainDispatcherMigrateSeamlessDstCompleteMessage msg;

    if (pthread_self() == dispatcher_get_thread_id(DISPATCHER(self))) {
        reds_on_client_seamless_migrate_complete(self->priv->reds, client);
        return;
    }

    msg.client = g_object_ref(client);
    dispatcher_send_message(DISPATCHER(self), MAIN_DISPATCHER_MIGRATE_SEAMLESS_DST_COMPLETE,
                            &msg);
}

void main_dispatcher_set_mm_time_latency(MainDispatcher *self, RedClient *client, uint32_t latency)
{
    MainDispatcherMmTimeLatencyMessage msg;

    if (pthread_self() == dispatcher_get_thread_id(DISPATCHER(self))) {
        reds_set_client_mm_time_latency(self->priv->reds, client, latency);
        return;
    }

    msg.client = g_object_ref(client);
    msg.latency = latency;
    dispatcher_send_message(DISPATCHER(self), MAIN_DISPATCHER_SET_MM_TIME_LATENCY,
                            &msg);
}

void main_dispatcher_client_disconnect(MainDispatcher *self, RedClient *client)
{
    MainDispatcherClientDisconnectMessage msg;

    if (!red_client_is_disconnecting(client)) {
        spice_debug("client %p", client);
        msg.client = g_object_ref(client);
        dispatcher_send_message(DISPATCHER(self), MAIN_DISPATCHER_CLIENT_DISCONNECT,
                                &msg);
    } else {
        spice_debug("client %p already during disconnection", client);
    }
}

/*
 * FIXME:
 * Reds routines shouldn't be exposed. Instead reds.c should register the callbacks,
 * and the corresponding operations should be made only via main_dispatcher.
 */
MainDispatcher* main_dispatcher_new(RedsState *reds)
{
    MainDispatcher *self = g_object_new(TYPE_MAIN_DISPATCHER,
                                        "spice-server", reds,
                                        "max-message-type", MAIN_DISPATCHER_NUM_MESSAGES,
                                        NULL);
    return self;
}

void main_dispatcher_constructed(GObject *object)
{
    MainDispatcher *self = MAIN_DISPATCHER(object);

    G_OBJECT_CLASS(main_dispatcher_parent_class)->constructed(object);
    dispatcher_set_opaque(DISPATCHER(self), self->priv->reds);

    self->priv->watch =
        dispatcher_create_watch(DISPATCHER(self), reds_get_core_interface(self->priv->reds));
    dispatcher_register_handler(DISPATCHER(self), MAIN_DISPATCHER_CHANNEL_EVENT,
                                main_dispatcher_handle_channel_event,
                                sizeof(MainDispatcherChannelEventMessage), false);
    dispatcher_register_handler(DISPATCHER(self), MAIN_DISPATCHER_MIGRATE_SEAMLESS_DST_COMPLETE,
                                main_dispatcher_handle_migrate_complete,
                                sizeof(MainDispatcherMigrateSeamlessDstCompleteMessage), false);
    dispatcher_register_handler(DISPATCHER(self), MAIN_DISPATCHER_SET_MM_TIME_LATENCY,
                                main_dispatcher_handle_mm_time_latency,
                                sizeof(MainDispatcherMmTimeLatencyMessage), false);
    dispatcher_register_handler(DISPATCHER(self), MAIN_DISPATCHER_CLIENT_DISCONNECT,
                                main_dispatcher_handle_client_disconnect,
                                sizeof(MainDispatcherClientDisconnectMessage), false);
}

static void main_dispatcher_finalize(GObject *object)
{
    MainDispatcher *self = MAIN_DISPATCHER(object);

    red_watch_remove(self->priv->watch);
    self->priv->watch = NULL;
    G_OBJECT_CLASS(main_dispatcher_parent_class)->finalize(object);
}
