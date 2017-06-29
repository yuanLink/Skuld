set(SCC_SRC ShaderCrossCompiler.cpp DXBCCompiler Compiler.h DXILCompiler.cpp SPIRVCompiler.cpp
	${SKULD_INCLUDE_DIR}/Skuld/Extra/ShaderCrossCompiler.h)

add_library(ShaderCrossCompiler SHARED ${SCC_SRC})

add_target_dependencies(ShaderCrossCompiler spirv-cross)

mark_as_extra(ShaderCrossCompiler)