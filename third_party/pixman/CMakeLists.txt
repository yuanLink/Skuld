set(pixman_SRC 
    pixman/pixman.c
    pixman/pixman-access.c
    pixman/pixman-access-accessors.c
    pixman/pixman-bits-image.c
    pixman/pixman-combine32.c
    pixman/pixman-combine-float.c
    pixman/pixman-conical-gradient.c
    pixman/pixman-edge.c
    pixman/pixman-edge-accessors.c
    pixman/pixman-fast-path.c
    pixman/pixman-filter.c
    pixman/pixman-general.c
    pixman/pixman-glyph.c
    pixman/pixman-gradient-walker.c
    pixman/pixman-image.c
    pixman/pixman-implementation.c
    pixman/pixman-linear-gradient.c
    pixman/pixman-matrix.c
    pixman/pixman-radial-gradient.c
    pixman/pixman-region16.c
    pixman/pixman-region32.c
    pixman/pixman-solid-fill.c
    pixman/pixman-timer.c
    pixman/pixman-trap.c
    pixman/pixman-utils.c
    pixman/pixman-arm.c
    pixman/pixman-x86.c
    pixman/pixman-ppc.c
    pixman/pixman-mips.c
    pixman/pixman-noop.c)

if (WITH_SIMD)
    if (RTC_ARCH_ARM OR RTC_ARCH_ARM64)
        list(APPEND pixman_SRC 
            pixman/pixman-arm-neon.c)
        set(USE_ARM_NEON 1)
    elseif(RTC_ARCH_X86 OR RTC_ARCH_X64)
        list(APPEND pixman_SRC 
            pixman/pixman-sse2.c)
        set(USE_SSE2 1)
    endif()
endif()

set(PACKAGE pixman)
set(HAVE_LIBPNG 1)

include_directories(${CMAKE_CURRENT_BINARY_DIR} pixman)
add_definitions(-DHAVE_CONFIG_H)
add_library(pixman STATIC ${pixman_SRC})

if (NOT SKULD_WIN32)
    set(HAVE_PTHREADS 1)
endif()
if (SKULD_ANDROID)
	target_include_directories(pixman PUBLIC ${CMAKE_ANDROID_NDK}/sources/android/cpufeatures)
endif()
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
mark_as_third_party(pixman ${CMAKE_CURRENT_SOURCE_DIR}/pixman ${CMAKE_CURRENT_BINARY_DIR})