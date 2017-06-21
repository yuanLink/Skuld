set(SCC_SRC ShaderCrossCompiler.cpp DXBCCompiler Compiler.h DXILCompiler.cpp SPIRVCompiler.cpp MetalIRCompiler.cpp
	${SKULD_INCLUDE_DIR}/Skuld/Extra/ShaderCrossCompiler.h)

add_library(ShaderCrossCompilerStatic STATIC ${SCC_SRC})
add_library(ShaderCrossCompiler SHARED ${SCC_SRC})

add_target_dependencies(ShaderCrossCompilerStatic spirv-cross)
add_target_dependencies(ShaderCrossCompiler spirv-cross)

mark_as_extra(ShaderCrossCompilerStatic)
mark_as_extra(ShaderCrossCompiler)