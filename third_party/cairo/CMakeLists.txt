set(_CAIRO_SRC 
    cairo.c 
    cairo-analysis-surface.c
    cairo-arc.c
    cairo-array.c 
    cairo-atomic.c 
    cairo-base64-stream.c
    cairo-bentley-ottmann.c
    cairo-bentley-ottmann-rectangular.c
    cairo-bentley-ottmann-rectilinear.c
    cairo-boxes.c
    cairo-boxes-intersect.c
    cairo-cache.c
    cairo-clip.c
    cairo-clip-boxes.c
    cairo-clip-polygon.c
    cairo-clip-region.c
    cairo-clip-surface.c
    cairo-clip-tor-scan-converter.c
    cairo-color.c
    cairo-composite-rectangles.c
    cairo-compositor.c
    cairo-contour.c
    cairo-damage.c
    cairo-debug.c
    cairo-default-context.c
    cairo-deflate-stream.c
    cairo-device.c
    cairo-error.c 
    cairo-font-face.c 
    cairo-font-face-twin.c
    cairo-font-face-twin-data.c
    cairo-font-options.c 
    cairo-ft-font.c
    cairo-freelist.c
    cairo-gstate.c
    cairo-hash.c
    cairo-hull.c
    cairo-image-compositor.c
    cairo-image-info.c
    cairo-image-source.c
    cairo-image-surface.c
    cairo-line.c
    cairo-mask-compositor.c
    cairo-matrix.c
    cairo-mesh-pattern-rasterizer.c
    cairo-misc.c
    cairo-mono-scan-converter.c
    cairo-mutex.c 
    cairo-no-compositor.c
    cairo-observer.c
    cairo-output-stream.c
    cairo-path.c 
    cairo-path-bounds.c
    cairo-path-fill.c
    cairo-path-fixed.c
    cairo-path-in-fill.c
    cairo-path-stroke.c
    cairo-path-stroke-boxes.c
    cairo-path-stroke-polygon.c
    cairo-path-stroke-traps.c
    cairo-path-stroke-tristrip.c
    cairo-pattern.c 
    cairo-pen.c
    cairo-png.c
    cairo-polygon.c
    cairo-polygon-intersect.c
    cairo-polygon-reduce.c
    cairo-raster-source-pattern.c
    cairo-recording-surface.c
    cairo-rectangle.c
    cairo-rectangular-scan-converter.c
    cairo-region.c
    cairo-scaled-font.c
    cairo-shape-mask-compositor.c
    cairo-slope.c
    cairo-spans.c
    cairo-spans-compositor.c
    cairo-spline.c
    cairo-stroke-dash.c
    cairo-stroke-style.c
    cairo-surface.c
    cairo-surface-offset.c
    cairo-surface-snapshot.c
    cairo-surface-wrapper.c
    cairo-tor-scan-converter.c
    cairo-tor22-scan-converter.c
    cairo-toy-font-face.c
    cairo-traps.c
    cairo-traps-compositor.c
    cairo-tristrip.c
    cairo-unicode.c
    cairo-user-font.c
    cairo-version.c
    cairo-wideint.c
    cairo-xml-surface.c)

set(CAIRO_SRC )
foreach(s ${_CAIRO_SRC})
    list(APPEND CAIRO_SRC "src/${s}")
endforeach()

set(CAIRO_HAS_PTHREAD 1)
set(CAIRO_HAS_REAL_PTHREAD 1)
set(HAS_PIXMAN_GLYPHS 1)
set(HAVE_FT_GET_X11_FONT_FORMAT 1)
set(HAVE_FT_GLYPHSLOT_EMBOLDEN 1)
set(HAVE_FT_GLYPHSLOT_OBLIQUE 1)
set(HAVE_FT_LIBRARY_SETLCDFILTER 1)
set(HAVE_FT_LOAD_SFNT_TABLE 1)
set(HAVE_ZLIB 1)

add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(cairo STATIC ${CAIRO_SRC})

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
mark_as_third_party(cairo ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
add_target_dependencies(cairo zlib png freetype pixman)