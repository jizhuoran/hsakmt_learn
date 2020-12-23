#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "amd-dbgapi" for configuration "Debug"
set_property(TARGET amd-dbgapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(amd-dbgapi PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "amd_comgr"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/librocm-dbgapi.so.0.42.0"
  IMPORTED_SONAME_DEBUG "librocm-dbgapi.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS amd-dbgapi )
list(APPEND _IMPORT_CHECK_FILES_FOR_amd-dbgapi "${_IMPORT_PREFIX}/lib/librocm-dbgapi.so.0.42.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
