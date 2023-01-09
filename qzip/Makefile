all: qcat qzip qunzip quicklz.so

CC     ?= gcc
PYTHON ?= python

RM ?= rm -f
LN ?= ln -s

# Uncomment these for debugging
#LDFLAGS = -g
#CFLAGS  = -g

CFLAGS  ?= -Ofast -flto=auto -march=native
LDFLAGS ?= $(CFLAGS)

qcat: qzip.o quicklz.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o qcat qzip.o quicklz.o


qzip: qzip.o quicklz.o qcat
	$(LN) qcat qzip


qunzip: qzip.o quicklz.o qcat
	$(LN) qcat qunzip


quicklz.so: quicklz.o distutils
	$(PYTHON) distutils build


clean:
	$(RM) qcat qzip qunzip quicklz.so
	$(RN) *.o
	$(RM) -r build/
