AM_CFLAGS = -Wall -Wextra -I..
AM_LFLAGS=-R


LIBRARY_SOURCES=ale/error.h error.c ale/daemon.h daemon.c portability.c ale/portability.h.in ale/bitset.h ale/fileutils.h fileutils.c ale/stringutils.h stringutils.c ale/exparser.h exparser.c btree.c btree-funcs.c ale/btree.h sl_list.c ale/sl_list.h ale/hash.h ale/siphash24.h siphash24.c ale/knn.h knn.c csv_lexer.l csv_tokens.h csv_tokens.c ale/vector.h ale/skiplist.h netutils.c ale/netutils.h ale/stats.h stats/base.c stats/categorical.c stats/norm.c stats/student.c stats/binom.c stats/poisson.c stats/unif.c stats/hyper.c stats/beta.c stats/gamma.c stats/kolmogorov.c stats/permutation.c ale/math.h math.c ale/algebra.h algebra.c ale/process.h process.c ale/matrix.h matrix.c ale/index.h index.c ale/memory.h memory.c ale/stats-normalize.h stats-normalize.c ale/ea.h ea.c ale/sort.h sort.c

lib_LTLIBRARIES = libale.la 
noinst_LTLIBRARIES=libale-debug.la
#noinst_lib_LTLIBRARIES=libale-debug.la
libale_la_SOURCES = $(LIBRARY_SOURCES)
BUILT_SOURCES = csv_lexer.h csv_lexer.c 
include_HEADERS = ale.h
pkginclude_HEADERS = ale/error.h ale/daemon.h ale/portability.h ale/bitset.h ale/fileutils.h ale/stringutils.h ale/exparser.h ale/btree.h ale/sl_list.h ale/hash.h ale/siphash24.h ale/knn.h ale/vector.h ale/skiplist.h ale/netutils.h ale/stats.h ale/math.h ale/algebra.h ale/process.h ale/matrix.h ale/index.h ale/memory.h ale/stats-normalize.h ale/ea.h ale/sort.h

libale_debug_la_SOURCES = $(LIBRARY_SOURCES)
libale_debug_la_CFLAGS=-g -O0 -fsanitize=bounds

bin_PROGRAMS = ipinfo
ipinfo_SOURCES = ipinfo.c
ipinfo_LDADD = libale.la
ipinfo_LDFLAGS = -static


csv_lexer.c csv_lexer.h:	csv_lexer.l
	$(LEX) -R csv_lexer.l
csv_lexer.h:	csv_lexer.l csv_lexer.c
