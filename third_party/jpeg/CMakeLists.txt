set(JPEG_SOURCE jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c
  jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c 
  jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdatadst.c jdatasrc.c
  jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c
  jdmaster.c jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c
  jfdctflt.c jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c 
  jquant1.c jquant2.c jutils.c jmemmgr.c jmemnobs.c jcarith.c jdarith.c jaricom.c)

add_library(jpeg STATIC ${JPEG_SOURCE})
target_include_directories(jpeg PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

include(CheckTypeSize)
check_type_size(size_t SIZEOF_SIZE_T)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/jconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/jconfig.h)

mark_as_third_party(jpeg ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})