file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config")

if (UNIX)
  check_include_file("unistd.h" HAVE_UNISTD_H)
  check_include_file("fcntl.h" HAVE_FCNTL_H)
  check_include_file("stdint.h" HAVE_STDINT_H)

  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/builds/unix/ftconfig.in"
    FTCONFIG_H)
  if (HAVE_UNISTD_H)
    string(REGEX REPLACE
      "#undef +(HAVE_UNISTD_H)" "#define \\1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_FCNTL_H)
    string(REGEX REPLACE
      "#undef +(HAVE_FCNTL_H)" "#define \\1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_STDINT_H)
    string(REGEX REPLACE
      "#undef +(HAVE_STDINT_H)" "#define \\1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  string(REPLACE "/undef " "#undef "
    FTCONFIG_H "${FTCONFIG_H}")
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
    "${FTCONFIG_H}")
else ()
  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/freetype/config/ftconfig.h"
    FTCONFIG_H)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
    "${FTCONFIG_H}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftconfig.h")


file(READ "${CMAKE_CURRENT_SOURCE_DIR}/include/freetype/config/ftoption.h"
  FTOPTION_H)
string(REGEX REPLACE
  "/\\* +(#define +FT_CONFIG_OPTION_SYSTEM_ZLIB) +\\*/" "\\1"
  FTOPTION_H "${FTOPTION_H}")
string(REGEX REPLACE
  "/\\* +(#define +FT_CONFIG_OPTION_USE_PNG) +\\*/" "\\1"
  FTOPTION_H "${FTOPTION_H}")

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftoption.h-new"
  "${FTOPTION_H}")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftoption.h-new"
  "${CMAKE_CURRENT_BINARY_DIR}/include/freetype/config/ftoption.h")


# Specify library include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/include")


file(GLOB PUBLIC_HEADERS "include/ft2build.h" "include/freetype/*.h")
file(GLOB PUBLIC_CONFIG_HEADERS "include/freetype/config/*.h")
file(GLOB PRIVATE_HEADERS "include/freetype/internal/*.h")


set(BASE_SRCS
  src/autofit/autofit.c
  src/base/ftbase.c
  src/base/ftbbox.c
  src/base/ftbdf.c
  src/base/ftbitmap.c
  src/base/ftcid.c
  src/base/ftfntfmt.c
  src/base/ftfstype.c
  src/base/ftgasp.c
  src/base/ftglyph.c
  src/base/ftgxval.c
  src/base/ftinit.c
  src/base/ftlcdfil.c
  src/base/ftmm.c
  src/base/ftotval.c
  src/base/ftpatent.c
  src/base/ftpfr.c
  src/base/ftstroke.c
  src/base/ftsynth.c
  src/base/ftsystem.c
  src/base/fttype1.c
  src/base/ftwinfnt.c
  src/bdf/bdf.c
  src/bzip2/ftbzip2.c
  src/cache/ftcache.c
  src/cff/cff.c
  src/cid/type1cid.c
  src/gzip/ftgzip.c
  src/lzw/ftlzw.c
  src/pcf/pcf.c
  src/pfr/pfr.c
  src/psaux/psaux.c
  src/pshinter/pshinter.c
  src/psnames/psnames.c
  src/raster/raster.c
  src/sfnt/sfnt.c
  src/smooth/smooth.c
  src/truetype/truetype.c
  src/type1/type1.c
  src/type42/type42.c
  src/winfonts/winfnt.c
)

if (WIN32)
  set(BASE_SRCS ${BASE_SRCS} builds/windows/ftdebug.c)
else ()
  set(BASE_SRCS ${BASE_SRCS} src/base/ftdebug.c)
endif ()

add_library(freetype STATIC
  ${PUBLIC_HEADERS}
  ${PUBLIC_CONFIG_HEADERS}
  ${PRIVATE_HEADERS}
  ${BASE_SRCS}
)
target_compile_definitions(freetype PRIVATE FT2_BUILD_LIBRARY)

mark_as_third_party(freetype ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_target_dependencies(freetype zlib png)