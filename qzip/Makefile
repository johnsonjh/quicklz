###############################################################################
# Targets

OUTPUT := | qcat_1 qcat_2 qcat_3
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

qcat_1: ; +@$(MAKE) --no-print-directory qcat1 qzip1 qunzip1 LEVEL=1
qcat_2: ; +@$(MAKE) --no-print-directory qcat2 qzip2 qunzip2 LEVEL=2
qcat_3: ; +@$(MAKE) --no-print-directory qcat3 qzip3 qunzip3 LEVEL=3

###############################################################################
# qcat

qcat$(LEVEL): \
	qzip.c quicklz.c quicklz.h
	-@printf '\n  %s\n\n' \
	  "***** Building level $(LEVEL) binary *****"
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
	env CFLAGS="$(CFLAGS) -Wno-int-conversion" $(PYTHON) distutils build

###############################################################################
# Test target

.PHONY: test check
test check: $(OUTPUT) quicklz.c
	+@$(MAKE) q_test --no-print-directory ||       \
	  {                                            \
	     printf '\n  %s\n\n'                       \
	       "***** ERROR!! TESTS FAILED!! *****" && \
	     exit 1;                                   \
	  }; exit 0

###############################################################################
# Test script

.PHONY: q_test
q_test: quicklz.c
	-@printf '\n  %s\n\n' \
	  "***** Starting verification tests *****"
	CKSUM=`cksum < quicklz.c` &&            \
	      ./qzip1 < quicklz.c | ./qcat1 |   \
	      cksum | grep -q "^$${CKSUM}$$" &&   \
	      ./qzip2 < quicklz.c | ./qcat2 |   \
	      cksum | grep -q "^$${CKSUM}$$" &&   \
	      ./qzip3 < quicklz.c | ./qcat3 |   \
	      cksum | grep -q "^$${CKSUM}$$"
	-@printf '\n  %s\n\n' \
	  "***** Tests completed successfully! *****"

###############################################################################
# Clean-up

.PHONY: clean distclean
ifneq (,$(findstring clean,$(MAKECMDGOALS)))
.NOTPARALLEL: clean distclean
endif
clean distclean:
	-@printf '\n  %s\n\n' \
	  "***** Starting source tree clean-up *****"
	-$(RM) -r build/
	-$(RM) qcat? qzip? qunzip? *.so *.o      \
		*.bak *~ core *.core
	-@printf '\n  %s\n\n' \
	  "***** Cleaning completed successfully! *****"

###############################################################################
