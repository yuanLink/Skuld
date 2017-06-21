add_library(jsoncpp STATIC src/lib_json/json_reader.cpp src/lib_json/json_value.cpp src/lib_json/json_writer.cpp)

mark_as_third_party(jsoncpp ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)