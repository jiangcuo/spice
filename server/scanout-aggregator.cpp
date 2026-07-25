/* SPDX-License-Identifier: LGPL-2.1-or-later */
#include "config.h"
#include "scanout-aggregator.h"

#include <algorithm>
#include <cstring>

#include "utils.h"
#include "reds.h"

static void release_frame(const SpiceScanoutFrame &frame)
{
    if (frame.release) {
        frame.release(frame.release_opaque);
    }
}

ScanoutAggregator::ScanoutAggregator(uint32_t head, bool enabled):
    head(head), enabled(enabled)
{
}

ScanoutAggregator::~ScanoutAggregator()
{
    reset();
}

int ScanoutAggregator::submit(const SpiceScanoutFrame *frame)
{
    if (!enabled) {
        return -1;
    }
    if (!frame || frame->head != head || !frame->data ||
        frame->width == 0 || frame->height == 0 ||
        frame->stride < frame->width * 4 ||
        frame->data_size < static_cast<size_t>(frame->stride) * frame->height) {
        return -1;
    }

    std::lock_guard<std::mutex> guard(mutex);
    if (frame->generation < generation || frame->frame_id <= last_frame_id) {
        release_frame(*frame);
        return -1;
    }
    if (frame->generation != generation) {
        if (waiting) {
            release_frame(*waiting);
            waiting.reset();
        }
        generation = frame->generation;
    }

    auto next = std::make_unique<SpiceScanoutFrame>(*frame);
    if (waiting) {
        release_frame(*waiting);
    }
    waiting = std::move(next);
    last_frame_id = frame->frame_id;
    return 0;
}

void ScanoutAggregator::reset()
{
    std::lock_guard<std::mutex> guard(mutex);
    if (waiting) {
        release_frame(*waiting);
        waiting.reset();
    }
    generation++;
    last_frame_id = 0;
}
