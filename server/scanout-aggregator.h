/* SPDX-License-Identifier: LGPL-2.1-or-later */
#ifndef SCANOUT_AGGREGATOR_H_
#define SCANOUT_AGGREGATOR_H_

#include <memory>
#include <mutex>

#include "spice-scanout.h"

struct ScanoutAggregator {
    ScanoutAggregator(uint32_t head, bool enabled);
    ~ScanoutAggregator();

    int submit(const SpiceScanoutFrame *frame);
    void reset();

private:
    uint32_t head;
    bool enabled;
    uint64_t last_frame_id = 0;
    uint32_t generation = 0;
    std::mutex mutex;
    std::unique_ptr<SpiceScanoutFrame> waiting;
};

#endif
