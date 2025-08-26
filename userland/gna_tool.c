#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <glob.h>
#include <errno.h>
#include <stdint.h>

#include "gna_drm.h"

static int open_render_node(void)
{
    glob_t g;
    int fd = -1;
    if (glob("/dev/dri/renderD*", 0, NULL, &g) == 0) {
        for (size_t i = 0; i < g.gl_pathc; ++i) {
            fd = open(g.gl_pathv[i], O_RDWR);
            if (fd >= 0) {
                printf("Opened %s\n", g.gl_pathv[i]);
                break;
            }
        }
        globfree(&g);
    }
    if (fd < 0)
        perror("open render node");
    return fd;
}

int main(void)
{
    int fd = open_render_node();
    if (fd < 0)
        return 1;

    union gna_parameter param;
    memset(&param, 0, sizeof(param));
    param.in.id = GNA_PARAM_DEVICE_TYPE;
    if (ioctl(fd, DRM_IOCTL_GNA_GET_PARAMETER, &param) == 0)
        printf("Device type: 0x%llx\n", (unsigned long long)param.out.value);
    else
        perror("DRM_IOCTL_GNA_GET_PARAMETER");

    union gna_gem_new gem_new;
    memset(&gem_new, 0, sizeof(gem_new));
    gem_new.in.size = 4096;
    if (ioctl(fd, DRM_IOCTL_GNA_GEM_NEW, &gem_new) == 0)
        printf("Allocated GEM handle %u (%llu bytes)\n", gem_new.out.handle,
               (unsigned long long)gem_new.out.size_granted);
    else
        perror("DRM_IOCTL_GNA_GEM_NEW");

    struct gna_mem_id mem = gem_new.out;

    struct gna_buffer buffer;
    memset(&buffer, 0, sizeof(buffer));
    buffer.handle = mem.handle;
    buffer.size = gem_new.out.size_granted;

    struct gna_compute_cfg cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.buffers_ptr = (uintptr_t)&buffer;
    cfg.buffer_count = 1;

    union gna_compute comp;
    memset(&comp, 0, sizeof(comp));
    comp.in.config = cfg;
    if (ioctl(fd, DRM_IOCTL_GNA_COMPUTE, &comp) == 0)
        printf("Compute request id %llu\n", (unsigned long long)comp.out.request_id);
    else
        perror("DRM_IOCTL_GNA_COMPUTE");

    union gna_wait wait;
    memset(&wait, 0, sizeof(wait));
    wait.in.request_id = comp.out.request_id;
    wait.in.timeout = 1000; /* 1s */
    if (ioctl(fd, DRM_IOCTL_GNA_WAIT, &wait) == 0)
        printf("Wait hw_status 0x%x\n", wait.out.hw_status);
    else
        perror("DRM_IOCTL_GNA_WAIT");

    struct gna_gem_free gem_free;
    memset(&gem_free, 0, sizeof(gem_free));
    gem_free.handle = mem.handle;
    if (ioctl(fd, DRM_IOCTL_GNA_GEM_FREE, &gem_free) != 0)
        perror("DRM_IOCTL_GNA_GEM_FREE");

    close(fd);
    return 0;
}
