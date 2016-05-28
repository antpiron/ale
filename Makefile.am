ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src test doc

perf-check:
	cd src/ && $(MAKE) $(AM_MAKEFLAGS)
	cd perf && $(MAKE) $(AM_MAKEFLAGS) perf-check

check-valgrind:
	cd test && $(MAKE) $(AM_MAKEFLAGS) check-valgrind

.PHONY:	perf-check check-valgrind
