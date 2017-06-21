add_library(spirv-cross STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/GLSL.std.450.h
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cross.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cross.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cfg.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_cfg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_glsl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spirv_glsl.hpp)

mark_as_third_party(spirv-cross ${CMAKE_CURRENT_SOURCE_DIR})