add_module(Support 
	${CMAKE_CURRENT_SOURCE_DIR}/FileStream.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/License.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Path.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Text.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/MemoryStream.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/LanguagePack.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Stream.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/MathLib.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/SIMDMath.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Bitmap.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Check.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Exception.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/GlobalConfig.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/IDisposable.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Plugin.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/PluginEntry.h 
	${CMAKE_CURRENT_SOURCE_DIR}/MathLib.h 
	${CMAKE_CURRENT_SOURCE_DIR}/SIMDMath.h 
	${CMAKE_CURRENT_SOURCE_DIR}/WriteCache.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ReadCache.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/String.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DynamicLibrary.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/IDisposable.cpp
	${SKULD_INCLUDE_DIR}/Skuld/IDisposable.h 
	${SKULD_INCLUDE_DIR}/Skuld/Ptr.hpp
	
	${SKULD_INCLUDE_DIR}/Skuld/Exception.h 
	${SKULD_INCLUDE_DIR}/Skuld/License.h 
	${SKULD_INCLUDE_DIR}/Skuld/Check.h 
	${SKULD_INCLUDE_DIR}/Skuld/GlobalConfig.h 
	${SKULD_INCLUDE_DIR}/Skuld/Plugin.h 
	${SKULD_INCLUDE_DIR}/Skuld/String.h 
	${SKULD_INCLUDE_DIR}/Skuld/Bitmap.h 
	${SKULD_INCLUDE_DIR}/Skuld/PixelFormat.h 
	${SKULD_INCLUDE_DIR}/Skuld/FileStream.h 
	${SKULD_INCLUDE_DIR}/Skuld/Path.h 
	${SKULD_INCLUDE_DIR}/Skuld/Text.h 
	${SKULD_INCLUDE_DIR}/Skuld/MemoryStream.h 
	${SKULD_INCLUDE_DIR}/Skuld/Stream.h 
	${SKULD_INCLUDE_DIR}/Skuld/MathStruct.h
	${SKULD_INCLUDE_DIR}/Skuld/LanguagePack.h
	${SKULD_INCLUDE_DIR}/Skuld/DynamicLibrary.h)
target_include_directories(Support PRIVATE ${SKULD_INTERNAL_INCLUDE_DIR})
add_target_dependencies(Support boost tinyxml2 jsoncpp)
