#               metadata

set(MONO_METADATA_SRC appdomain.c assembly.c attach.c class.c class-accessors.c cominterop.c custom-attrs.c debug-helpers.c debug-mono-ppdb.c debug-mono-symfile.c
     decimal-ms.c domain.c dynamic-image.c dynamic-stream.c environment.c exception.c filewatcher.c
     gc.c gc-stats.c handle.c icall.c image.c jit-info.c loader.c locales.c
     marshal.c mempool.c metadata.c metadata-cross-helpers.c metadata-verify.c method-builder.c
     monitor.c mono-basic-block.c mono-config.c mono-config-dirs.c mono-debug.c mono-hash.c mono-mlist.c
     mono-perfcounters.c mono-route.c mono-security.c number-ms.c object.c opcodes.c profiler.c property-bag.c rand.c reflection.c remoting.c runtime.c
     security-core-clr.c security-manager.c seq-points-data.c sgen-bridge.c sgen-mono.c sgen-new-bridge.c sgen-old-bridge.c sgen-stw.c sgen-tarjan-bridge.c
     sgen-toggleref.c sre.c sre-encode.c sre-save.c string-icalls.c sysmath.c threadpool.c threadpool-io.c threadpool-worker-default.c threads.c verify.c
     w32file.c w32handle.c w32handle-namespace.c w32process.c w32socket.c)
if (MONO_WIN32)
    list(APPEND MONO_METADATA_SRC console-win32.c coree.c file-mmap-windows.c 
    icall-windows.c marshal-windows.c mono-security-windows.c
    w32error-win32.c w32event-win32.c w32file-win32.c w32mutex-win32.c 
    w32process-win32.c w32semaphore-win32.c w32socket-win32.c)
elseif(MONO_UNIX OR MONO_ANDROID)
    list(APPEND MONO_METADATA_SRC console-unix.c file-mmap-posix.c w32error-unix.c w32event-unix.c
        w32file-unix.c w32file-unix-glob.c w32mutex-unix.c w32process-unix.c w32process-unix-default.c w32semaphore-unix.c w32socket-unix.c)
elseif(MONO_OSX)
    list(APPEND MONO_METADATA_SRC console-unix.c file-mmap-posix.c w32error-unix.c w32event-unix.c
        w32file-unix.c w32file-unix-glob.c w32mutex-unix.c w32process-unix.c w32process-unix-osx.c w32semaphore-unix.c w32socket-unix.c)
endif()

foreach(src ${MONO_METADATA_SRC})
    append_mono_src(mono ${CMAKE_CURRENT_SOURCE_DIR}/metadata/${src})
endforeach()

#               utils

set(MONO_UTILS_SRC atomic.c bsearch.c dlmalloc.c hazard-pointer.c json.c lock-free-alloc.c lock-free-array-queue.c lock-free-queue.c 
    memfuncs.c monobitset.c mono-counters.c 
    mono-codeman.c mono-conc-hashtable.c mono-context.c mono-dl.c mono-embed.c
    mono-error.c mono-filemap.c mono-hwcap.c mono-internal-hash.c mono-io-portability.c mono-linked-list-set.c mono-logger.c
    mono-log-common.c mono-math.c mono-mmap.c mono-networkinterfaces.c mono-path.c 
    mono-poll.c mono-proclib.c mono-property-hash.c mono-publib.c mono-rand.c mono-sha1.c mono-threads.c 
    mono-threads-coop.c mono-threads-state-machine.c mono-time.c mono-tls.c mono-uri.c
    networking.c networking-posix.c parse.c strenc.c)
if (MONO_WIN32)
    list(APPEND MONO_UTILS_SRC mono-dl-windows.c mono-log-windows.c mono-mmap-windows.c mono-proclib-windows.c
        mono-rand-windows.c mono-threads-windows.c
        networking-windows.c os-event-win32.c)
    if (MONO_AMD64)
        list(APPEND MONO_UTILS_SRC win64.asm)
    endif()
elseif(MONO_UNIX)
    list(APPEND MONO_UTILS_SRC mono-dl-posix.c mono-log-posix.c mono-threads-linux.c
        mono-threads-posix.c mono-threads-posix-signals.c networking-posix.c os-event-unix.c)
elseif(MONO_OSX) 
    list(APPEND MONO_UTILS_SRC mach-support.c mono-dl-darwin.c mono-dl-posix.c mono-log-posix.c
        mono-threads-mach.c mono-threads-mach-helper.c mono-threads-posix.c mono-threads-posix-signals.c networking-posix.c os-event-unix.c)
    if (MONO_AMD64)
        list(APPEND MONO_UTILS_SRC mach-support-amd64.c)
    endif()
