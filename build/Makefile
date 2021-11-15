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


CFLAGS			:= $(INCLUDES) -I. $(WFLAGS)

#
# AR flags
#
AFLAGS			:= -rcSv



LFLAGS	:= -Wl,--allow-multiple-definition
LIBS	:= -lm -lpthread  -ldl


VPATH	:=	$(APPLDIR)

all: $(APPL_COBJS)

clean:
	rm -f *.o

.SUFFIXES:      .c .o

.c.o:	$<
	$(GCC) -O0 -c $(CFLAGS) $(DFLAGS) $<