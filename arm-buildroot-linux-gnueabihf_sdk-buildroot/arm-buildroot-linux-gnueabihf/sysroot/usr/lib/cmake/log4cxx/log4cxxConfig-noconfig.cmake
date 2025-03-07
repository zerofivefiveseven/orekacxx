#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "log4cxx" for configuration ""
set_property(TARGET log4cxx APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(log4cxx PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/liblog4cxx.so.15.4.0"
  IMPORTED_SONAME_NOCONFIG "liblog4cxx.so.15"
  )

list(APPEND _IMPORT_CHECK_TARGETS log4cxx )
list(APPEND _IMPORT_CHECK_FILES_FOR_log4cxx "/home/revyakin/orekacxx/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/liblog4cxx.so.15.4.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
