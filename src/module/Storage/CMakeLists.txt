add_module(Storage
	${SKULD_INCLUDE_DIR}/Skuld/Storage/Database.h
	${SKULD_INCLUDE_DIR}/Skuld/Storage/libMesh.h
	${SKULD_INCLUDE_DIR}/Skuld/Storage/libAssets.h
	 
	${CMAKE_CURRENT_SOURCE_DIR}/SQLite.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Assets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/libAssets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/libMesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Record.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Record.h
	${CMAKE_CURRENT_SOURCE_DIR}/AssetsInternal.h
)

add_target_dependencies(Storage boost sqlite3)