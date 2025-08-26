#include <linux/seq_file.h>
#include <drm/drm_debugfs.h>
#include <drm/drm_device.h>

#include "gna_debugfs.h"
#include "gna_device.h"

/*
 * Simple debugfs support to expose a few runtime statistics about the
 * device.  The hardware status register is reported along with the number
 * of requests ever submitted to the engine.
 */

static int gna_hw_status_show(struct seq_file *m, void *data)
{
    struct gna_device *gna = m->private;

    seq_printf(m, "0x%08x\n", gna->hw_status);
    return 0;
}

static int gna_request_count_show(struct seq_file *m, void *data)
{
    struct gna_device *gna = m->private;

    seq_printf(m, "%u\n", gna->request_count);
    return 0;
}

static const struct drm_info_list gna_debugfs_entries[] = {
    { "hw_status", gna_hw_status_show, 0, NULL },
    { "request_count", gna_request_count_show, 0, NULL },
};

const struct drm_info_list *gna_debugfs_files = gna_debugfs_entries;
const int gna_debugfs_file_count = ARRAY_SIZE(gna_debugfs_entries);
