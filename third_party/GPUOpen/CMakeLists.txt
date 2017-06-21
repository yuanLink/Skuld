set(aofx_src amd_aofx/src/AMD_AOFX.cpp 
    amd_aofx/src/AMD_AOFX_DEBUG.cpp 
    amd_aofx/src/AMD_AOFX_OPAQUE.cpp
)
set(geometryfx_src amd_geometryfx/src/AMD_GeometryFX_Filtering.cpp
    amd_geometryfx/src/AMD_GeometryFX_Utility.cpp
    amd_geometryfx/src/GeometryFXMesh.cpp
    amd_geometryfx/src/GeometryFXMeshManager.cpp
    amd_geometryfx/src/GeometryFXUtility_Internal.cpp
)
set(shadowfx11_src amd_shadowfx/src/AMD_ShadowFX11.cpp
    amd_shadowfx/src/AMD_ShadowFX11_Opaque.cpp
)
set(shadowfx12_src amd_shadowfx/src/AMD_ShadowFX12.cpp
    amd_shadowfx/src/AMD_ShadowFX12_Opaque.cpp
)
set(tressfx_src amd_tressfx/src/TressFX.cpp
    amd_tressfx/src/TressFXAsset.cpp
    amd_tressfx/src/TressFXMesh.cpp
    amd_tressfx/src/TressFXOpaque.cpp
    amd_tressfx/src/TressFXRenderer.cpp
    amd_tressfx/src/TressFXShortCut.cpp
    amd_tressfx/src/TressFXSimulation.cpp
    amd_tressfx/src/Util.cpp
    amd_tressfx/src/Math/Matrix33.cpp
    amd_tressfx/src/Math/Matrix44.cpp
    amd_tressfx/src/Math/Quaternion.cpp
    amd_tressfx/src/Math/Transform.cpp
    amd_tressfx/src/Math/Vector3D.cpp
)
set(depthoffieldfx_src amd_depthoffieldfx/src/AMD_DepthOfFieldFX.cpp
    amd_depthoffieldfx/src/AMD_DepthOfFieldFX_Opaque.cpp
)
set(amdlib_src amd_lib/shared/d3d11/src/AMD_Buffer.cpp
    amd_lib/shared/d3d11/src/AMD_Common.cpp
    amd_lib/shared/d3d11/src/AMD_FullscreenPass.cpp
    amd_lib/shared/d3d11/src/AMD_Rand.cpp
    amd_lib/shared/d3d11/src/AMD_SaveRestoreState.cpp
    amd_lib/shared/d3d11/src/AMD_Serialize.cpp
    amd_lib/shared/d3d11/src/AMD_Texture2D.cpp
    amd_lib/shared/d3d11/src/AMD_UnitCube.cpp
    amd_lib/shared/d3d11/src/DirectXTex/ScreenGrab.cpp
)

add_definitions(
	-D_SCL_SECURE_NO_WARNINGS 
	-DAMD_TRESSFX_COMPILE_DYNAMIC_LIB=0 
	-DAMD_SHADOWFX_COMPILE_STATIC_LIB 
	-DAMD_GEOMETRYFX_COMPILE_DYNAMIC_LIB=0
	-DAMD_AOFX_COMPILE_DYNAMIC_LIB=0
	-DAMD_DEPTHOFFIELDFX_COMPILE_DYNAMIC_LIB=0)
add_library(AOFX STATIC ${aofx_src})
add_library(GeometryFX STATIC ${geometryfx_src})
add_library(TressFX STATIC ${tressfx_src})
add_library(DepthOfFieldFX STATIC ${depthoffieldfx_src})
add_library(ShadowFX11 STATIC ${shadowfx11_src})
add_library(ShadowFX12 STATIC ${shadowfx12_src})
add_library(AMDLIB STATIC ${amdlib_src})
target_compile_definitions(ShadowFX12 PRIVATE AMD_SHADOWFX_D3D12)

mark_as_third_party(AOFX ${CMAKE_CURRENT_SOURCE_DIR}/amd_aofx/inc)
mark_as_third_party(GeometryFX ${CMAKE_CURRENT_SOURCE_DIR}/amd_geometryfx/inc)
mark_as_third_party(TressFX ${CMAKE_CURRENT_SOURCE_DIR}/amd_tressfx/inc)
mark_as_third_party(DepthOfFieldFX ${CMAKE_CURRENT_SOURCE_DIR}/amd_depthoffieldfx/inc)
mark_as_third_party(ShadowFX11 ${CMAKE_CURRENT_SOURCE_DIR}/amd_shadowfx/inc)
mark_as_third_party(ShadowFX12 ${CMAKE_CURRENT_SOURCE_DIR}/amd_shadowfx/inc)
mark_as_third_party(AMDLIB 
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/ags_lib/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/shared/common/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/shared/d3d11/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/shared/d3d12/inc)
	
target_include_directories(AOFX PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/amd_aofx/inc)
target_include_directories(GeometryFX PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/amd_geometryfx/inc)
target_include_directories(TressFX PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/amd_tressfx/inc)
target_include_directories(DepthOfFieldFX PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/amd_depthoffieldfx/inc)
target_include_directories(ShadowFX11 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/amd_shadowfx/inc)
target_include_directories(ShadowFX12 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/amd_shadowfx/inc)
target_include_directories(AMDLIB PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/ags_lib/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/shared/common/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/shared/d3d11/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/amd_lib/shared/d3d12/inc)
	
add_target_dependencies(AOFX AMDLIB)
add_target_dependencies(GeometryFX AMDLIB)
add_target_dependencies(TressFX AMDLIB)
add_target_dependencies(DepthOfFieldFX AMDLIB)
add_target_dependencies(ShadowFX11 AMDLIB)
add_target_dependencies(ShadowFX12 AMDLIB)