###############################################################################
# Targets

OUTPUT := qcat_1 qcat_2 qcat_3
.PHONY: $(OUTPUT) all
all:    $(OUTPUT)

###############################################################################
# Configuration: Build

OPFLAGS ?= -Ofast
QZFLAGS ?= -DQLZ_STREAMING_BUFFER=1000000
CLFLAGS ?= $(OPFLAGS) -flto=auto -march=native
#CLFLAGS += -DQLZ_MEMORY_SAFE=1

###############################################################################
# Configuration: Tools

CC     ?= gcc
RM     ?= rm -f
LN     ?= ln -fs
MV     ?= mv -f
PYTHON ?= python

###############################################################################
# Build variants

qcat_1: ; +$(MAKE) qcat1 qzip1 qunzip1 LEVEL=1
qcat_2: ; +$(MAKE) qcat2 qzip2 qunzip2 LEVEL=2
qcat_3: ; +$(MAKE) qcat3 qzip3 qunzip3 LEVEL=3

###############################################################################
# qcat

qcat$(LEVEL): \
	qzip.c quicklz.c quicklz.h
	$(CC) $(CLFLAGS)      \
		$(QZFLAGS)  \
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
	$(PYTHON) distutils build

###############################################################################
# Test target

.PHONY: q_test test check
test check: $(OUTPUT) quicklz.c
	+@$(MAKE) q_test ||                          \
	  {                                          \
	    printf '\n  %s\n\n'                      \
	      "***** ERROR! TESTS FAILED! *****" &&  \
	    exit 1;                                  \
	  };                                         \
	exit 0

###############################################################################
# Test script

q_test: $(OUTPUT) quicklz.c
	+$(MAKE) all
	CKSUM=`cksum < quicklz.c` &&            \
	      ./qzip1 < quicklz.c | ./qcat1 |   \
	      cksum | grep -q "^$${CKSUM}$$" &&   \
	      ./qzip2 < quicklz.c | ./qcat2 |   \
	      cksum | grep -q "^$${CKSUM}$$" &&   \
	      ./qzip3 < quicklz.c | ./qcat3 |   \
	      cksum | grep -q "^$${CKSUM}$$"
	@printf '\n  %s\n\n' \
	  "***** Tests completed successfully! *****"

###############################################################################
# Clean-up

.PHONY: clean distclean
ifneq (,$(findstring clean,$(MAKECMDGOALS)))
.NOTPARALLEL: clean distclean
endif
clean distclean:
	-$(RM) -r build/
	-$(RM) qcat? qzip? qunzip? *.so *.o      \
		*.bak *~ core *.core

###############################################################################
