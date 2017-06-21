cmake_minimum_required(VERSION 2.8.0)

## HarfBuzz build configurations
set(HB_BUILTIN_UCDN ON)
set(HB_HAVE_FREETYPE ON)

include_directories(AFTER
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR}/src
  )

add_definitions(-DHAVE_OT)

if (MSVC)
  add_definitions(-wd4244 -wd4267 -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
endif ()
##

## Extract variables from Makefile files
# http://stackoverflow.com/a/27630120/1414809
function (prepend var prefix)
  set(listVar "")
  foreach (f ${ARGN})
    list(APPEND listVar "${prefix}${f}")
  endforeach ()
  set(${var} "${listVar}" PARENT_SCOPE)
endfunction ()

function (extract_make_variable variable file prefix)
  string(REGEX MATCH "${variable} = ([^$]+)\\$" temp ${file})
  string(REGEX MATCHALL "[^ \n\t\\]+" list ${CMAKE_MATCH_1})
  prepend(list ${prefix} ${list})
  set(${variable} ${list} PARENT_SCOPE)
endfunction ()

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/src/Makefile.sources SRCSOURCES)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/util/Makefile.sources UTILSOURCES)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/src/hb-ucdn/Makefile.sources UCDNSOURCES)

extract_make_variable(HB_BASE_sources ${SRCSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/")
extract_make_variable(HB_BASE_headers ${SRCSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/")
extract_make_variable(HB_OT_sources ${SRCSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/")
extract_make_variable(HB_OT_headers ${SRCSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/")

set(RAGEL_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
extract_make_variable(HB_BASE_RAGEL_GENERATED_sources ${SRCSOURCES} ${RAGEL_GENERATED_DIR})
extract_make_variable(HB_OT_RAGEL_GENERATED_sources ${SRCSOURCES} ${RAGEL_GENERATED_DIR})

extract_make_variable(HB_VIEW_sources ${UTILSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/util/")
extract_make_variable(HB_SHAPE_sources ${UTILSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/util/")
extract_make_variable(HB_OT_SHAPE_CLOSURE_sources ${UTILSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/util/")

extract_make_variable(LIBHB_UCDN_sources ${UCDNSOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/hb-ucdn/")

file(READ configure.ac CONFIGUREAC)
string(REGEX MATCH "\\[(([0-9]+)\\.([0-9]+)\\.([0-9]+))\\]" HB_VERSION_MATCH ${CONFIGUREAC})
set(HB_VERSION ${CMAKE_MATCH_1})
set(HB_VERSION_MAJOR ${CMAKE_MATCH_2})
set(HB_VERSION_MINOR ${CMAKE_MATCH_3})
set(HB_VERSION_MICRO ${CMAKE_MATCH_4})
set(HB_VERSION_STRING "${HB_VERSION_MAJOR}.${HB_VERSION_MINOR}.${HB_VERSION_MICRO}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/hb-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/hb-version.h)
##

## Define sources and headers of the project
set(project_sources
  ${HB_BASE_sources}
  ${HB_BASE_RAGEL_GENERATED_sources}

  ${HB_OT_sources}
  ${HB_OT_RAGEL_GENERATED_sources}
  )

set(project_headers
  ${HB_VERSION_H}

  ${HB_BASE_headers}
  ${HB_OT_headers}
  )

if (HB_HAVE_FREETYPE)
  add_definitions(-DHAVE_FREETYPE=1 -DHAVE_FT_FACE_GETCHARVARIANTINDEX=1)

  list(APPEND project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/hb-ft.cc)
  list(APPEND project_headers ${CMAKE_CURRENT_SOURCE_DIR}/src/hb-ft.h)
endif ()

if (HB_BUILTIN_UCDN)
  include_directories(src/hb-ucdn)
  add_definitions(-DHAVE_UCDN)

  list(APPEND project_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hb-ucdn.cc
    ${LIBHB_UCDN_sources})
endif ()
##

## Atomic ops availability detection
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/try_compile_intel_atomic_primitives.c"
"		void memory_barrier (void) { __sync_synchronize (); }
		int atomic_add (int *i) { return __sync_fetch_and_add (i, 1); }
		int mutex_trylock (int *m) { return __sync_lock_test_and_set (m, 1); }
		void mutex_unlock (int *m) { __sync_lock_release (m); }
		int main () { return 0; }
")
try_compile(HB_HAVE_INTEL_ATOMIC_PRIMITIVES
  ${CMAKE_CURRENT_BINARY_DIR}/try_compile_intel_atomic_primitives
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/try_compile_intel_atomic_primitives.c)
if (HB_HAVE_INTEL_ATOMIC_PRIMITIVES)
  add_definitions(-DHAVE_INTEL_ATOMIC_PRIMITIVES)
endif ()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/try_compile_solaris_atomic_ops.c"
"		#include <atomic.h>
		/* This requires Solaris Studio 12.2 or newer: */
		#include <mbarrier.h>
		void memory_barrier (void) { __machine_rw_barrier (); }
		int atomic_add (volatile unsigned *i) { return atomic_add_int_nv (i, 1); }
		void *atomic_ptr_cmpxchg (volatile void **target, void *cmp, void *newval) { return atomic_cas_ptr (target, cmp, newval); }
		int main () { return 0; }
")
try_compile(HB_HAVE_SOLARIS_ATOMIC_OPS
  ${CMAKE_CURRENT_BINARY_DIR}/try_compile_solaris_atomic_ops
  SOURCES ${CMAKE_CURRENT_BINARY_DIR}/try_compile_solaris_atomic_ops.c)
if (HB_HAVE_SOLARIS_ATOMIC_OPS)
  add_definitions(-DHAVE_SOLARIS_ATOMIC_OPS)
endif ()
##

add_library(harfbuzz STATIC ${project_sources} ${project_headers})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
mark_as_third_party(harfbuzz ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR})
add_target_dependencies(harfbuzz freetype)