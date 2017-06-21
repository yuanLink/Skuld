if(ENGINENAME_ANDROID)
	add_library(cpufeatures OBJECT
		${CMAKE_ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
	)
	mark_as_third_party(cpufeatures)
endif()