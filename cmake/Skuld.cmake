#Export variables:
#
#	Platform:
#		SKULD_WIN32
#		SKULD_UNIX
#		SKULD_ANDROID
#		SKULD_IOS
#		SKULD_UWP
#		SKULD_MAC
#		SKULD_WINPHONE
#
#	Compiler:
#		SKULD_GCC_LIKE
#
#	Architectures:
#		RTC_ARCH_X64
#		RTC_ARCH_X86
#		RTC_ARCH_ARM
#		RTC_ARCH_ARM64
#
#	Others:
#		SKULD_HOST
#		SKULD_WIN32ARM
#		SKULD_MULTICONFIG
#		SKULD_SDK_DIR
#
#Options:
#		SKULD_BUILD_TOOLS
#		SKULD_BUILD_TESTS
#		SKULD_WITH_SIMD
#		SKULD_ENABLE_MSIL
#		SKULD_ENABLE_LUA
#		SKULD_ENABLE_JAVASCRIPT
#		UNIX_BUILD_64
#		UNIX_BUILD_32
#
#Export Functions:
#		strip
#		add_unit_test
#		add_functional_test
#		add_plugin
#		add_module
#		third_party_include_directories
#		mark_as_third_party
#		add_engine_tool
#		mark_as_extra
#
#Global Properties:
#		_SKULD_EXTRA
#		_SKULD_THIRD_PARTY_NL
#		_SKULD_TOOL
#		_SKULD_THIRD_PARTY
#		_SKULD_INCLUDE_DIR
#		_SKULD_SRC


#################################################################################
############################           Init         #############################
#################################################################################

