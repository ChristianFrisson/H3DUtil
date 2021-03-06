IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
PROJECT(H3DUtil)


# Make sure the full path is used for installed libraries
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")


# Add the directory to INCLUDE_DIRECTORIES before any other statement to make sure that this header file is found first.
# This assumes that we always use "#include <path/include_header.h>" in our code even if the file is actually found in
# the same directory as the file that contains the include statement.
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/include )
SET( H3DUTIL_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include ${H3DUtil_SOURCE_DIR}/../include CACHE INTERNAL "Set to internal to propagate change" )

INCLUDE_DIRECTORIES( ${H3DUtil_SOURCE_DIR}/../include )

# set the install directory to the H3D directory on Windows
IF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${H3DUtil_SOURCE_DIR}/../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  SET( H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET TRUE )
ENDIF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )

# Add a cache variable GENERATE_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DAPI. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_CPACK_PROJECT )
  SET( GENERATE_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_CPACK_PROJECT )

# Add a cache variable PREFER_STATIC_LIBRARIES to have the choice of generating a project
# linking against static libraries if they exist. Default is NO since most people will not use this.
IF( NOT DEFINED PREFER_STATIC_LIBRARIES )
  SET( PREFER_STATIC_LIBRARIES "NO" CACHE BOOL "Decides if CMake should prefer static libraries to dynamic libraries when both exist." )
  MARK_AS_ADVANCED(PREFER_STATIC_LIBRARIES)
ENDIF( NOT DEFINED PREFER_STATIC_LIBRARIES )

IF( PREFER_STATIC_LIBRARIES )
  SET( CMAKE_FIND_LIBRARY_SUFFIXES .a;${CMAKE_FIND_LIBRARY_SUFFIXES} )  
ENDIF( PREFER_STATIC_LIBRARIES )

SET( H3DUTIL_MAJOR_VERSION 1 )
SET( H3DUTIL_MINOR_VERSION 3 )
SET( H3DUTIL_BUILD_VERSION 1 )

SET( H3DUTIL_FULL_VERSION
${H3DUTIL_MAJOR_VERSION}.${H3DUTIL_MINOR_VERSION}.${H3DUTIL_BUILD_VERSION} ) 

# Add all sources, they are added to a variable called H3DUTIL_SRCS defined
# in the included file. All header files are added to a variable called
# H3DUTIL_HEADERS.
INCLUDE( ${H3DUtil_SOURCE_DIR}/H3DUtilSourceFiles.txt )

