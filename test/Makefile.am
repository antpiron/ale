LDADD = ../src/libale.la
AM_LDFLAGS = -static
AM_CFLAGS = -g -O0 -I$(top_srcdir)/src

check_PROGRAMS = t-bitset t-mkpath t-mkpath-fopen t-rmpath t-unlink-path t-error-ret t-strjoin t-strtolower t-exparser t-btree-mknode t-btree-slice t-btree-init t-btree-split-right-leaf t-btree-split-right-internal t-btree-split-leaf-child t-btree-split-internal-child t-btree-split-root t-btree-insert t-btree-search t-sl-init t-sl-stack-operations t-sl-mknode t-sl-insert-remove-first-next-nth t-sl-search t-hash-init-destroy t-hash-set-get t-hash-del t-hash-foreach t-portability-getrandom t-classifier-sl t-knn t-vla-string t-clean-escaped t-csv-readline t-vector

TESTS = $(check_PROGRAMS)
