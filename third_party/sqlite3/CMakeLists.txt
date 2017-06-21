add_library(sqlite3 STATIC sqlite3.c sqlite3.h sqlite3ext.h)

mark_as_third_party(sqlite3 ${CMAKE_CURRENT_SOURCE_DIR})