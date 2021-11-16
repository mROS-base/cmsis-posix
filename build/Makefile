include ../Makefile.mros2

#
# compiler
#
GCC			:= gcc
AR			:= ar
RANLIB		:= ranlib
CP			:= /bin/cp
RM			:= /bin/rm

#
# Warning flags
#
WFLAGS			:= -g -Wall
WFLAGS			+= -Wunknown-pragmas
WFLAGS			+= -Wimplicit-int
WFLAGS			+= -Wtrigraphs
WFLAGS			+= -std=gnu99
WFLAGS			+= -mtune=native -march=native -mfpmath=both 

#
# Define flags
#
DFLAGS			:= -DOS_POSIX


CFLAGS			:= $(INCLUDES) -I. -I../test/fw $(WFLAGS)

#
# AR flags
#
AFLAGS			:= -rcSv



LFLAGS	:= -Wl,--allow-multiple-definition
LIBS	:= -lm -lpthread  -ldl


VPATH	:=	$(APPLDIR)
VPATH	+=  ../test/api_test

APPL_COBJS += main.o

all: $(APPL_COBJS)
	gcc -o mros-proc $(APPL_COBJS) $(LIBS)

clean:
	rm -f *.o

.SUFFIXES:      .c .o

.c.o:	$<
	$(GCC) -O0 -c $(CFLAGS) $(DFLAGS) $<
