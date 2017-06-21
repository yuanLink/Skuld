
if(SKULD_WIN32)
	add_library(DDSTextureLoader STATIC DDSTextureLoader.cpp)
	mark_as_third_party(DDSTextureLoader)
endif()