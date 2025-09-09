/*
   Copyright (C) 2023 Intel Corporation.

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
#include "udev.h"

#ifdef HAVE_UDEV
#include <libudev.h>
#include <stdbool.h>
#include <stdlib.h>

GpuVendor spice_udev_detect_gpu(int gpu_vendor)
{
    struct udev *udev;
    struct udev_device *drm_dev, *pci_dev;
    struct udev_enumerate *udev_enum;
    struct udev_list_entry *entry, *devices;
    const char *path, *vendor_id;
    GpuVendor vendor = VENDOR_GPU_NOTDETECTED;

    udev = udev_new();
    if (!udev) {
        return VENDOR_GPU_UNKNOWN;
    }

    udev_enum = udev_enumerate_new(udev);
    if (udev_enum) {
        udev_enumerate_add_match_subsystem(udev_enum, "drm");
        udev_enumerate_add_match_sysname(udev_enum, "card[0-9]");
        udev_enumerate_scan_devices(udev_enum);
        devices = udev_enumerate_get_list_entry(udev_enum);

        udev_list_entry_foreach(entry, devices) {
            path = udev_list_entry_get_name(entry);
            drm_dev = udev_device_new_from_syspath(udev, path);
            if (!drm_dev) {
                continue;
            }

            pci_dev = udev_device_get_parent_with_subsystem_devtype(drm_dev,
                                                                    "pci", NULL);
            if (pci_dev) {
                vendor_id = udev_device_get_sysattr_value(pci_dev, "vendor");
                if (vendor_id && strtol(vendor_id, NULL, 16) == gpu_vendor) {
                    vendor = VENDOR_GPU_DETECTED;
                    udev_device_unref(drm_dev);
                    break;
                }
            }
            udev_device_unref(drm_dev);
        }
        udev_enumerate_unref(udev_enum);
    }
    udev_unref(udev);

    return vendor;
}
#else
GpuVendor spice_udev_detect_gpu(int gpu_vendor)
{
    return VENDOR_GPU_UNKNOWN;
}
#endif

