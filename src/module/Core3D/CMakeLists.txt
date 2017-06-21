add_module(Core3D
	${CMAKE_CURRENT_SOURCE_DIR}/Engine3D.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EffectSelector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Effect.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EffectImpl.h
	${CMAKE_CURRENT_SOURCE_DIR}/EffectSelector.h
	${CMAKE_CURRENT_SOURCE_DIR}/TextureManager.h
	${CMAKE_CURRENT_SOURCE_DIR}/TextureManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp

	${SKULD_INCLUDE_DIR}/Skuld/Core/Engine3D.h
	${SKULD_INCLUDE_DIR}/Skuld/Core/Material.h
	${SKULD_INCLUDE_DIR}/Skuld/Core/Effect.h
)

add_target_dependencies(Core3D jsoncpp boost)