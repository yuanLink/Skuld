add_subdirectory(tinyxml2)
add_subdirectory(DDSTextureLoader)
add_subdirectory(cairo)
add_subdirectory(pixman)
add_subdirectory(harfbuzz)
add_subdirectory(freetype)
add_subdirectory(libpng)
add_subdirectory(zlib)
if (SKULD_ENABLE_MSIL)
	add_subdirectory(mono)
endif()
add_subdirectory(etc1)
if (SKULD_ENABLE_LUA)
	add_subdirectory(lua)
endif()
add_subdirectory(cpufeatures)
add_subdirectory(liblzma)
add_subdirectory(boost)
add_subdirectory(sqlite3)
if (SKULD_WIN32)
	add_subdirectory(GPUOpen)
endif()
add_subdirectory(jsoncpp)
add_subdirectory(libogg)
add_subdirectory(libvorbis)
add_subdirectory(jpeg)
add_subdirectory(SPIRV-Cross)