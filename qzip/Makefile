###############################################################################
#
# SPDX-License-Identifier: GPL-1.0-only OR GPL-2.0-only OR GPL-3.0-only
#
# Copyright (c) 2006-2011 Lasse Mikkel Reinhold <lar@quicklz.com>
# Copyright (c) 2023 Jeffrey H. Johnson <trnsz@pobox.com>
#
###############################################################################

###############################################################################
# Targets

OUTPUT := | qcat_1 qcat_2 qcat_3
.PHONY: $(OUTPUT) all
all:    $(OUTPUT)

###############################################################################
# Configuration: Build

OPFLAGS   ?= -Ofast
QZFLAGS   ?= -DQLZ_STREAMING_BUFFER=1000000
SFFLAGS   := -DQLZ_MEMORY_SAFE=1
CLFLAGS   ?= $(OPFLAGS) -flto=auto -march=native

###############################################################################
# Configuration: Tools

CC     ?= gcc
RM     ?= rm -f
LN     ?= ln -fs
MV     ?= mv -f
PYTHON ?= python

###############################################################################
# Build variants

qcat_1: ; +@$(MAKE) --no-print-directory qcat1 qzip1 qunzip1 LEVEL=1
qcat_2: ; +@$(MAKE) --no-print-directory qcat2 qzip2 qunzip2 LEVEL=2
qcat_3: ; +@$(MAKE) --no-print-directory qcat3 qzip3 qunzip3 LEVEL=3

###############################################################################
# qcat

qcat$(LEVEL): qzip.c quicklz.c quicklz.h
	-@printf '\n  %s\n\n' "***** Building level $(LEVEL) binary *****"
	$(CC) $(CLFLAGS)      \
		$(QZFLAGS)  \
		$(SFFLAGS)             \
		-DQLZ_COMPRESSION_LEVEL=$(LEVEL)       \
		qzip.c quicklz.c -o qcat$(LEVEL)

###############################################################################
# qzip

qzip$(LEVEL): qcat$(LEVEL)
	$(LN) qcat$(LEVEL) qzip$(LEVEL)

###############################################################################
# qunzip

qunzip$(LEVEL): qzip$(LEVEL)
	$(LN) qcat$(LEVEL) qunzip$(LEVEL)

###############################################################################
# Python module

.PHONY: python
python quicklz.so: quicklz.o distutils
	env CFLAGS="$(CLFLAGS) -Wno-int-conversion" $(PYTHON) distutils build

###############################################################################
# Benchmark target

.PHONY: bench perf
bench perf: $(OUTPUT) quicklz.c
	+@$(MAKE) q_bench --no-print-directory ||      \
	  {  printf '\n  %s\n\n'                       \
	       "***** ERROR!! BENCH FAILED!! *****" && \
	     $(RM) /tmp/zero.qz?   > /dev/null 2>&1 && \
	     $(RM) /tmp/random.qz? > /dev/null 2>&1 && \
	     exit 1;                                   \
	  }; exit 0

###############################################################################
# Test target

.PHONY: test check
test check: $(OUTPUT) quicklz.c
	+@$(MAKE) q_test --no-print-directory ||       \
	  {  printf '\n  %s\n\n'                       \
	       "***** ERROR!! TESTS FAILED!! *****" && \
	     exit 1;                                   \
	  }; exit 0

###############################################################################
# Test script

.PHONY: q_test
q_test: quicklz.c
	-@printf '\n  %s\n\n' "***** Starting verification tests *****"
	CKSUM=`cksum < quicklz.c` &&            \
	      ./qzip1 < quicklz.c | ./qcat1 |   \
	      cksum | grep -q "^$${CKSUM}$$" &&   \
	      ./qzip2 < quicklz.c | ./qcat2 |   \
	      cksum | grep -q "^$${CKSUM}$$" &&   \
	      ./qzip3 < quicklz.c | ./qcat3 |   \
	      cksum | grep -q "^$${CKSUM}$$"
	-@printf '\n  %s\n\n' "***** Tests completed successfully! *****"

###############################################################################
# Benchmark script

