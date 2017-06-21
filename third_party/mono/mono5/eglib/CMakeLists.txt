set(MONO_EGLIB_SRC garray.c gbytearray.c gerror.c gfile.c ghashtable.c giconv.c 
    glist.c gmarkup.c gmem.c goutput.c gpath.c gpattern.c gptrarray.c gqsort.c
    gqueue.c gshell.c gslist.c gspawn.c gstr.c gstring.c gunicode.c gutf8.c)

if (MONO_WIN32)
    list(APPEND MONO_EGLIB_SRC gdate-win32.c gdir-win32.c gfile-posix.c gfile-win32.c gmisc-win32.c gmodule-win32.c gtimer-win32.c gunicode-win32.c)
else()
    list(APPEND MONO_EGLIB_SRC gdate-unix.c gdir-unix.c gfile-posix.c gfile-unix.c gmisc-unix.c gmodule-unix.c gtimer-unix.c)
endif()

foreach(src ${MONO_EGLIB_SRC})
    append_mono_src(eglib ${CMAKE_CURRENT_SOURCE_DIR}/src/${src})
endforeach()
