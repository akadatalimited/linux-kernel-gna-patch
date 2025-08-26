CC ?= cc
CFLAGS ?= -Wall -Wextra -O2

all: gna_tool

gna_tool: gna_tool.c gna_drm.h
	$(CC) $(CFLAGS) -o $@ gna_tool.c

clean:
	rm -f gna_tool

.PHONY: all clean
