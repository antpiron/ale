LDADD = ../src/libale.la
AM_LDFLAGS = -static
AM_CFLAGS = -g -O0 -I$(top_srcdir)/src

dist_data_DATA = csv-test.csv asntoowner.txt ipv4toasn.txt rir-ripencc.txt
check_PROGRAMS = t-bitset t-mkpath t-mkpath-fopen t-rmpath t-unlink-path t-error-ret t-strjoin t-strtolower t-exparser t-btree-mknode t-btree-slice t-btree-init t-btree-split-right-leaf t-btree-split-right-internal t-btree-split-leaf-child t-btree-split-internal-child t-btree-split-root t-btree-insert t-btree-search t-sl-init t-sl-stack-operations t-sl-mknode t-sl-insert-remove-first-next-nth t-sl-search t-hash t-portability-getrandom t-classifier-sl t-knn t-vla-string t-clean-escaped t-csv-readline t-vector t-skiplist t-netutils t-stats-mean t-stats-diff-mean t-stats-var t-stats-diff-var t-stats-sd t-stats-diff-sd t-stats-cov t-stats-pearson-corr t-stats-mat-cov t-stats-unif-std-rand t-stats-unif-rand t-stats-unif-std-F t-stats-unif-F t-stats-norm-std-rand t-stats-norm-rand t-stats-norm-std-F t-stats-norm-F t-stats-student-F t-stats-student-rand  t-stats-gamma-rand t-stats-gamma-F t-stats-beta-F t-stats-beta-rand t-stats-beta-fit-mm t-math-gamma t-math-lgamma t-math-beta t-math-lbeta t-math-rilgamma t-math-riugamma t-math-ibeta t-stats-t-test t-stats-t-test-paired t-stats-t-test-welch t-process-popen t-fileutils-gzfopen t-fileutils-isgzip

TESTS = $(check_PROGRAMS)

check-valgrind:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do echo "valgrind $$prog"; valgrind -q ./$$prog; done
