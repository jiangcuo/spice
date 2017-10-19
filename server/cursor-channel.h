/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2009 Red Hat, Inc.

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

#ifndef CURSOR_CHANNEL_H_
#define CURSOR_CHANNEL_H_

#include "common-graphics-channel.h"
#include "red-parse-qxl.h"

G_BEGIN_DECLS

/**
 * This type it's a RedChannel class which implement cursor (mouse)
 * movements.
 * A pointer to CursorChannel can be converted to a RedChannel.
 */
typedef struct CursorChannel CursorChannel;
typedef struct CursorChannelClass CursorChannelClass;

#define TYPE_CURSOR_CHANNEL cursor_channel_get_type()

#define CURSOR_CHANNEL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_CURSOR_CHANNEL, CursorChannel))
#define CURSOR_CHANNEL_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_CURSOR_CHANNEL, CursorChannelClass))
#define IS_CURSOR_CHANNEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_CURSOR_CHANNEL))
#define IS_CURSOR_CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_CURSOR_CHANNEL))
#define CURSOR_CHANNEL_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_CURSOR_CHANNEL, CursorChannelClass))

GType cursor_channel_get_type(void) G_GNUC_CONST;

/**
 * Create CursorChannel.
 * Since CursorChannel is intended to be run in a separate thread,
 * it does not register its own client callbacks since they would
 * be called from a different thread. Therefore users of this
 * class are responsible for registering their own client callbacks
 * for CursorChannel. These 'wrapper' client callbacks must forward
 * execution on to the CursorChannel thread.
 * cursor_channel_client_migrate() and cursor_channel_connect() are
 * provided as helper functions and should only be called from the
 * CursorChannel thread.
 */
CursorChannel* cursor_channel_new(RedsState *server, int id,
                                  const SpiceCoreInterfaceInternal *core);

void                 cursor_channel_reset       (CursorChannel *cursor);
void                 cursor_channel_do_init     (CursorChannel *cursor);
void                 cursor_channel_process_cmd (CursorChannel *cursor, RedCursorCmd *cursor_cmd);
void                 cursor_channel_set_mouse_mode(CursorChannel *cursor, uint32_t mode);

/**
 * Connect a new client to CursorChannel.
 * This is the equivalent of RedChannel client connect callback.
 * See comment on cursor_channel_new.
 */
void                 cursor_channel_connect     (CursorChannel *cursor, RedClient *client,
                                                 RedsStream *stream,
                                                 int migrate,
                                                 RedChannelCapabilities *caps);

/**
 * Migrate a client channel from a CursorChannel.
 * This is the equivalent of RedChannel client migrate callback.
 * See comment on cursor_channel_new.
 */
void                 cursor_channel_client_migrate(RedChannelClient *client);

G_END_DECLS

#endif /* CURSOR_CHANNEL_H_ */
