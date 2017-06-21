cmake_minimum_required(VERSION 3.7)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

if(POLICY CMP0022) 
	cmake_policy(SET CMP0022 OLD)
endif() 

if (POLICY CMP0026)
	cmake_policy(SET CMP0026 OLD)
endif()  
 
if (COMMAND cmake_policy)
	cmake_policy (SET CMP0003 NEW)
endif()

if(POLICY CMP0042)
    cmake_policy(SET CMP0042 OLD)
endif()

project(Skuld C CXX)

set(SKULD_VERSION "1.0")

option(SKULD_WITH_SIMD "With SIMD support" off)
option(SKULD_BUILD_TESTS "Build tests" off)
option(SKULD_BUILD_TOOLS "Build tools" off)
option(SKULD_BUILD_EXTRA "Build extra module" on)
option(SKULD_ENABLE_MSIL "Enable Mono" on)
option(SKULD_ENABLE_LUA "Enable Lua" on)
option(SKULD_ENABLE_JAVASCRIPT "Enable JavaScript" on)
option(SKULD_ENABLE_PYTHON "Enable Python" on)

include(cmake/Skuld.cmake)
include(cmake/BinaryToHeader.cmake) 
include(cmake/MergeStaticLibs.cmake) 
add_definitions(-DBOOST_ALL_NO_LIB)

set(SKULD_INTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/internal_include)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/cairo_license_mpl.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/cairo_license_mpl.h cairo_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/cpufeature_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/cpufeature_license.h cpufeature_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/DDSTextureLoader_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/DDSTextureLoader_license.h DDSTextureLoader_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/etc1_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/etc1_license.h etc1_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/freetype_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/freetype_license.h freetype_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/libpng_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/libpng_license.h libpng_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/lua_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/lua_license.h lua_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/mono_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/mono_license.h mono_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/pixman_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/pixman_license.h pixman_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/tinyxml2_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/tinyxml2_license.h tinyxml2_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/zlib_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/zlib_license.h zlib_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/liblzma_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/liblzma_license.h liblzma_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/harfbuzz_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/harfbuzz_license.h harfbuzz_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/boost_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/boost_license.h boost_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/jsoncpp_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/jsoncpp_license.h jsoncpp_license)
binary_to_header(${CMAKE_CURRENT_SOURCE_DIR}/licenses/jpeg_license.txt 
	${SKULD_INTERNAL_INCLUDE_DIR}/License/jpeg_license.h jpeg_license)

add_subdirectory(third_party)
add_subdirectory(src)