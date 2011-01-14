#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Compute the installation prefix relative to this file.
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

# Import target "Box2D_shared" for configuration "RelWithDebInfo"
SET_PROPERTY(TARGET Box2D_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
SET_TARGET_PROPERTIES(Box2D_shared PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libBox2D.dylib"
  IMPORTED_SONAME_RELWITHDEBINFO "libBox2D.dylib"
  )

# Import target "Box2D" for configuration "RelWithDebInfo"
SET_PROPERTY(TARGET Box2D APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
SET_TARGET_PROPERTIES(Box2D PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libBox2D.a"
  )

# Cleanup temporary variables.
SET(_IMPORT_PREFIX)

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)
