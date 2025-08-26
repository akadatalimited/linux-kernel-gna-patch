CC ?= cc
CFLAGS ?= -Wall -Wextra -O2
PKG_CONFIG ?= pkg-config
LIBDRM_CFLAGS := $(shell $(PKG_CONFIG) --cflags libdrm 2>/dev/null)
LIBDRM_LIBS := $(shell $(PKG_CONFIG) --libs libdrm 2>/dev/null)

all: gna_tool

gna_tool: gna_tool.c gna_drm.h
	$(CC) $(CFLAGS) $(LIBDRM_CFLAGS) -o $@ gna_tool.c $(LIBDRM_LIBS)

clean:
	rm -f gna_tool

.PHONY: all clean
