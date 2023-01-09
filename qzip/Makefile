###############################################################################

.PHONY: all
all:    qcat1 qzip1 qunzip1 qcat2 qzip2 qunzip2 qcat3 qzip3 qunzip3

###############################################################################

CC     ?= gcc
RM     ?= rm -f
LN     ?= ln -fs
MV     ?= mv -f
PYTHON ?= python

###############################################################################

OPFLAGS ?= -Ofast
QZFLAGS ?= -DQLZ_STREAMING_BUFFER=1000000
CLFLAGS ?= $(OPFLAGS) -flto=auto -march=native

###############################################################################

qcat1 qzip1 qunzip1: \
	qzip.c quicklz.c quicklz.h
	$(CC) $(CLFLAGS)      \
		$(QZFLAGS)  \
		-DQLZ_COMPRESSION_LEVEL=1       \
		qzip.c quicklz.c -o qcat1 &&    \
		$(LN) qcat1 qzip1 &&            \
		$(LN) qcat1 qunzip1

###############################################################################

qcat2 qzip2 qunzip2: \
	qzip.c quicklz.c quicklz.h
	$(CC) $(CLFLAGS)      \
		$(QZFLAGS)  \
		-DQLZ_COMPRESSION_LEVEL=2       \
		qzip.c quicklz.c -o qcat1 &&    \
		$(LN) qcat2 qzip1 &&            \
		$(LN) qcat2 qunzip2

###############################################################################

qcat3 qzip3 qunzip3: \
	qzip.c quicklz.c quicklz.h
	$(CC) $(CLFLAGS)      \
		$(QZFLAGS)  \
		-DQLZ_COMPRESSION_LEVEL=3       \
		qzip.c quicklz.c -o qcat1 &&    \
		$(LN) qcat3 qzip3 &&            \
		$(LN) qcat3 qunzip3

###############################################################################

.PHONY: python
python quicklz.so: quicklz.o distutils
	$(PYTHON) distutils build

###############################################################################

.PHONY: clean
clean:
	-$(RM) qcat? qzip? qunzip? *.so *.o *.bak *~ core *.core
	-$(RM) -r build/

###############################################################################
