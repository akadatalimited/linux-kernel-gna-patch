GNA DebugFS Interface
=====================

The GNA driver exposes a few runtime metrics through the DRM debugfs
interface.  Once the driver is loaded, the following files appear under
``/sys/kernel/debug/dri/<minor>/``::

  hw_status
     Current raw value of the hardware status register.

  request_count
     Total number of compute requests submitted to the hardware.

These entries are read-only and intended for developers to inspect the
state of the accelerator for debugging purposes.
