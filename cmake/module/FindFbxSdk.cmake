#.rst
# FindFbxSdk
# ------------
#
# Created by Walter Gray.
# Locate and configure FbxSdk
#
# Interface Targets
# ^^^^^^^^^^^^^^^^^
#   FbxSdk::FbxSdk
#
# Variables
# ^^^^^^^^^
#   FbxSdk_ROOT_DIR
#   FbxSdk_FOUND
#   FbxSdk_INCLUDE_DIR
#   FbxSdk_LIBRARIES

set(FBX_SEARCH_PATHS
	${EXTERNAL_LIBRARY_DIR}
	"$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2017.0.1/"
	"$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2017.0.1/"
	"$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2017.0/"
	"$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2017.0/"
	"$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2016.1.2/"
	"$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2016.1.2/"
	"$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2016.1.1/"
	"$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2016.1.1/"
	"$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2015.1/"
	"$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2015.1/"
	"$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2014.2/"
	"$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2014.2/"
	"/Applications/Autodesk/FBX SDK/2017.0.1"
	"/Applications/Autodesk/FBX SDK/2017.0"
	"/Applications/Autodesk/FBX SDK/2016.1.2"
	"/Applications/Autodesk/FBX SDK/2016.1.1"
	"/Applications/Autodesk/FBX/FBX SDK/2015.1"
	"/Applications/Autodesk/FBX/FBX SDK/2014.2"
	"/usr"
	)

find_path(FbxSdk_ROOT_DIR
          NAMES include/fbxsdk.h
          HINTS ${FBX_SEARCH_PATHS})

set(FbxSdk_INCLUDE_DIR "${FbxSdk_ROOT_DIR}/include")

if (CMAKE_SIZEOF_VOID_P EQUAL 8) # 64bit
  set(BUILD_BIT_TYPE "x64")
else() # 32bit
  set(BUILD_BIT_TYPE "x86")
endif()

if(MSVC)
	IF(MSVC80)
	    SET(FBX_VS_VER "vs2005")
	ELSEIF(MSVC90)
	    SET(FBX_VS_VER "vs2008")
	ELSEIF(MSVC10)
	    SET(FBX_VS_VER "vs2010")
	ELSEIF(MSVC11)
	    SET(FBX_VS_VER "vs2012")
	ELSEIF(MSVC12 OR MSVC_VERSION==1800)
	    SET(FBX_VS_VER "vs2013")
	ELSEIF(MSVC14 OR MSVC_VERSION>1900)
	    SET(FBX_VS_VER "vs2015")
	ENDIF()
  find_library(FbxSdk_LIBRARY_RELEASE "libfbxsdk-md.lib" HINTS "${FbxSdk_ROOT_DIR}/lib/${FBX_VS_VER}/${BUILD_BIT_TYPE}/release")
  find_library(FbxSdk_LIBRARY_DEBUG "libfbxsdk-md.lib" HINTS "${FbxSdk_ROOT_DIR}/lib/${FBX_VS_VER}/${BUILD_BIT_TYPE}/debug")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # This is the correct way to detect Linux operating system -- see http://www.openguru.com/2009/04/cmake-detecting-platformoperating.html
  find_library(FbxSdk_LIBRARY_RELEASE "libfbxsdk.a" HINTS "${FbxSdk_ROOT_DIR}/lib/gcc4/${BUILD_BIT_TYPE}/release")
  find_library(FbxSdk_LIBRARY_DEBUG "libfbxsdk.a" HINTS "${FbxSdk_ROOT_DIR}/lib/gcc4/${BUILD_BIT_TYPE}/debug")
else()
  find_library(FbxSdk_LIBRARY_RELEASE "libfbxsdk.a" HINTS "${FbxSdk_ROOT_DIR}/lib/clang/release")
  find_library(FbxSdk_LIBRARY_DEBUG "libfbxsdk.a" HINTS "${FbxSdk_ROOT_DIR}/lib/clang/debug")
endif()

if (FbxSdk_FOUND)
	message("-- Found fbxsdk")
	message("-- FBX debug library : ${FbxSdk_LIBRARY_DEBUG}")
	message("-- FBX release library : ${FbxSdk_LIBRARY_RELEASE}")
	message("-- FBX include path : ${FbxSdk_INCLUDE_DIR}")
endif()

include(SelectConfigurations)
select_configurations(FbxSdk LIBRARY LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FbxSdk DEFAULT_MSG FbxSdk_INCLUDE_DIR FbxSdk_LIBRARIES)

include(CreateImportTargetHelpers)
generate_import_target(FbxSdk STATIC)
