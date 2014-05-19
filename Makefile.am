ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src test

perf-check:
	cd src/ && $(MAKE) $(AM_MAKEFLAGS)
	cd perf && $(MAKE) $(AM_MAKEFLAGS) perf-check

.PHONY:	perf-check