set(SKULD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(SKULD_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk)
set(SKULD_UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/module/")

include(CheckSymbolExists)
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(CheckIncludeFiles)

set(SKULD_WIN32 off)
set(SKULD_UNIX off)
set(SKULD_ANDROID off)
set(SKULD_IOS off)
set(SKULD_UWP off)
set(SKULD_MAC off)
set(SKULD_WINPHONE off)

set(SKULD_GCC_LIKE off)

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
	add_definitions(-Wimplicit-function-declaration)
	set(SKULD_GCC_LIKE on)
endif()

if (CMAKE_C_COMPILER_ID MATCHES "GNU")
	set(SKULD_GCC_LIKE on)
endif()

if (SKULD_GCC_LIKE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	add_definitions(-fvisibility=hidden -fPIC)
endif()


###########################################################################################################
################################            Check Target System             ###############################
###########################################################################################################

message("-- Target System: ${CMAKE_SYSTEM_NAME}")

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
	set(SKULD_ANDROID on)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(SKULD_WIN32 on)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(SKULD_MAC on)
else()
	set(SKULD_UNIX on)
endif()

if (SKULD_MAC)
	add_definitions(-D__MACOSX__)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework Cocoa")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
endif()

if (SKULD_WIN32 OR SKULD_MAC OR SKULD_UNIX)
	set(SKULD_HOST on)
else()
	set(SKULD_HOST off)
endif()

if (NOT SKULD_HOST)
	set(SKULD_BUILD_TOOLS off)
endif()

if (SKULD_GCC_LIKE)
	if(NOT SKULD_ANDROID)
		set(CMAKE_STRIP strip)
	else()
		set(CMAKE_STRIP ${CMAKE_CXX_ANDROID_TOOLCHAIN_PREFIX}strip${CMAKE_CXX_ANDROID_TOOLCHAIN_SUFFIX})
	endif()
endif()

if (SKULD_ANDROID)
	set(CMAKE_AR ${CMAKE_CXX_ANDROID_TOOLCHAIN_PREFIX}ar${CMAKE_CXX_ANDROID_TOOLCHAIN_SUFFIX})
endif()

###########################################################################################################
################################            Check Target Arch               ###############################
###########################################################################################################

if(SKULD_WIN32)
  check_symbol_exists("_M_AMD64" "" RTC_ARCH_X64)
  check_symbol_exists("_M_IX86" "" RTC_ARCH_X86)
  check_symbol_exists("_M_ARM" "" RTC_ARCH_ARM)
  check_symbol_exists("_M_ARM64" "" RTC_ARCH_ARM64)
elseif(SKULD_ANDROID)
  if (CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
    set(RTC_ARCH_X64 on)
  elseif (CMAKE_ANDROID_ARCH_ABI STREQUAL "x86")
	set(RTC_ARCH_X86 on)
  elseif (CMAKE_ANDROID_ARCH_ABI STREQUAL "armeabi-v7a")
	set(RTC_ARCH_ARM on)
  elseif (CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
	set(RTC_ARCH_ARM64 on)
  endif()
else()
  check_symbol_exists("__i386__" "" RTC_ARCH_X86)
  check_symbol_exists("__x86_64__" "" RTC_ARCH_X64)
  check_symbol_exists("__arm__" "" RTC_ARCH_ARM)
  check_symbol_exists("__aarch64__" "" RTC_ARCH_ARM64)
endif()

if (RTC_ARCH_X86)
	message("-- Build Arch: x86")
elseif (RTC_ARCH_X64)
	message("-- Build Arch: x64")
elseif (RTC_ARCH_ARM)
	message("-- Build Arch: arm")
elseif (RTC_ARCH_ARM64)
	message("-- Build Arch: arm64")
endif()

if (NOT RTC_ARCH_X86 AND NOT RTC_ARCH_X64 AND NOT RTC_ARCH_ARM AND NOT RTC_ARCH_ARM64)
	message(FATAL_ERROR "-- Error Arch")
endif()

if ((RTC_ARCH_ARM OR RTC_ARCH_ARM64) AND SKULD_WIN32)
	set(SKULD_WIN32ARM on)
else()
	set(SKULD_WIN32ARM off)
endif()

if (NOT SKULD_WIN32)
	ENABLE_LANGUAGE(ASM)
endif()

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS /EHsc)
	if (NOT RTC_ARCH_ARM AND NOT RTC_ARCH_ARM64)
		ENABLE_LANGUAGE(ASM_MASM)
	else()
		SET( CMAKE_C_STANDARD_LIBRARIES "shell32.lib gdi32.lib kernel32.lib user32.lib ole32.lib oleaut32.lib uuid.lib advapi32.lib" 
			CACHE STRING "Libraries linked by default with all C applications." FORCE )
		SET( CMAKE_CXX_STANDARD_LIBRARIES "shell32.lib gdi32.lib kernel32.lib user32.lib ole32.lib oleaut32.lib uuid.lib advapi32.lib"
			CACHE STRING "Libraries linked by default with all C++ applications." FORCE )
	endif()
	IF(RTC_ARCH_X86)
		SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
		SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
		SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
	ENDIF()
endif()

if (SKULD_UNIX)
	if (UNIX_BUILD_64)
		set(CMAKE_SIZEOF_VOID_P 8)
		set(CMAKE_CXX_FLAGS "-m64 ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-m64 ${CMAKE_C_FLAGS}")
		set(RTC_ARCH_X64 on)
		set(RTC_ARCH_X86 off)
	elseif(UNIX_BUILD_32)
		set(CMAKE_SYSTEM_PROCESSOR i386)
		set(CMAKE_SYSTEM_LIBRARY_PATH /lib32 /usr/lib32 /usr/local/lib32 /usr/lib/i386-linux-gnu)
		set(FIND_LIBRARY_USE_LIB64_PATHS OFF)
		set(CMAKE_IGNORE_PATH /lib /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu)

		set(CMAKE_SIZEOF_VOID_P 4)
		set(CMAKE_CXX_FLAGS "-msse2 -m32 ${CMAKE_CXX_FLAGS}")
		set(CMAKE_C_FLAGS "-msse2 -m32 ${CMAKE_C_FLAGS}")
		set(RTC_ARCH_X64 off)
		set(RTC_ARCH_X86 on)
	endif()
endif()

if("${CMAKE_CFG_INTDIR}" STREQUAL ".")
	set(SKULD_MULTICONFIG FALSE)
else()
	set(SKULD_MULTICONFIG TRUE)
endif()

check_include_file(inttypes.h  HAVE_INTTYPES_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)
check_include_file(unistd.h    Z_HAVE_UNISTD_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(assert.h    HAVE_ASSERT_H)
check_include_file(dlfcn.h     HAVE_DLFCN_H)
check_include_file(fcntl.h     HAVE_FCNTL_H)
check_include_file(dirent.h    HAVE_DIRENT_H)
check_include_file(wchar.h     HAVE_WCHAR_H)
check_include_file(errno.h     HAVE_ERRNO_H)
check_include_file(stdarg.h    HAVE_STDARG_PROTOTYPES)
check_include_file(io.h        HAVE_IO_H)
check_include_file(limits.h    HAVE_LIMITS_H)
check_include_file(malloc.h    HAVE_MALLOC_H)
check_include_file(search.h    HAVE_SEARCH_H)
check_include_file(strings.h   HAVE_STRINGS_H)
check_include_file(sys/time.h  HAVE_SYS_TIME_H)
check_include_file(sys/mman.h  HAVE_SYS_MMAN_H)
check_include_file(sys/stat.h  HAVE_SYS_STAT_H)
check_include_file(fenv.h 	   HAVE_FENV_H)
check_include_file(byteswap.h  HAVE_BYTESWAP_H)
check_include_file(libgen.h  HAVE_LIBGEN_H)
check_include_file(signal.h  HAVE_SIGNAL_H)
check_include_file(setjmp.h  HAVE_SETJMP_H)
check_include_file(sched.h  HAVE_SCHED_H)
check_include_file(sys/int_types.h  HAVE_SYS_INT_TYPES_H)
check_include_file(sys/ioctl.h  HAVE_SYS_IOCTL_H)
check_include_file(sys/poll.h  HAVE_SYS_POLL_H)
check_include_file(sys/socket.h  HAVE_SYS_SOCKET_H)
check_include_file(sys/un.h  HAVE_SYS_UN_H)
check_include_file(sys/wait.h  HAVE_SYS_WAIT_H)
check_include_file(time.h  HAVE_TIME_H)
check_include_files("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)
check_function_exists(getisax HAVE_GETISAX)
check_function_exists(getpagesize HAVE_GETPAGESIZE)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(mprotect HAVE_MPROTECT)
check_function_exists(posix_memalign HAVE_POSIX_MEMALIGN)
check_function_exists(sigaction HAVE_SIGACTION)
check_function_exists(mprotect HAVE_MPROTECT)
check_function_exists(clock_gettime HAVE_CLOCK_GETTIME)
check_function_exists(fseeko HAVE_FSEEKO)
check_function_exists(alarm HAVE_ALARM)
check_function_exists(__builtin_clz HAVE_BUILTIN_CLZ)
check_function_exists(feenableexcept HAVE_FEENABLEEXCEPT)
check_function_exists(__builtin_return_address HAVE_BUILTIN_RETURN_ADDRESS)
check_function_exists(drand48 HAVE_DRAND48)
check_function_exists(ctime_r HAVE_CTIME_R)
check_function_exists(flockfile HAVE_FLOCKFILE)
check_function_exists(funlockfile HAVE_FUNLOCKFILE)
check_function_exists(fork HAVE_FORK)
check_function_exists(link HAVE_LINK)
check_function_exists(getline HAVE_GETLINE)
check_function_exists(feclearexcept HAVE_FECLEAREXCEPT)
check_function_exists(fedisableexcept HAVE_FEDISABLEEXCEPT)
check_function_exists(poppler_page_render HAVE_POPPLER_PAGE_RENDER)
check_function_exists(raise HAVE_RAISE)
check_function_exists(strndup HAVE_STRNDUP)
check_function_exists(rsvg_pixbuf_from_file HAVE_RSVG_PIXBUF_FROM_FILE)
check_function_exists(sched_getaffinity HAVE_SCHED_GETAFFINITY)
check_type_size(__float128 HAVE_FLOAT128)
check_type_size(int SIZEOF_INT)
check_type_size(long SIZEOF_LONG)
check_type_size(ssize_t HAVE_SSIZE_T)
check_type_size(off64_t OFF64_T)
check_type_size(size_t SIZEOF_SIZE_T)
check_type_size(off_t SIZEOF_OFF_T)
check_type_size("void *" SIZEOF_VOID_P)
check_type_size(wchar_t SIZEOF_WCHAR_T)
check_type_size("uint64_t" HAVE_UINT64_T)
check_type_size("int64_t" HAVE_INT64_T) 
check_type_size("uint32_t" HAVE_UINT32_T)
check_type_size("long long" SIZEOF_LONG_LONG)
check_type_size("int32_t" HAVE_INT32_T)
check_type_size(short SIZEOF_SHORT)
check_type_size(uint128_t HAVE_UINT128_T)
check_symbol_exists(FE_DIVBYZERO "fenv.h" HAVE_FEDIVBYZERO)
set(HAVE_LONG_LONG ${SIZEOF_LONG_LONG})
set(PY_FORMAT_LONG_LONG "ll")
SET(HAVE_UNISTD_H ${Z_HAVE_UNISTD_H})

function(strip Target)
	if(NOT SKULD_MULTICONFIG)
		get_target_property(TARGET_LOCATION ${Target} LOCATION)
		add_custom_command(TARGET ${Target}
			POST_BUILD
			COMMAND ${CMAKE_STRIP} "${TARGET_LOCATION}")
	endif()
endfunction()

function(add_unit_test module_name)
	set(plugin_src ${ARGV})
	list(REMOVE_AT plugin_src 0)
	add_executable(${module_name}Test ${plugin_src})
	set_target_properties(${module_name}Test PROPERTIES FOLDER "Test")
	add_dependencies(${module_name}Test CoreStatic)
	target_link_libraries(${module_name}Test CoreStatic)
	target_include_directories(${module_name}Test PRIVATE ${SKULD_INCLUDE_DIR})
	if (SKULD_GCC_LIKE)
		set_target_properties(${module_name}Test PROPERTIES LINK_FLAGS "-Wl,--version-script=\"${SKULD_VERSION_SCRIPT_DIR}/export_test.map\"")
	endif()
	target_compile_definitions(${module_name}Test PRIVATE "SKULD_BUILD_DIR=\"${CMAKE_BINARY_DIR}\"")
	if (SKULD_MULTICONFIG)
		foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
			string(TOUPPER ${build_type} up_build_type)
			set_target_properties(${module_name}Test PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/test)
			set_target_properties(${module_name}Test PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/test)
			set_target_properties(${module_name}Test PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/test)
		endforeach()
	else()
		set_target_properties(${module_name}Test PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
		set_target_properties(${module_name}Test PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
		set_target_properties(${module_name}Test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
	endif()
	strip(${module_name}Test)
endfunction()

function(add_functional_test module_name)
	set(testname ${module_name}Test)
	set(plugin_src ${ARGV})
	list(REMOVE_AT plugin_src 0)
	add_executable(${testname} ${plugin_src})
	set_target_properties(${testname} PROPERTIES FOLDER "Test")
	add_dependencies(${testname} Core)
	target_link_libraries(${testname} Core)
	target_compile_definitions(${module_name}Test PRIVATE SKULD_USE_DYNAMIC)
	if (SKULD_GCC_LIKE)
		set_target_properties(${module_name}Test PROPERTIES LINK_FLAGS "-Wl,--version-script=\"${SKULD_VERSION_SCRIPT_DIR}/export_test.map\"")
	endif()
	target_compile_definitions(${module_name}Test PRIVATE "SKULD_BUILD_DIR=\"${CMAKE_BINARY_DIR}\"")
	if (SKULD_MULTICONFIG)
		foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
			string(TOUPPER ${build_type} up_build_type)
			set_target_properties(${testname} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/test)
			set_target_properties(${testname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/test)
			set_target_properties(${testname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/test)
		endforeach()
	else()
		set_target_properties(${testname} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
		set_target_properties(${testname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
		set_target_properties(${testname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
	endif()
	strip(${testname})
endfunction()

function(add_plugin plugin_type plugin_id)
	set(plugin_name ${plugin_type}_${plugin_id})
	set(plugin_src ${ARGV})
	list(REMOVE_AT plugin_src 0 1)
	list(APPEND plugin_src ${SKULD_SDK_DIR}/${plugin_type}.cpp)

	add_library(${plugin_name} SHARED ${plugin_src})
	add_dependencies(${plugin_name} CoreStatic)
	target_link_libraries(${plugin_name} CoreStatic)
	target_compile_definitions(${plugin_name} PRIVATE SKULD_BUILD_DYNAMIC SKULD_PLUGIN)
	target_include_directories(${plugin_name} PRIVATE ${SKULD_INCLUDE_DIR} ${SKULD_SDK_DIR})

	add_library(${plugin_name}Static STATIC ${plugin_src})
	target_compile_definitions(${plugin_name}Static PRIVATE SKULD_PLUGIN)
	target_include_directories(${plugin_name}Static PRIVATE ${SKULD_INCLUDE_DIR} ${SKULD_SDK_DIR})
	set_target_properties(${plugin_name} PROPERTIES FOLDER "Plugin/Shared")
	set_target_properties(${plugin_name}Static PROPERTIES FOLDER "Plugin/Static")
	
	if (SKULD_GCC_LIKE)
		set_target_properties(${plugin_name} PROPERTIES LINK_FLAGS "-Wl,--version-script=\"${SKULD_VERSION_SCRIPT_DIR}/export_plugin.map\"")
	endif()
	if (SKULD_MULTICONFIG)
		foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
			string(TOUPPER ${build_type} up_build_type)
			set_target_properties(${plugin_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/plugin)
			set_target_properties(${plugin_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/plugin)
			set_target_properties(${plugin_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/plugin)
			set_target_properties(${plugin_name}Static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/plugin)
			set_target_properties(${plugin_name}Static PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/plugin)
			set_target_properties(${plugin_name}Static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/plugin)
		endforeach()
	else()
		set_target_properties(${plugin_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/plugin)
		set_target_properties(${plugin_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/plugin)
		set_target_properties(${plugin_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/plugin)
		set_target_properties(${plugin_name}Static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/plugin)
		set_target_properties(${plugin_name}Static PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/plugin)
		set_target_properties(${plugin_name}Static PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/plugin)
	endif()
	strip(${plugin_name})
endfunction()

function(add_module target)
	set(__internal_src ${ARGV})
	list(REMOVE_AT __internal_src 0)
	get_property(__internal GLOBAL PROPERTY "_SKULD_SRC")
	list(APPEND __internal ${__internal_src})
	set_property(GLOBAL PROPERTY _SKULD_SRC ${__internal})
	add_library(${target} STATIC ${__internal_src})
	get_property(__internal GLOBAL PROPERTY "_SKULD_MODULE")
	list(APPEND __internal ${target})
	set_property(GLOBAL PROPERTY _SKULD_MODULE ${__internal})
	set_target_properties(${target} PROPERTIES FOLDER "Module")
	target_include_directories(${target} PRIVATE ${SKULD_INCLUDE_DIR})
endfunction()

function(mark_as_third_party target)
	set(incs ${ARGV})
	list(REMOVE_AT incs 0)
	set_property(GLOBAL PROPERTY _SKULD_include_of_${target} ${incs})

	get_property(__internal GLOBAL PROPERTY "_SKULD_THIRD_PARTY")
	list(APPEND __internal ${target})
	set_property(GLOBAL PROPERTY _SKULD_THIRD_PARTY ${__internal})
	set_target_properties(${target} PROPERTIES FOLDER "ThirdParty/Unused")
endfunction()

function(add_target_dependencies target)
	set(deps ${ARGV})
	list(REMOVE_AT deps 0)
	set_property(GLOBAL PROPERTY "_SKULD_depend_of_${target}" ${deps})
endfunction()

function(append_should_link_3rd target)
	get_property(__internal GLOBAL PROPERTY "_SKULD_THIRD_PARTY_SHOULD_LINK")
	list(FIND __internal ${target} temp)
	if (${temp} EQUAL -1)
		list(APPEND __internal ${target})
	endif()
	set_property(GLOBAL PROPERTY _SKULD_THIRD_PARTY_SHOULD_LINK ${__internal})
endfunction()

function(append_link_libraries target)
	set(deps ${ARGV})
	list(REMOVE_AT deps 0)
	get_target_property(__internal ${target} LINK_LIBRARIES)
	if (NOT __internal)
		set(__internal)
	endif()
	foreach(dep ${deps})
		list(FIND __internal ${dep} temp)
		if (${temp} EQUAL -1)
			list(APPEND __internal ${dep})
		endif()
	endforeach()
	set_property(TARGET ${target} PROPERTY LINK_LIBRARIES ${__internal})
endfunction()

function(append_should_not_link_3rd target)
	get_property(__internal GLOBAL PROPERTY "_SKULD_THIRD_PARTY_NO_LINK")
	get_property(__internal2 GLOBAL PROPERTY "_SKULD_THIRD_PARTY_SHOULD_LINK")
	list(FIND __internal ${target} temp)
	list(FIND __internal2 ${target} temp2)
	if ((${temp} EQUAL -1) AND (${temp2} EQUAL -1))
		list(APPEND __internal ${target})
	endif()
	set_property(GLOBAL PROPERTY _SKULD_THIRD_PARTY_NO_LINK ${__internal})
endfunction()

function(detect_dep_for_module ${target})
	list(GET ARGV 1 cur)
	get_property(deps GLOBAL PROPERTY _SKULD_depend_of_${cur})
	foreach(dep ${deps})
		append_should_link_3rd(${dep})
		get_property(inc GLOBAL PROPERTY "_SKULD_include_of_${dep}")
		target_include_directories(Core PRIVATE ${inc})
		target_include_directories(${target} PRIVATE ${inc})
		get_target_property(type ${target} TYPE)
		if (type STREQUAL "STATIC_LIBRARY" OR type STREQUAL "SHARED_LIBRARY")
			append_link_libraries(${target} ${dep})
		endif()
		detect_dep_for_module(${target} ${dep})
	endforeach()
endfunction()

function(detect_dep_for ${target})
	list(GET ARGV 1 cur)
	get_property(deps GLOBAL PROPERTY _SKULD_depend_of_${cur})
	foreach(dep ${deps})
		append_should_not_link_3rd(${dep})
		get_property(inc GLOBAL PROPERTY "_SKULD_include_of_${dep}")
		target_include_directories(${target} PRIVATE ${inc})
		get_target_property(type ${target} TYPE)
		if (type STREQUAL "STATIC_LIBRARY" OR type STREQUAL "SHARED_LIBRARY")
			append_link_libraries(${target} ${dep})
		endif()
		detect_dep_for(${target} ${dep})
	endforeach() 
endfunction()

function(detect_dep_for_3rd ${target})
	list(GET ARGV 1 cur)
	get_property(deps GLOBAL PROPERTY _SKULD_depend_of_${cur})
	foreach(dep ${deps})
		get_property(inc GLOBAL PROPERTY "_SKULD_include_of_${dep}")
		target_include_directories(${target} PRIVATE ${inc})
		detect_dep_for_3rd(${target} ${dep})
	endforeach()
endfunction()

function(detect_link_map)
	get_property(SKULD_MODULE GLOBAL PROPERTY "_SKULD_MODULE")
	get_property(SKULD_EXTRA GLOBAL PROPERTY "_SKULD_EXTRA")
	get_property(SKULD_TOOL GLOBAL PROPERTY "_SKULD_TOOL")
	get_property(SKULD_TEST GLOBAL PROPERTY "_SKULD_TEST")
	get_property(SKULD_PLUGIN GLOBAL PROPERTY "_SKULD_PLUGIN")
	get_property(SKULD_THIRD_PARTY_LIST GLOBAL PROPERTY "_SKULD_THIRD_PARTY")

	foreach(target ${SKULD_MODULE})
		detect_dep_for_module(${target} ${target})
	endforeach()
	foreach(target ${SKULD_EXTRA} ${SKULD_PLUGIN} ${SKULD_TOOL})
		detect_dep_for(${target} ${target})
	endforeach()
	foreach(target ${SKULD_THIRD_PARTY_LIST})
		detect_dep_for_3rd(${target} ${target})
	endforeach()
	get_property(SKULD_THIRD_PARTY GLOBAL PROPERTY _SKULD_THIRD_PARTY_SHOULD_LINK)
	get_property(SKULD_THIRD_PARTY_NL GLOBAL PROPERTY _SKULD_THIRD_PARTY_NO_LINK)

	foreach(lib ${SKULD_THIRD_PARTY})
		set_target_properties(${lib} PROPERTIES FOLDER "ThirdParty/Core")
	endforeach()

	foreach(lib ${SKULD_THIRD_PARTY_NL})
		set_target_properties(${lib} PROPERTIES FOLDER "ThirdParty/Extra")
		if (SKULD_MULTICONFIG)
			foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
				string(TOUPPER ${build_type} up_build_type)
				set_target_properties(${lib} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/lib/third_party)
				set_target_properties(${lib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin/third_party)
				set_target_properties(${lib} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin/third_party)
			endforeach()
		else()
			set_target_properties(${lib} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/third_party)
			set_target_properties(${lib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin/third_party)
			set_target_properties(${lib} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin/third_party)
		endif()
	endforeach()
	
	add_dependencies(Core ${SKULD_THIRD_PARTY})
	target_link_libraries(Core ${SKULD_THIRD_PARTY})
endfunction()

function(add_engine_tool target)
	set(src ${ARGV})
	list(REMOVE_AT src 0)
	add_executable(${target} ${src})

	get_property(__internal GLOBAL PROPERTY "_SKULD_TOOL")
	list(APPEND __internal ${target})
	set_property(GLOBAL PROPERTY _SKULD_TOOL ${__internal})
	set_target_properties(${target} PROPERTIES FOLDER "Tool")

	add_dependencies(${target} Core)
	target_link_libraries(${target} Core)

	add_target_dependencies(${target} boost)
	target_include_directories(${target} PRIVATE ${SKULD_INCLUDE_DIR})

	strip(${target})
	if (SKULD_MULTICONFIG)
		foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
			string(TOUPPER ${build_type} up_build_type)
			set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin)
			set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin)
			set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin)
		endforeach()
	else()
		set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
		set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
		set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
	endif()
endfunction()

function(mark_as_extra target)
	get_property(__internal GLOBAL PROPERTY "_SKULD_EXTRA")
	target_include_directories(${target} PRIVATE ${SKULD_INCLUDE_DIR})
	list(APPEND __internal ${target})
	set_property(GLOBAL PROPERTY _SKULD_EXTRA ${__internal})
	set_target_properties(${target} PROPERTIES FOLDER "Extra")
	
	get_target_property(type ${target} TYPE)
	if (type STREQUAL "SHARED_LIBRARY")
		target_compile_definitions(${target} PRIVATE SKULD_BUILD_DYNAMIC)
		target_link_libraries(${target} CoreStatic)
		add_dependencies(${target} CoreStatic)
	endif()
	target_compile_definitions(${target} PRIVATE SKULD_EXTRA)

	if (SKULD_MULTICONFIG)
		foreach(build_type ${CMAKE_CONFIGURATION_TYPES})
			string(TOUPPER ${build_type} up_build_type)
			set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin)
			set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/lib)
			set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${up_build_type} ${CMAKE_BINARY_DIR}/${build_type}/bin)
		endforeach()
	else()
		set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
		set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
		set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
	endif()
endfunction()

if (SKULD_WITH_SIMD)
	add_definitions(-DMATH_USE_SIMD)
	if (RTC_ARCH_X86 OR RTC_ARCH_X64)
		add_definitions(-DSIMD_SSE2)
	elseif (RTC_ARCH_ARM OR RTC_ARCH_ARM64)
		add_definitions(-DSIMD_NEON)
	endif()
endif()

if (SKULD_WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
endif()

if (NOT SKULD_WIN32)
	set(SKULD_VERSION_SCRIPT_DIR ${CMAKE_CURRENT_BINARY_DIR}/version_script)
	file(WRITE ${SKULD_VERSION_SCRIPT_DIR}/export.map "{
	global:
		extern \"C++\"
		{
			SKULD::*;
		};
	local: *;
};")
	file(WRITE ${SKULD_VERSION_SCRIPT_DIR}/export_test.map "{
	global: main;
	local: *;
};")
	file(WRITE ${SKULD_VERSION_SCRIPT_DIR}/export_plugin.map "{
	global: SKULD_GetPluginEntry;SKULD_GetPluginSig;
	local: *;
};")
endif()