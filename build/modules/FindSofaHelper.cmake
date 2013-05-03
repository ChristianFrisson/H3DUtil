# - Find sofa helper library
# Find the native PTHREAD headers and libraries.
#
#  SOFAHELPER_INCLUDE_DIR -  where to find helper.h, AdvancedTimer.h etc.
#  SOFAHELPER_LIBRARIES    - List of libraries when using sofa helper component.
#  SOFAHELPER_FOUND        - True if sofa helper found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

SET( SOFA_LIBRARY_POSTFIX "_1_0" )

# Look for the header file.
FIND_PATH(SOFAHELPER_INCLUDE_DIR   NAMES   sofa/helper/AdvancedTimer.h
                                    PATHS   $ENV{H3D_EXTERNAL_ROOT}/include
                                            $ENV{H3D_EXTERNAL_ROOT}/include/sofahelper/framework
                                            $ENV{H3D_ROOT}/../External/include  
                                            $ENV{H3D_ROOT}/../External/include/sofahelper/framework
                                            ../../External/include
                                            ../../External/include/sofa/framework
                                            ${module_file_path}/../../../External/include
                                            ${module_file_path}/../../../External/include/sofa
                                    DOC     "Path in which the file AdvancedTimer.h is located." )
                                    
MARK_AS_ADVANCED(SOFAHELPER_INCLUDE_DIR)

# Look for the library.

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )
FIND_LIBRARY(SOFAHELPER_LIBRARY    NAMES   SofaHelper${SOFA_LIBRARY_POSTFIX}2 
                                    PATHS   $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                            $ENV{H3D_ROOT}/../External/${LIB}
                                            ../../External/${LIB}
                                            ${module_file_path}/../../../External/${LIB}
                                    DOC     "Path to SofaHelper${SOFA_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(SOFAHELPER_LIBRARY)
FIND_LIBRARY(SOFAHELPER_DEBUG_LIBRARY  NAMES   SofaHelper${SOFA_LIBRARY_POSTFIX}d2
                                        PATHS   $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                                $ENV{H3D_ROOT}/../External/${LIB}
                                                ../../External/${LIB}
                                                ${module_file_path}/../../../External/${LIB}
                                        DOC     "Path to SofaHelper${SOFA_LIBRARY_POSTFIX}d library.")

MARK_AS_ADVANCED(SOFAHELPER_DEBUG_LIBRARY)

# Copy the results to the output variables.
IF(SOFAHELPER_INCLUDE_DIR AND SOFAHELPER_LIBRARY)
  SET(SOFAHELPER_FOUND 1)
  SET(SOFAHELPER_INCLUDE_DIR ${SOFAHELPER_INCLUDE_DIR})
  IF(SOFAHELPER_DEBUG_LIBRARY)
    MESSAGE(STATUS "debug library found")
    SET(SOFAHELPER_LIBRARIES 
        optimized   ${SOFAHELPER_LIBRARY}
        debug       ${SOFAHELPER_DEBUG_LIBRARY})
  ELSE(SOFAHELPER_DEBUG_LIBRARY)
    MESSAGE( STATUS "Sofa helper debug library is not found. Debug compilation might not work with sofa heler." )
    SET(SOFAHELPER_LIBRARIES ${SOFAHELPER_LIBRARY})
  ENDIF(SOFAHELPER_DEBUG_LIBRARY)
ELSE(SOFAHELPER_INCLUDE_DIR AND SOFAHELPER_LIBRARY)
  SET(SOFAHELPER_FOUND 0)
  SET(SOFAHELPER_LIBRARIES)
  SET(SOFAHELPER_INCLUDE_DIR)
ENDIF(SOFAHELPER_INCLUDE_DIR AND SOFAHELPER_LIBRARY)

# Report the results.
IF(NOT SOFAHELPER_FOUND)
  SET(SOFAHELPER_DIR_MESSAGE
    "Sofa helper component not found. Make sure SOFAHELPER_LIBRARY and SOFAHELPER_INCLUDE_DIR are set . Currently, it is only supported on windows. ")
  IF(SOFAHELPER_FIND_REQUIRED)
    SET(SOFAHELPER_DIR_MESSAGE
        "${SOFAHELPER_DIR_MESSAGE} Sofa helper component is required to build.")
    MESSAGE(FALTAL_ERROR "${SOFAHELPER_DIR_MESSAGE}")
  ELSEIF(NOT SOFAHELPER_FIND_QUIETLY)
    SET(SOFAHELPER_DIR_MESSAGE
        "${SOFAHELPER_DIR_MESSAGE} Timer profiling will be disabled without Sofa helper component")
  ENDIF(SOFAHELPER_FIND_REQUIRED)
ENDIF(NOT SOFAHELPER_FOUND)