elseif(MONO_ANDROID)
    list(APPEND MONO_UTILS_SRC mono-dl-posix.c mono-log-android.c mono-threads-android.c
        mono-threads-posix.c mono-threads-posix-signals.c networking-posix.c os-event-unix.c)
endif()

if (MONO_CROSS_COMPILE)
    list(APPEND MONO_UTILS_SRC mono-hwcap-cross.c)
else()
    if (MONO_AMD64 OR MONO_X86)
        list(APPEND MONO_UTILS_SRC mono-hwcap-x86.c)
    elseif (MONO_ARM)
        list(APPEND MONO_UTILS_SRC mono-hwcap-arm.c)
    elseif (MONO_ARM64)
        list(APPEND MONO_UTILS_SRC mono-hwcap-arm64.c)
    elseif (MONO_POWERPC OR MONO_POWERPC64)
        list(APPEND MONO_UTILS_SRC mono-hwcap-ppc.c)
    endif()
endif()

foreach(src ${MONO_UTILS_SRC})
    append_mono_src(utils ${CMAKE_CURRENT_SOURCE_DIR}/utils/${src})
endforeach()

#               sgen

set(MONO_SGEN_SRC sgen-alloc.c sgen-array-list.c sgen-cardtable.c sgen-debug.c sgen-descriptor.c
    sgen-fin-weak-hash.c sgen-gc.c sgen-gchandles.c sgen-gray.c sgen-hash-table.c sgen-internal.c sgen-layout-stats.c
    sgen-los.c sgen-marksweep.c sgen-memory-governor.c sgen-nursery-allocator.c
    sgen-pinning.c sgen-pinning-stats.c sgen-pointer-queue.c sgen-protocol.c
    sgen-qsort.c sgen-simple-nursery.c sgen-split-nursery.c sgen-thread-pool.c sgen-workers.c)

foreach(src ${MONO_SGEN_SRC})
    append_mono_src(sgen ${CMAKE_CURRENT_SOURCE_DIR}/sgen/${src})
endforeach()

#               mini

set(MONO_MINI_SRC abcremoval.c alias-analysis.c aot-compiler.c aot-runtime.c arch-stubs.c branch-opts.c cfgdump.c cfold.c debugger-agent.c debug-mini.c 
    decompose.c dominators.c driver.c dwarfwriter.c graph.c helpers.c image-writer.c
    jit-icalls.c linear-scan.c liveness.c lldb.c local-propagation.c method-to-ir.c mini.c mini-codegen.c mini-cross-helpers.c
    mini-exceptions.c mini-gc.c mini-generic-sharing.c mini-native-types.c mini-runtime.c mini-trampolines.c
    seq-points.c simd-intrinsics.c ssa.c tasklets.c trace.c type-checking.c unwind.c xdebug.c)

if (MONO_WIN32)
    list(APPEND MONO_MINI_SRC mini-windows.c)
elseif (MONO_UNIX OR MONO_ANDROID)
    list(APPEND MONO_MINI_SRC mini-posix.c)
elseif(MONO_OSX)
    list(APPEND MONO_MINI_SRC mini-posix.c mini-darwin.c)
endif()

foreach(src ${MONO_MINI_SRC})
    append_mono_src(mini ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()

foreach(src exceptions-amd64.c mini-amd64.c mini-amd64-gsharedvt.c 
        tramp-amd64.c tramp-amd64-gsharedvt.c)
    append_mono_src(mini-amd64 ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()

foreach(src exceptions-x86.c mini-x86.c mini-x86-gsharedvt.c 
        tramp-x86.c tramp-x86-gsharedvt.c)
    append_mono_src(mini-x86 ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()

foreach(src exceptions-arm64.c mini-arm64.c mini-arm64-gsharedvt.c 
        tramp-arm64.c tramp-arm64-gsharedvt.c)
    append_mono_src(mini-arm64 ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()

foreach(src exceptions-arm.c mini-arm.c mini-arm-gsharedvt.c 
        tramp-arm.c tramp-arm-gsharedvt.c)
    append_mono_src(mini-arm ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()

foreach(src exceptions-ppc.c mini-ppc.c tramp-ppc.c)
    append_mono_src(mini-ppc ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()

foreach(src exceptions-ppc.c mini-ppc.c tramp-ppc.c)
    append_mono_src(mini-ppc64 ${CMAKE_CURRENT_SOURCE_DIR}/mini/${src})
endforeach()