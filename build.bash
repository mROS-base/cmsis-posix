#!/bin/bash

CMSIS_OS_BRANCH="mros2"
CMSIS_OS_HDIR="Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
CMSIS_OS_REPO=https://raw.githubusercontent.com/mROS-base/STM32CubeF7/${CMSIS_OS_BRANCH}/Middlewares/${CMSIS_OS_HDIR}

function download_cmsis_osheader()
{
	if [ -d ${CMSIS_OS_HDIR} ]
	then
		:
	else
		wget "${CMSIS_OS_REPO}/cmsis_os.h"
		wget "${CMSIS_OS_REPO}/cmsis_os2.h"
		mkdir -p ${CMSIS_OS_HDIR}
		mv cmsis_os.h ${CMSIS_OS_HDIR}/
		mv cmsis_os2.h ${CMSIS_OS_HDIR}/
	fi
}

if [ $# -ne 0 -a $# -ne 1 ]
then
	echo "Usage: $0 {all|test|clean}"
	exit 1
fi

download_cmsis_osheader

OPT=${1}
if [ $# -eq 0 ]
then
	OPT="all"
fi

echo $OPT

cd cmake-build
if [ ${OPT} = "clean" ]
then
	rm -rf ./*
	exit 0
fi

cmake -D debug=true -D gcov=true ..
make

if [ ${OPT} = "test" ]
then
	make test
	cd src/CMakeFiles/cmsis.dir/api
	lcov -c -d . -o lcov.info
	genhtml lcov.info -o ./info
	cd ../core
	lcov -c -d . -o lcov.info
	genhtml lcov.info -o ./info
fi

