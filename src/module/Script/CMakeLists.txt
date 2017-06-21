set(SCRIPT_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/Script.cpp

	${SKULD_INCLUDE_DIR}/Skuld/Script/Script.h)
	
if (SKULD_ENABLE_MSIL)
	list(APPEND SCRIPT_SRC 
		${CMAKE_CURRENT_SOURCE_DIR}/MSILLoader.cpp)
endif()

if (SKULD_ENABLE_LUA)
	list(APPEND SCRIPT_SRC 
		${CMAKE_CURRENT_SOURCE_DIR}/LuaLoader.cpp)
endif()

if (SKULD_ENABLE_JAVASCRIPT)
	list(APPEND SCRIPT_SRC 
		${CMAKE_CURRENT_SOURCE_DIR}/JavaScriptLoader.cpp)
endif()

add_module(Script ${SCRIPT_SRC})

set(Script_dep)

if (SKULD_ENABLE_MSIL)
	target_compile_definitions(Script PRIVATE SKULD_ENABLE_MSIL)
	list(APPEND Script_dep libmono)
endif()
if (SKULD_ENABLE_LUA)
	target_compile_definitions(Script PRIVATE SKULD_ENABLE_LUA)
	list(APPEND Script_dep lua)
endif()
if (SKULD_ENABLE_JAVASCRIPT)
	target_compile_definitions(Script PRIVATE SKULD_ENABLE_JAVASCRIPT)
	list(APPEND Script_dep )
endif()
if (SKULD_ENABLE_PYTHON)
	target_compile_definitions(Script PRIVATE SKULD_ENABLE_PYTHON)
	list(APPEND Script_dep )
endif()

add_target_dependencies(Script ${Script_dep})