#!/bin/bash

if [ $# -ne 0 -a $# -ne 1 ]
then
	echo "Usage: $0 {all|test|clean}"
	exit 1
fi

bash Third_Party/download.bash

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

if [ ${OPT} = "test" ]
then
	cmake -D test=true -D debug=true -D gcov=true ..
else
	cmake -D debug=true -D gcov=true ..
fi
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

