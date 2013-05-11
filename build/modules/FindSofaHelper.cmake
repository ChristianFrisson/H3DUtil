# - Find sofa helper library
# Find the native PTHREAD headers and libraries.
#
#  SOFAHELPER_INCLUDE_DIR -  where to find helper.h, AdvancedTimer.h etc.
#  SOFAHELPER_LIBRARIES    - List of libraries when using sofa helper component.
#  SOFAHELPER_FOUND        - True if sofa helper found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

SET( SOFA_LIBRARY_POSTFIX "_1_0" )

IF ( NOT SOFA_DELAY_FIND_LIBS )
  IF( H3D_USE_DEPENDENCIES_ONLY AND WIN32 )
    SET(SOFA_INSTALL_DIR "${EXTERNAL_INCLUDE_DIR}/sofa" CACHE PATH "Path to root of SOFA installation" )
  ELSE( H3D_USE_DEPENDENCIES_ONLY AND WIN32 )
    IF( DEFINED ENV{SOFA_ROOT} )
      SET(SOFA_INSTALL_DIR "$ENV{SOFA_ROOT}" CACHE PATH "Path to root of SOFA installation" )
    ELSE( DEFINED ENV{SOFA_ROOT} )
      IF( DEFINED ENV{H3D_EXTERNAL_ROOT} )
        SET(SOFA_INSTALL_DIR "$ENV{H3D_EXTERNAL_ROOT}/include/sofa" CACHE PATH "Path to root of SOFA installation" )
      ELSE( DEFINED ENV{H3D_EXTERNAL_ROOT} )
        IF ( DEFINED ENV{H3D_ROOT} )
          SET(SOFA_INSTALL_DIR "$ENV{H3D_ROOT}/../External/include/sofa" CACHE PATH "Path to root of SOFA installation" )
        ELSE ( DEFINED ENV{H3D_ROOT} )
          SET(SOFA_INSTALL_DIR "NOTFOUND" CACHE PATH "Path to root of SOFA installation" )
        ENDIF ( DEFINED ENV{H3D_ROOT} )
      ENDIF( DEFINED ENV{H3D_EXTERNAL_ROOT} )
    ENDIF( DEFINED ENV{SOFA_ROOT} )
  ENDIF( H3D_USE_DEPENDENCIES_ONLY AND WIN32 )
ENDIF ()

# Look for the header file.
FIND_PATH(SOFAHELPER_INCLUDE_DIR   NAMES   sofa/helper/AdvancedTimer.h
                                    PATHS   ${SOFA_INSTALL_DIR}/framework
											$ENV{H3D_EXTERNAL_ROOT}/include
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
IF ( SOFA_DELAY_FIND_LIBS )
    # Assume it will be build before it is used by this project
    IF( WIN32 )
      SET( SOFAHELPER_LIBRARY ${SOFA_INSTALL_DIR}/lib/SofaHelper${SOFA_LIBRARY_POSTFIX}.lib CACHE FILE "Sofa helper library" )
      SET( SOFAHELPER_DEBUG_LIBRARY ${SOFA_INSTALL_DIR}/lib/SofaHelper${SOFA_LIBRARY_POSTFIX}d.lib CACHE FILE "Sofa helper debug library" )
    ELSEIF ( UNIX )
      SET( SOFAHELPER_LIBRARY ${SOFA_INSTALL_DIR}/lib/libSofaHelper${SOFA_LIBRARY_POSTFIX}.so CACHE FILE "Sofa helper library" )
      SET( SOFAHELPER_DEBUG_LIBRARY ${SOFA_INSTALL_DIR}/lib/libSofaHelper${SOFA_LIBRARY_POSTFIX}d.so CACHE FILE "Sofa helper debug library" )
    ENDIF ()
	MARK_AS_ADVANCED(SOFAHELPER_LIBRARY)
	MARK_AS_ADVANCED(SOFAHELPER_DEBUG_LIBRARY)
ELSE ()
FIND_LIBRARY(SOFAHELPER_LIBRARY    NAMES   SofaHelper${SOFA_LIBRARY_POSTFIX}
                                    PATHS   ${SOFA_INSTALL_DIR}/lib
											$ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                            $ENV{H3D_ROOT}/../External/${LIB}
                                            ../../External/${LIB}
                                            ${module_file_path}/../../../External/${LIB}
                                    DOC     "Path to SofaHelper${SOFA_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(SOFAHELPER_LIBRARY)
FIND_LIBRARY(SOFAHELPER_DEBUG_LIBRARY  NAMES   SofaHelper${SOFA_LIBRARY_POSTFIX}d
                                        PATHS   ${SOFA_INSTALL_DIR}/lib
												$ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                                $ENV{H3D_ROOT}/../External/${LIB}
                                                ../../External/${LIB}
                                                ${module_file_path}/../../../External/${LIB}
                                        DOC     "Path to SofaHelper${SOFA_LIBRARY_POSTFIX}d library.")

MARK_AS_ADVANCED(SOFAHELPER_DEBUG_LIBRARY)
ENDIF()



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
