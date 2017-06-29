if (SKULD_WIN32)

add_plugin(Audio XAudio2 
	XAudio2Sound.cpp
	XAudio2Device.cpp
	XAudio2Factory.cpp
	XAudio2Music.cpp)

endif()