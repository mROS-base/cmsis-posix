set(MY_CMAKE_VERSION 3.5.1)
cmake_minimum_required(VERSION ${MY_CMAKE_VERSION})

project(cmsis-posix-project
    VERSION 1.0.0
    DESCRIPTION "cmsis-posix"
    LANGUAGES C CXX
)
enable_testing()
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON) 

add_compile_definitions(OS_POSIX)
set(CMSIS_INTERFACE_DIR "${PROJECT_SOURCE_DIR}/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2")

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

set(INSTALL_CMAKE_DIR ${PROJECT_SOURCE_DIR}/public)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/src/include/
	DESTINATION ${INSTALL_CMAKE_DIR}/include
)
install(
	DIRECTORY ${CMSIS_INTERFACE_DIR}/
	DESTINATION ${INSTALL_CMAKE_DIR}/include
)

install(
	TARGETS cmsis 
	DESTINATION     ${INSTALL_CMAKE_DIR} 
	EXPORT cmsis-export
    LIBRARY         DESTINATION lib
    INCLUDES        DESTINATION include
    PUBLIC_HEADER   DESTINATION include
)
install(
	EXPORT cmsis-export 
    FILE    cmsis-config.cmake
	DESTINATION ${INSTALL_CMAKE_DIR}
    EXPORT_LINK_INTERFACE_LIBRARIES
)
