#include <stdio.h>
#include "cmsis_os.h"
#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	osKernelStart();
	return RUN_ALL_TESTS();
}
