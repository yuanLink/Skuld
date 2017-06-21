add_module(Algorithm
	${SKULD_INCLUDE_DIR}/Skuld/Algorithm/Hash.h
	${SKULD_INCLUDE_DIR}/Skuld/Algorithm/CompressedStream.h
	${SKULD_INCLUDE_DIR}/Skuld/Algorithm/EncryptedStream.h
	 
	${CMAKE_CURRENT_SOURCE_DIR}/Common.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Common.h

	${CMAKE_CURRENT_SOURCE_DIR}/deflate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LZMA.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/AES.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DES.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/xor.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/CRC.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MD5.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SHA.cpp
)
add_target_dependencies(Algorithm liblzma zlib)