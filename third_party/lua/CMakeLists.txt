file(GLOB templuasrc src/*.c src/*.h)

set(lua_src)
foreach(f ${templuasrc})
    if((NOT f MATCHES "lua.c$") AND (NOT f MATCHES "luac.c$"))
        list(APPEND lua_src ${f})
    endif()
endforeach()

add_library(lua STATIC ${lua_src})
mark_as_third_party(lua ${CMAKE_CURRENT_SOURCE_DIR}/src)