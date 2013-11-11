# - Find vld(visual leak detector)
# Find the visual leak detector headers and libraries.
#
#  VLD_INCLUDE_DIR -  where to find vld.h,vld_def.h, etc.
#  VLD_LIBRARIES    - List of libraries when using vld.
#  VLD_FOUND        - True if vld found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( CMAKE_CL_64 )
  MESSAGE( SEND_ERROR "64 bit version of visual leak detector is not tested yet." )
  SET( LIB "lib32" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

# Look for the header file.
FIND_PATH(VLD_INCLUDE_DIR    NAMES vld/vld.h
                             PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                   $ENV{H3D_ROOT}/../External/include
                                   ../../External/include
                                   ${module_file_path}/../../../External/include
                             DOC "Path in which the file vld/vld.h is located." )
MARK_AS_ADVANCED(VLD_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(VLD_LIBRARY    NAMES  vld
                            PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                  $ENV{H3D_ROOT}/../External/${LIB}
                                  ../../External/${LIB}
                                  ${module_file_path}/../../../External/${LIB}
                            DOC "Path to vld library." )
MARK_AS_ADVANCED(VLD_LIBRARY)

SET( VLD_LIBRARIES_FOUND 0 )

IF( VLD_LIBRARY )
  SET( VLD_LIBRARIES_FOUND 1 )
ENDIF( VLD_LIBRARY )

# Copy the results to the output variables.
IF(VLD_INCLUDE_DIR AND VLD_LIBRARIES_FOUND)
  SET(VLD_FOUND 1)
  SET(VLD_LIBRARIES ${VLD_LIBRARY})
  SET(VLD_INCLUDE_DIR ${VLD_INCLUDE_DIR})
ELSE(VLD_INCLUDE_DIR AND VLD_LIBRARIES_FOUND)
  SET(VLD_FOUND 0)
  SET(VLD_LIBRARIES)
  SET(VLD_INCLUDE_DIR)
ENDIF(VLD_INCLUDE_DIR AND VLD_LIBRARIES_FOUND)

# Report the results.
IF(NOT VLD_FOUND)
  SET(VLD_DIR_MESSAGE
    "visual leak detector was not found. Make sure VLD_LIBRARY and VLD_INCLUDE_DIR are set to the directory of your visual leak detector lib and include files.")
  IF(VLD_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${VLD_DIR_MESSAGE}")
  ELSEIF(NOT VLD_FIND_QUIETLY)
    MESSAGE(STATUS "${VLD_DIR_MESSAGE}")
  ENDIF(VLD_FIND_REQUIRED)
ENDIF(NOT VLD_FOUND)
