#
# Makefile for a cmsis-asp3 test
#

TOPPERS_KERNEL = asp3

# compile switch
CDEFS := -DUSE_ASP3_FOR_STM

# name of main application
APPLNAME = test

# information for directory tree
ROOTDIR           := ../../..
DRIVERSDIR        := $(ROOTDIR)/STM32CubeF7/Drivers
MIDDLESDIR        := $(ROOTDIR)/STM32CubeF7/Middlewares
CMSIS_MAKEFILEDIR := ${ROOTDIR}/cmsis-asp3
ASP3_MAKEFILEDIR  := ${ROOTDIR}/asp3-f767zi

include Makefile.test

# cmsis_asp3 makefiles include
include ${CMSIS_MAKEFILEDIR}/Makefile.mros2

# asp3-f767zi makefiles include
include ${ASP3_MAKEFILEDIR}/Makefile.mros2

