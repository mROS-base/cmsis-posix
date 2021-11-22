set(MY_CMAKE_VERSION 3.5.1)
cmake_minimum_required(VERSION ${MY_CMAKE_VERSION})

project(cmsis-posix-project
    VERSION 1.0.0
    DESCRIPTION "cmsis-posix"
    LANGUAGES C CXX
)
enable_testing()

add_compile_definitions(OS_POSIX)
include_directories("${PROJECT_SOURCE_DIR}/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2")

set(CMAKE_C_FLAGS "-std=gnu99")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wunknown-pragmas")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wtrigraphs")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wimplicit-int")

set(BUILD_TYPE "release")
if (debug)
    set(BUILD_TYPE "debug")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -O0")
else()
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -O2")
endif()

set(GCOV "disabled")
if (gcov)
    set(GCOV "enabled")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

message(STATUS "BUILD_TYPE " ${BUILD_TYPE})
message(STATUS "GCOV " ${GCOV})

add_subdirectory(src)

if (test)
    add_subdirectory(test)
endif()