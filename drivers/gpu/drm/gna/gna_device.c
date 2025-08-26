#include <linux/module.h>
#include <linux/platform_device.h>
#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_device.h>
#include <drm/drm_debugfs.h>

#include "gna_device.h"
#include "gna_debugfs.h"

/* Simple stub probe function to illustrate debugfs registration */
static int gna_probe(struct platform_device *pdev)
{
    struct gna_device *gna = devm_kzalloc(&pdev->dev, sizeof(*gna), GFP_KERNEL);
    int ret;

    if (!gna)
        return -ENOMEM;

    /* In a real driver this would set up hardware and register with DRM */
    ret = drm_dev_init(&gna->ddev, NULL, &pdev->dev);
    if (ret)
        return ret;

#ifdef CONFIG_DEBUG_FS
    drm_debugfs_add_files(gna->ddev.primary, gna_debugfs_files,
                          gna_debugfs_file_count);
#endif

    return 0;
}

static struct platform_driver gna_platform_driver = {
    .probe = gna_probe,
    .driver = {
        .name = "gna",
    },
};

module_platform_driver(gna_platform_driver);

MODULE_AUTHOR("ChatGPT");
MODULE_LICENSE("GPL");