.PHONY: q_bench
q_bench: quicklz.c
	-@printf '\n  %s\n\n' "***** Starting benchmarking tests *****"
	-@printf '  %s\n\n'  " ##### Compression benchmark #####"
	-@printf ' %s\n'    "  ==> Benchmark: /dev/urandom"
	-@sync                                                 > /dev/null 2>&1
	+@dd if="/dev/urandom" bs="1000000" count="4096"      2> /dev/null | \
	  pv -erc -i "0.1" -s "4096000000" -N "         Level 1" -B "1024" | \
	    ./qzip1 > /tmp/random.qz1
	-@sync                                                 > /dev/null 2>&1
	+@dd if="/dev/urandom" bs="1000000" count="2048"      2> /dev/null | \
	  pv -erc -i "0.1" -s "2048000000" -N "         Level 2" -B "1024" | \
	    ./qzip2 > /tmp/random.qz2
	-@sync                                                 > /dev/null 2>&1
	+@dd if="/dev/urandom" bs="1000000" count="1024"      2> /dev/null | \
	  pv -erc -i "0.1" -s "1024000000" -N "         Level 3" -B "1024" | \
	    ./qzip3 > /tmp/random.qz3
	-@sync                                                 > /dev/null 2>&1
	-@printf '\n %s\n' "  ==> Benchmark: /dev/zero"
	+@dd if="/dev/zero" bs="1000000" count="4096"         2> /dev/null | \
	  pv -erc -i "0.1" -s "4096000000" -N "         Level 1" -B "1024" | \
	    ./qzip1 > /tmp/zero.qz1
	-@sync                                                 > /dev/null 2>&1
	+@dd if="/dev/zero" bs="1000000" count="2048"         2> /dev/null | \
	  pv -erc -i "0.1" -s "2048000000" -N "         Level 2" -B "1024" | \
	    ./qzip2 > /tmp/zero.qz2
	-@sync                                                 > /dev/null 2>&1
	+@dd if="/dev/zero" bs="1000000" count="1024"         2> /dev/null | \
	  pv -erc -i "0.1" -s "1024000000" -N "         Level 3" -B "1024" | \
	    ./qzip3 > /tmp/zero.qz3
	-@sync                                                 > /dev/null 2>&1
	-@printf '\n  %s\n\n' " ##### Decompression benchmark #####"
	-@printf ' %s\n'     "  ==> Benchmark: /dev/urandom"
	-@cat "/tmp/random.qz1"                                > /dev/null 2>&1
	+@dd if="/tmp/random.qz1" bs="1000000"                2> /dev/null | \
	                                              ./qcat1 2> /dev/null | \
	  pv -erc -i "0.1" -s "4096000000" -N "         Level 1" -B "1024"   \
	                                                       > /dev/null
	-@$(RM) "/tmp/random.qz1"
	-@cat "/tmp/random.qz2"                                > /dev/null 2>&1
	+@dd if="/tmp/random.qz2" bs="1000000"                2> /dev/null | \
	                                              ./qcat2 2> /dev/null | \
	  pv -erc -i "0.1" -s "2048000000" -N "         Level 2" -B "1024"   \
	                                                       > /dev/null
	-@$(RM) "/tmp/random.qz2"
	-@cat "/tmp/random.qz3"                                > /dev/null 2>&1
	+@dd if="/tmp/random.qz3" bs="1000000"                2> /dev/null | \
	                                              ./qcat3 2> /dev/null | \
	  pv -erc -i "0.1" -s "1024000000" -N "         Level 3" -B "1024"   \
	                                                       > /dev/null
	-@$(RM) "/tmp/random.qz3"
	-@printf '\n %s\n'   "  ==> Benchmark: /dev/zero"
	-@cat "/tmp/zero.qz1"                                  > /dev/null 2>&1
	+@dd if="/tmp/zero.qz1" bs="1000000"                  2> /dev/null | \
	                                              ./qcat1 2> /dev/null | \
	  pv -erc -i "0.1" -s "4096000000" -N "         Level 1" -B "1024"   \
	                                                       > /dev/null
	-@$(RM) "/tmp/zero.qz1"
	-@cat "/tmp/zero.qz2"                                  > /dev/null 2>&1
	+@dd if="/tmp/zero.qz2" bs="1000000"                  2> /dev/null | \
	                                              ./qcat2 2> /dev/null | \
	  pv -erc -i "0.1" -s "2048000000" -N "         Level 2" -B "1024"   \
	                                                       > /dev/null
	-@$(RM) "/tmp/zero.qz2"
	-@cat "/tmp/zero.qz3"                                  > /dev/null 2>&1
	+@dd if="/tmp/zero.qz3" bs="1000000"                  2> /dev/null | \
	                                              ./qcat3 2> /dev/null | \
	  pv -erc -i "0.1" -s "1024000000" -N "         Level 3" -B "1024"   \
	                                                       > /dev/null
	-@$(RM) "/tmp/zero.qz3"
	-@printf '\n   %s\n\n' "***** Benchmarking tests completed! *****"

###############################################################################
# Clean-up

.PHONY: clean distclean
ifneq (,$(findstring clean,$(MAKECMDGOALS)))
.NOTPARALLEL: clean distclean
endif
clean distclean:
	-@printf '\n  %s\n\n' "***** Starting source tree cleaning *****"
	-$(RM) -r build/
	-$(RM) qcat? qzip? qunzip? *.so *.o      \
		*.bak *~ core *.core
	-@printf '\n  %s\n\n' "***** Cleaning completed successfully! *****"

###############################################################################
