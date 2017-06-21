if(HAVE_INTTYPES_H)
	add_definitions(-DHAVE_INTTYPES_H)
endif()

if(HAVE_STDINT_H)
	add_definitions(-DHAVE_STDINT_H)
endif()

if(HAVE_LIMITS_H)
	add_definitions(-DHAVE_LIMITS_H)
endif()

check_include_file(stdbool.h HAVE_STDBOOL__H)
if(HAVE_STDBOOL__H)
	add_definitions(-DHAVE_STDBOOL_H)
endif()

if(HAVE_STRING_H)
	add_definitions(-DHAVE_STRING_H)
endif()

if(HAVE_STRINGS_H)
	add_definitions(-DHAVE_STRINGS_H)
endif()

if(HAVE_MEMORY_H)
	add_definitions(-DHAVE_MEMORY_H)
endif()

check_type_size(bool HAVE_BOOL)
if (HAVE_BOOL)
	add_definitions(-DHAVE__BOOL)
endif()

add_definitions(-DHAVE_ENCODER_LZMA2 -DHAVE_ENCODER_LZMA1 -DHAVE_CHECK_CRC32
	-DHAVE_MF_BT3 -DHAVE_MF_BT2 -DHAVE_MF_BT4 -DHAVE_MF_HC3 -DHAVE_MF_HC4
	-DHAVE_DECODER_LZMA1 -DHAVE_DECODER_LZMA2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common ${CMAKE_CURRENT_SOURCE_DIR}/liblzma/common
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/rangecoder
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/api
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/lzma
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/check
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/simple
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/delta
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/lz)

add_library(liblzma STATIC 
	liblzma/check/check.c
	liblzma/check/crc32_fast.c
	liblzma/check/crc32_table.c
	liblzma/common/block_decoder.c
	liblzma/common/block_encoder.c
	liblzma/common/block_header_decoder.c
	liblzma/common/block_header_encoder.c
	liblzma/common/block_util.c
	liblzma/common/common.c
	liblzma/common/filter_common.c
	liblzma/common/filter_decoder.c
	liblzma/common/filter_encoder.c
	liblzma/common/filter_flags_decoder.c
	liblzma/common/filter_flags_encoder.c
	liblzma/common/easy_encoder.c
	liblzma/common/easy_preset.c
	liblzma/common/index.c
	liblzma/common/index_decoder.c
	liblzma/common/index_encoder.c
	liblzma/common/index_hash.c
	liblzma/common/stream_decoder.c
	liblzma/common/stream_encoder.c
	liblzma/common/stream_flags_common.c
	liblzma/common/stream_flags_decoder.c
	liblzma/common/stream_flags_encoder.c
	liblzma/common/vli_decoder.c
	liblzma/common/vli_encoder.c
	liblzma/common/vli_size.c
	liblzma/lz/lz_decoder.c
	liblzma/lz/lz_encoder.c
	liblzma/lz/lz_encoder_mf.c
	liblzma/lzma/fastpos_table.c
	liblzma/lzma/lzma_decoder.c
	liblzma/lzma/lzma_encoder.c
	liblzma/lzma/lzma_encoder_optimum_fast.c
	liblzma/lzma/lzma_encoder_optimum_normal.c
	liblzma/lzma/lzma_encoder_presets.c
	liblzma/lzma/lzma2_decoder.c
	liblzma/lzma/lzma2_encoder.c
	liblzma/rangecoder/price_table.c
)

mark_as_third_party(liblzma
	${CMAKE_CURRENT_SOURCE_DIR}/liblzma/api)