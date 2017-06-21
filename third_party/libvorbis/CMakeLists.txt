add_library(vorbis STATIC 
		lib/window.c
		lib/vorbisenc.c
		lib/synthesis.c
		lib/smallft.c
		lib/sharedbook.c
		lib/res0.c
		lib/registry.c
		lib/psy.c
		lib/mdct.c
		lib/mapping0.c
		lib/lsp.c
		lib/lpc.c
		lib/lookup.c
		lib/info.c
		lib/floor1.c
		lib/floor0.c
		lib/envelope.c
		lib/codebook.c
		lib/block.c
		lib/bitrate.c
		lib/analysis.c
)

target_include_directories(vorbis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(vorbisfile STATIC lib/vorbisfile.c)

mark_as_third_party(vorbis ${CMAKE_CURRENT_SOURCE_DIR}/include)
mark_as_third_party(vorbisfile)

add_target_dependencies(vorbis ogg)
add_target_dependencies(vorbisfile vorbis)