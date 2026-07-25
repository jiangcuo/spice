/* SPDX-License-Identifier: LGPL-2.1-or-later */
#ifndef SPICE_SCANOUT_H_
#define SPICE_SCANOUT_H_

#if !defined(SPICE_H_INSIDE) && !defined(SPICE_SERVER_INTERNAL)
#error "Only spice.h can be included directly."
#endif

#include "spice-core.h"
#include <stddef.h>

SPICE_BEGIN_DECLS

#define SPICE_INTERFACE_SCANOUT "scanout"
#define SPICE_INTERFACE_SCANOUT_MAJOR 1
#define SPICE_INTERFACE_SCANOUT_MINOR 0

typedef struct SpiceScanoutInstance SpiceScanoutInstance;

typedef enum {
    SPICE_SCANOUT_FORMAT_BGRX = 1,
} SpiceScanoutFormat;

typedef struct SpiceScanoutFrame {
    uint32_t head;
    uint64_t frame_id;
    uint64_t timestamp_ns;
    uint32_t generation;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    SpiceScanoutFormat format;
    const uint8_t *data;
    size_t data_size;
    uint32_t damage_left;
    uint32_t damage_top;
    uint32_t damage_right;
    uint32_t damage_bottom;
    void (*release)(void *opaque);
    void *release_opaque;
} SpiceScanoutFrame;

struct SpiceScanoutInstance {
    SpiceBaseInstance base;
    uint32_t head;
    void *opaque;
};

int spice_scanout_submit_frame(SpiceScanoutInstance *instance,
                               const SpiceScanoutFrame *frame);
void spice_scanout_reset(SpiceScanoutInstance *instance);

SPICE_END_DECLS

#endif
