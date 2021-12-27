#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cmsis" for configuration ""
set_property(TARGET cmsis APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(cmsis PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "/mnt/c/project/tokyo-univ/private/app-mros2-posix/cmsis-posix/public/libcmsis.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS cmsis )
list(APPEND _IMPORT_CHECK_FILES_FOR_cmsis "/mnt/c/project/tokyo-univ/private/app-mros2-posix/cmsis-posix/public/libcmsis.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
