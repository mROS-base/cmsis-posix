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


CFLAGS			:= $(INCLUDES) -I. -I../test/fw -I../test/api_test $(WFLAGS)

#
# AR flags
#
AFLAGS			:= -rcSv



LFLAGS	:= -Wl,--allow-multiple-definition
LIBS	:= -lm -lpthread  -ldl


VPATH	:=	$(APPLDIR)
VPATH	+=  ../test/api_test
VPATH	+=  ../test/fw

APPL_COBJS += main.o
APPL_COBJS += cmsis_thread_test.o
APPL_COBJS += cmsis_generic_test.o
APPL_COBJS += cmsis_kernel_test.o
APPL_COBJS += autosar_os_ext_testfw.o

all: $(APPL_COBJS)
	gcc -o mros-proc $(APPL_COBJS) $(LIBS)

clean:
	rm -f *.o mros-proc

.SUFFIXES:      .c .o

.c.o:	$<
	$(GCC) -O0 -g -c $(CFLAGS) $(DFLAGS) $<
