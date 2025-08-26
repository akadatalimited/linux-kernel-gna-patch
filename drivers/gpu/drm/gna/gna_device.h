#ifndef _GNA_DEVICE_H_
#define _GNA_DEVICE_H_

#include <drm/drm_device.h>

struct gna_device {
    struct drm_device ddev;
    u32 hw_status;
    u32 request_count;
};

#endif /* _GNA_DEVICE_H_ */