# add the H3DUtil.rc resource file if Visual Studio
IF(MSVC)
  SET( H3DUTIL_SRCS ${H3DUTIL_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc )
ENDIF(MSVC)

# add all optional libraries to this variable
SET(optionalLibs)

# add all required libraries to this variable
SET(requiredLibs)

SET(CMAKE_MODULE_PATH ${H3DUtil_SOURCE_DIR}/modules )

SET( ENABLE_PROFILER "OFF" CACHE BOOL "switcher to control the profiler")
IF( ENABLE_PROFILER )
  FIND_PACKAGE(SofaHelper)
  IF(SOFAHELPER_FOUND )
    SET(HAVE_PROFILER 1)
    INCLUDE_DIRECTORIES(${SOFAHELPER_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${SOFAHELPER_LIBRARIES} )
  ELSEIF( SOFAHELPER_FOUND )
    message(WARNING "No sofa helper library is found, profiler can not be enabled")
  ENDIF(SOFAHELPER_FOUND)
ENDIF( ENABLE_PROFILER )

# Optional extra library. Needed to support memory leak detector
SET( ENABLE_MEMORY_LEAK_DETECTOR "OFF" CACHE BOOL "enable viusal leak detector")
IF( ENABLE_MEMORY_LEAK_DETECTOR )
    FIND_PACKAGE(VLD)
    IF(VLD_FOUND)
        SET(HAVE_LIBVLD 1)
        INCLUDE_DIRECTORIES( ${VLD_INCLUDE_DIR} )
        SET(optionalLibs ${optionalLibs} ${VLD_LIBRARIES})
    ELSEIF (VLD_FOUND)
        MESSAGE(WARNING "NO visual leak detector found, memory leak detection can not be enabled")
    ENDIF(VLD_FOUND)
ENDIF( ENABLE_MEMORY_LEAK_DETECTOR )

FIND_PACKAGE(FreeImage)
IF(FREEIMAGE_FOUND)
  SET(HAVE_FREEIMAGE 1)
  INCLUDE_DIRECTORIES( ${FREEIMAGE_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${FREEIMAGE_LIBRARIES} )
ENDIF(FREEIMAGE_FOUND)

FIND_PACKAGE(H3DZLIB)
IF(ZLIB_FOUND)
  SET(HAVE_ZLIB 1)
  INCLUDE_DIRECTORIES( ${ZLIB_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${ZLIB_LIBRARIES} )
ENDIF(ZLIB_FOUND)

FIND_PACKAGE(DCMTK)
IF(DCMTK_FOUND)
  SET( HAVE_DCMTK 1 )
  INCLUDE_DIRECTORIES( ${DCMTK_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${DCMTK_LIBRARIES} )
ENDIF(DCMTK_FOUND)

FIND_PACKAGE(H3DTeem)
IF(Teem_FOUND)
  SET( HAVE_TEEM 1 )
  INCLUDE_DIRECTORIES( ${Teem_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${Teem_LIBRARIES} )
ENDIF(Teem_FOUND)

# Optional extra library. Used to handle OpenEXR HDR image formats.
FIND_PACKAGE(OpenEXR)
IF(OpenEXR_FOUND)
  SET(HAVE_OPENEXR 1)
  INCLUDE_DIRECTORIES( ${OpenEXR_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${OpenEXR_LIBRARIES} )
ENDIF(OpenEXR_FOUND)

FIND_PACKAGE(PTHREAD REQUIRED)
IF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${PTHREAD_LIBRARIES} )
ENDIF(PTHREAD_FOUND)

# H3DUtil shared library definition.
ADD_LIBRARY(H3DUtil SHARED ${H3DUTIL_SRCS} ${H3DUTIL_HEADERS})

# make sure that the H3DUtil.rc contains the correct svn-version
IF( MSVC )
  SET( H3DUTIL_SVN_VERSION "0" )
  # Find SubWCRev.exe
  FIND_FILE( SubWCRev
             NAMES "SubWCRev.exe"
             DOC   "Set to SubWCRev.exe that comes with TortoiseSVN. Used to find svn revision number." )
  MARK_AS_ADVANCED( SubWCRev )

  SET( H3DUtil_is_working_copy 10 )
  IF( SubWCRev )
    EXECUTE_PROCESS( COMMAND ${SubWCRev} ${H3DUtil_SOURCE_DIR}/../
                     RESULT_VARIABLE H3DUtil_is_working_copy )
    IF( ${H3DUtil_is_working_copy} EQUAL 0 )
      SET( H3DUTIL_SVN_VERSION "$WCREV$" )
    ENDIF( ${H3DUtil_is_working_copy} EQUAL 0 )
  ENDIF( SubWCRev )

  # autogenerate H3DUtil.rc depending on the version
  CONFIGURE_FILE( H3DUtil.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc )
  
  ADD_CUSTOM_COMMAND( TARGET H3DUtil
                      PRE_BUILD
                      COMMAND ${H3DUtil_SOURCE_DIR}/UpdateResourceFile 
                      ARGS H3DUTIL ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc ${H3DUtil_SOURCE_DIR}/H3DUtil.rc.cmake
                      ${H3DUTIL_MAJOR_VERSION} ${H3DUTIL_MINOR_VERSION}
                      ${H3DUTIL_BUILD_VERSION} "${H3DUTIL_SVN_VERSION}"
                      "H3DUTIL_Output_Name"
                      "$(TargetFileName)" )

  IF( SubWCRev AND ${H3DUtil_is_working_copy} EQUAL 0 )
    # Update SVN revision in file.
    EXECUTE_PROCESS( COMMAND ${SubWCRev} ${H3DUtil_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc )

    ADD_CUSTOM_COMMAND( TARGET H3DUtil 
                        PRE_BUILD 
                        COMMAND ${SubWCRev} 
                        ARGS ${H3DUtil_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DUtil.rc )
  ENDIF( SubWCRev AND ${H3DUtil_is_working_copy} EQUAL 0 )
ENDIF(MSVC)

# check all available libraries

SET( DEFAULT_BIN_INSTALL "bin" )
SET( DEFAULT_LIB_INSTALL "lib" )

IF( WIN32 )
  SET( DEFAULT_BIN_INSTALL "bin32" )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_BIN_INSTALL "bin64" )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  SET(requiredLibs ${requiredLibs} winmm.lib )
ENDIF(WIN32)

# add the libraries needed for linking
TARGET_LINK_LIBRARIES( H3DUtil ${requiredLibs} ${optionalLibs} )

# make sure symbols are exported.
SET( H3DUTIL_COMPILE_FLAGS "-DH3DUTIL_EXPORTS" )

SET( ENABLE_THREAD_LOCK_DEBUG "OFF" CACHE BOOL "switcher to control the thread lock debug collection" )
IF( ENABLE_THREAD_LOCK_DEBUG )
  IF( COMMAND cmake_policy )
    IF( POLICY CMP0054 )
      cmake_policy(SET CMP0054 OLD)
    ENDIF( POLICY CMP0054 )
  ENDIF( COMMAND cmake_policy )
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
      add_definitions("-std=gnu++11")
    elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
      add_definitions("-std=gnu++0x")
    else ()
      message(FATAL_ERROR "Enabling ENABLE_THREAD_LOCK_DEBUG requires C++11 support. This compiler lacks such support.")
    endif ()
  elseif (${MSVC_VERSION} LESS 1600 )
    message(FATAL_ERROR "Enabling ENABLE_THREAD_LOCK_DEBUG requires C++11 support. This compiler lacks such support.")
  endif ()
  SET( THREAD_LOCK_DEBUG 1 )
ENDIF( ENABLE_THREAD_LOCK_DEBUG )

# make the name of debug libraries end in _d.
SET_TARGET_PROPERTIES( H3DUtil PROPERTIES DEBUG_POSTFIX "_d" )

# set the version of the library
SET_TARGET_PROPERTIES( H3DUtil PROPERTIES VERSION ${H3DUTIL_FULL_VERSION} )


IF(APPLE)
  SET_TARGET_PROPERTIES (H3DUtil
               PROPERTIES 
#       BUILD_WITH_INSTALL_RPATH 1
                         INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF(APPLE)

IF(UNIX)
  SET_TARGET_PROPERTIES( H3DUtil PROPERTIES OUTPUT_NAME h3dutil )
ENDIF(UNIX)

IF( MSVC )
  include( H3DCommonFunctions )
  # change the name depending on compiler to be able to tell them apart
  # since they are not compatible with each other. 
  setH3DMSVCOutputName( H3DUtil "H3DUtil" )
  # Add common H3D compile flags for microsoft visual studio
  addCommonH3DMSVCCompileFlags( H3DUTIL_COMPILE_FLAGS )
  
  # H3DUtil can not link incrementally on vc8 for some reason. We shut of incremental linking for
  # all visual studio versions.
  SET_TARGET_PROPERTIES( H3DUtil PROPERTIES LINK_FLAGS_DEBUG " /NODEFAULTLIB:msvcrt /INCREMENTAL:NO" )

ELSEIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  IF(NOT MINGW)
    #used to link in dynamic library load functions. Needed by some examples in HAPI
    SET_TARGET_PROPERTIES( H3DUtil PROPERTIES LINK_FLAGS "-ldl" )
  ENDIF(NOT MINGW)
ENDIF( MSVC )

# set compile flags.
SET_TARGET_PROPERTIES( H3DUtil PROPERTIES COMPILE_FLAGS "${H3DUTIL_COMPILE_FLAGS}"  )

# autogenerate H3DUtil.h depending on the libraries available.
IF( EXISTS ${H3DUtil_SOURCE_DIR}/../include/H3DUtil/H3DUtil.h )
  FILE( REMOVE ${H3DUtil_SOURCE_DIR}/../include/H3DUtil/H3DUtil.h )
ENDIF( EXISTS ${H3DUtil_SOURCE_DIR}/../include/H3DUtil/H3DUtil.h )
CONFIGURE_FILE( ${H3DUtil_SOURCE_DIR}/../include/H3DUtil/H3DUtil.cmake ${CMAKE_CURRENT_BINARY_DIR}/include/H3DUtil/H3DUtil.h )

IF( NOT GENERATE_CPACK_PROJECT )
  INSTALL( FILES ${CMAKE_CURRENT_BINARY_DIR}/include/H3DUtil/H3DUtil.h
           DESTINATION ${H3DUtil_SOURCE_DIR}/../include/H3DUtil )
ENDIF( NOT GENERATE_CPACK_PROJECT )

INSTALL( TARGETS H3DUtil 
         LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL} COMPONENT H3DUtil_cpack_runtime
         RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL} COMPONENT H3DUtil_cpack_runtime
         ARCHIVE DESTINATION ${DEFAULT_LIB_INSTALL} COMPONENT H3DUtil_cpack_libraries )

IF( NOT ( WIN32 OR GENERATE_CPACK_PROJECT ) )
  # Install header files on non-windows system (e.g. Unix).
  INSTALL( FILES ${H3DUTIL_HEADERS}
           DESTINATION include/H3DUtil )
ENDIF( NOT ( WIN32 OR GENERATE_CPACK_PROJECT ) )

IF( NOT HAPI_SOURCE_DIR )
  include( ${H3DUtil_SOURCE_DIR}/H3DUtilCPack.cmake )
ENDIF( NOT HAPI_SOURCE_DIR )
