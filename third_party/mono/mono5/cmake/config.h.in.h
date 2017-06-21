/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
#cmakedefine AC_APPLE_UNIVERSAL_BUILD 1

/* Define to 1 if you have the ANSI C header files. */
#cmakedefine STDC_HEADERS 1

/* size of machine integer registers */
#define SIZEOF_REGISTER SIZEOF_VOID_P

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T ${SIZEOF_SIZE_T}

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P ${SIZEOF_VOID_P}

/* The runtime is compiled for cross-compiling mode */
#cmakedefine MONO_CROSS_COMPILE 1

/* ... */
#cmakedefine HOST_AMD64 1

/* ... */
#cmakedefine HOST_ARM 1

/* ... */
#cmakedefine HOST_ARM64 1

/* Host Platform is Win32 */
#cmakedefine HOST_WIN32 1

/* ... */
#cmakedefine HOST_X86 1

/* ... */
#cmakedefine TARGET_AMD64 1

/* ... */
#cmakedefine TARGET_ANDROID 1

/* ... */
#cmakedefine TARGET_ARM 1

/* ... */
#cmakedefine TARGET_ARM64 1

/* The JIT/AOT targets iOS */
#cmakedefine TARGET_IOS 1

/* The JIT/AOT targets Apple platforms */
#cmakedefine TARGET_MACH 1

/* The JIT/AOT targets OSX */
#cmakedefine TARGET_OSX 1

/* ... */
#cmakedefine TARGET_POWERPC 1

/* ... */
#cmakedefine TARGET_POWERPC64 1

/* ... */
#cmakedefine TARGET_PS3 1

/* ... */
#cmakedefine TARGET_PS4 1

/* The JIT/AOT targets WatchOS */
#cmakedefine TARGET_WATCHOS 1

/* Target Platform is Win32 */
#cmakedefine TARGET_WIN32 1

/* ... */
#cmakedefine TARGET_X86 1

/* ... */
#cmakedefine TARGET_XBOX360 1

/* Define to 1 if you have the <pthread.h> header file. */
#cmakedefine HAVE_PTHREAD_H 1

/* Define to 1 if you have the `pthread_kill' function. */
#cmakedefine HAVE_PTHREAD_KILL 1

/* Define to 1 if you have the `pthread_mutex_timedlock' function. */
#cmakedefine HAVE_PTHREAD_MUTEX_TIMEDLOCK 1

/* Define to 1 if you have the <pthread_np.h> header file. */
#cmakedefine HAVE_PTHREAD_NP_H 1

/* Define to 1 if you have the `pthread_setname_np' function. */
#cmakedefine HAVE_PTHREAD_SETNAME_NP 1

/* Define to 1 if you have the `pthread_attr_getstack' function. */
#cmakedefine HAVE_PTHREAD_ATTR_GETSTACK 1

/* Define to 1 if you have the `pthread_attr_getstacksize' function. */
#cmakedefine HAVE_PTHREAD_ATTR_GETSTACKSIZE 1

/* Define to 1 if you have the `pthread_attr_get_np' function. */
#cmakedefine HAVE_PTHREAD_ATTR_GET_NP 1

/* Define to 1 if you have the `pthread_attr_setstacksize' function. */
#cmakedefine HAVE_PTHREAD_ATTR_SETSTACKSIZE 1

/* Define to 1 if you have the `pthread_getattr_np' function. */
#cmakedefine HAVE_PTHREAD_GETATTR_NP 1

/* Define to 1 if you have the `pthread_get_stackaddr_np' function. */
#cmakedefine HAVE_PTHREAD_GET_STACKADDR_NP 1

/* Define to 1 if you have the `pthread_get_stacksize_np' function. */
#cmakedefine HAVE_PTHREAD_GET_STACKSIZE_NP 1

/* Define to 1 if you have the <sys/auxv.h> header file. */
#cmakedefine HAVE_SYS_AUXV_H 1

/* Define to 1 if you have the <sys/epoll.h> header file. */
#cmakedefine HAVE_SYS_EPOLL_H 1

/* Define to 1 if you have the <sys/event.h> header file. */
#cmakedefine HAVE_SYS_EVENT_H 1

/* Define to 1 if you have the <sys/extattr.h> header file. */
#cmakedefine HAVE_SYS_EXTATTR_H 1

/* Define to 1 if you have the <sys/filio.h> header file. */
#cmakedefine HAVE_SYS_FILIO_H 1

/* Define to 1 if you have the <sys/inotify.h> header file. */
#cmakedefine HAVE_SYS_INOTIFY_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#cmakedefine HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#cmakedefine HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/mkdev.h> header file. */
#cmakedefine HAVE_SYS_MKDEV_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#cmakedefine HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/mount.h> header file. */
#cmakedefine HAVE_SYS_MOUNT_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#cmakedefine HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#cmakedefine HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/prctl.h> header file. */
#cmakedefine HAVE_SYS_PRCTL_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#cmakedefine HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/sdt.h> header file. */
#cmakedefine HAVE_SYS_SDT_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#cmakedefine HAVE_SYS_SELECT_H

/* Define to 1 if you have the <sys/sendfile.h> header file. */
#cmakedefine HAVE_SYS_SENDFILE_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#cmakedefine HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
#cmakedefine HAVE_SYS_SOCKIO_H 1

/* Define to 1 if you have the <sys/statfs.h> header file. */
#cmakedefine HAVE_SYS_STATFS_H 1

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#cmakedefine HAVE_SYS_STATVFS_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/syscall.h> header file. */
#cmakedefine HAVE_SYS_SYSCALL_H 1

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#cmakedefine HAVE_SYS_SYSCTL_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#cmakedefine HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#cmakedefine HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#cmakedefine HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#cmakedefine HAVE_SYS_UN_H 1

/* Define to 1 if you have the <sys/user.h> header file. */
#cmakedefine HAVE_SYS_USER_H 1

/* Define to 1 if you have the <sys/utime.h> header file. */
#cmakedefine HAVE_SYS_UTIME_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#cmakedefine HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <sys/vfstab.h> header file. */
#cmakedefine HAVE_SYS_VFSTAB_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#cmakedefine HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <sys/xattr.h> header file. */
#cmakedefine HAVE_SYS_XATTR_H 1

/* Define to 1 if you have the declaration of `InterlockedAdd', and to 0 if
   you don't. */
#cmakedefine01 HAVE_DECL_INTERLOCKEDADD

/* Define to 1 if you have the declaration of `InterlockedAdd64', and to 0 if
   you don't. */
#cmakedefine01 HAVE_DECL_INTERLOCKEDADD64

/* Define to 1 if you have the declaration of `InterlockedCompareExchange64',
   and to 0 if you don't. */
#cmakedefine01 HAVE_DECL_INTERLOCKEDCOMPAREEXCHANGE64

/* Define to 1 if you have the declaration of `InterlockedDecrement64', and to
   0 if you don't. */
#cmakedefine01 HAVE_DECL_INTERLOCKEDDECREMENT64

/* Define to 1 if you have the declaration of `InterlockedExchange64', and to
   0 if you don't. */
#cmakedefine01 HAVE_DECL_INTERLOCKEDEXCHANGE64

/* Define to 1 if you have the declaration of `InterlockedIncrement64', and to
   0 if you don't. */
#cmakedefine01 HAVE_DECL_INTERLOCKEDINCREMENT64

/* Support for the visibility ("hidden") attribute */
#cmakedefine HAVE_VISIBILITY_HIDDEN 1

/* Have tm_gmtoff */
#cmakedefine HAVE_TM_GMTOFF 1

/* Define to 1 if you have the `trunc' function. */
#cmakedefine HAVE_TRUNC 1

/* Define to 1 if you have the `ttyname_r' function. */
#cmakedefine HAVE_TTYNAME_R 1

/* Define to 1 if you have the <ucontext.h> header file. */
#cmakedefine HAVE_UCONTEXT_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H 1

/* Define to 1 if you have the <unwind.h> header file. */
#cmakedefine HAVE_UNWIND_H 1

/* Define to 1 if you have /usr/include/malloc.h. */
#cmakedefine HAVE_USR_INCLUDE_MALLOC_H 1

/* Define to 1 if you have the `utimensat' function. */
#cmakedefine HAVE_UTIMENSAT 1

/* Define to 1 if you have the <utime.h> header file. */
#cmakedefine HAVE_UTIME_H 1

/* Define to 1 if you have the `vsnprintf' function. */
#cmakedefine HAVE_VSNPRINTF 1

/* Define to 1 if you have the <wchar.h> header file. */
#cmakedefine HAVE_WCHAR_H 1

/* Define to 1 if you have the <alloca.h> header file. */
#cmakedefine HAVE_ALLOCA_H 1

/* The architecture this is running on */
#define MONO_ARCHITECTURE "${MONO_ARCHITECTURE}"

/* Enable the allocation and indexing of arrays greater than Int32.MaxValue */
#cmakedefine MONO_BIG_ARRAYS 1

/* Version of the corlib-runtime interface */
#define MONO_CORLIB_VERSION 1050000000

/* Disable banned functions from being used by the runtime */
#cmakedefine MONO_INSIDE_RUNTIME 1

/* The LLVM back end is dynamically loaded */
#cmakedefine MONO_LLVM_LOADED 1

/* AOT cross offsets file */
#cmakedefine MONO_OFFSETS_FILE 1

/* Reduce runtime requirements (and capabilities) */
#cmakedefine MONO_SMALL_CONFIG 1

/* Xen-specific behaviour */
#cmakedefine MONO_XEN_OPT 1

/* Length of zero length arrays */
#define MONO_ZERO_LEN_ARRAY ${MONO_ZERO_LEN_ARRAY}

/* Name of /dev/random */
#define NAME_DEV_RANDOM "${NAME_DEV_RANDOM}"

/* Define to 1 if the system has the type `suseconds_t'. */
#cmakedefine HAVE_SUSECONDS_T 1

/* Define to 1 if you have the `swab' function. */
#cmakedefine HAVE_SWAB 1

/* Define to 1 if you have the `sysconf' function. */
#cmakedefine HAVE_SYSCONF 1

/* Define to 1 if you have the <syslog.h> header file. */
#cmakedefine HAVE_SYSLOG_H 1

/* Define to 1 if you have the `system' function. */
#cmakedefine HAVE_SYSTEM 1

/* Define to 1 if you have the `statfs' function. */
#cmakedefine HAVE_STATFS 1

/* Define to 1 if you have the `statvfs' function. */
#cmakedefine HAVE_STATVFS 1

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine HAVE_STDLIB_H 1

/* Define to 1 if you have the `stime' function. */
#cmakedefine HAVE_STIME 1

/* Define to 1 if you have the `strerror_r' function. */
#cmakedefine HAVE_STRERROR_R 1

/* Define to 1 if you have the <strings.h> header file. */
#cmakedefine HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#cmakedefine HAVE_STRING_H 1

/* Define to 1 if you have the `poll' function. */
#cmakedefine HAVE_POLL 1

/* Define to 1 if you have the <poll.h> header file. */
#cmakedefine HAVE_POLL_H 1

/* Define to 1 if you have the `posix_fadvise' function. */
#cmakedefine HAVE_POSIX_FADVISE 1

/* Define to 1 if you have the `posix_fallocate' function. */
#cmakedefine HAVE_POSIX_FALLOCATE 1

/* Define to 1 if you have the `posix_madvise' function. */
#cmakedefine HAVE_POSIX_MADVISE 1

/* Define to 1 if you have the `prctl' function. */
#cmakedefine HAVE_PRCTL 1

/* Define to 1 if you have the `preadv' function. */
#cmakedefine HAVE_PREADV 1

/* Define to 1 if you have the `psignal' function. */
#cmakedefine HAVE_PSIGNAL 1

/* Define to 1 if you have the <pwd.h> header file. */
#cmakedefine HAVE_PWD_H 1

/* ... */
#cmakedefine USE_MACH_SEMA 1

/* This platform does not support symlinks */
#cmakedefine PLATFORM_NO_SYMLINKS 1

/* ... */
#cmakedefine USE_GCC_ATOMIC_OPS 1

/* Define to 1 if you have the `pwritev' function. */
#cmakedefine HAVE_PWRITEV 1

/* Define to 1 if you have the `readlinkat' function. */
#cmakedefine HAVE_READLINKAT 1

/* Define to 1 if you have the `readv' function. */
#cmakedefine HAVE_READV 1

/* Define to 1 if you have the `remap_file_pages' function. */
#cmakedefine HAVE_REMAP_FILE_PAGES 1

/* Define to 1 if you have the `sched_getaffinity' function. */
#cmakedefine HAVE_SCHED_GETAFFINITY 1

/* Define to 1 if you have the `sched_getcpu' function. */
#cmakedefine HAVE_SCHED_GETCPU 1

/* Define to 1 if you have the `sched_setaffinity' function. */
#cmakedefine HAVE_SCHED_SETAFFINITY 1

/* Define to 1 if you have the `seekdir' function. */
#cmakedefine HAVE_SEEKDIR 1

/* Define to 1 if you have the <semaphore.h> header file. */
#cmakedefine HAVE_SEMAPHORE_H 1

/* Define to 1 if you have the `sendfile' function. */
#cmakedefine HAVE_SENDFILE 1

/* Define to 1 if you have the `setdomainname' function. */
#cmakedefine HAVE_SETDOMAINNAME 1

/* Define to 1 if you have the `setgrent' function. */
#cmakedefine HAVE_SETGRENT 1

/* Define to 1 if you have the `setgroups' function. */
#cmakedefine HAVE_SETGROUPS 1

/* Define to 1 if you have the `sethostid' function. */
#cmakedefine HAVE_SETHOSTID 1

/* Define to 1 if you have the `sethostname' function. */
#cmakedefine HAVE_SETHOSTNAME 1

/* Define to 1 if you have the `setpgid' function. */
#cmakedefine HAVE_SETPGID 1

/* Define to 1 if you have the `setpriority' function. */
#cmakedefine HAVE_SETPRIORITY 1

/* Define to 1 if you have the `setpwent' function. */
#cmakedefine HAVE_SETPWENT 1

/* Define to 1 if you have the `setresuid' function. */
#cmakedefine HAVE_SETRESUID 1

/* Define to 1 if you have the `setusershell' function. */
#cmakedefine HAVE_SETUSERSHELL 1

/* Define to 1 if you have the `shm_open' function. */
#cmakedefine HAVE_SHM_OPEN 1

/* Define to 1 if you have the `sigaction' function. */
#cmakedefine HAVE_SIGACTION 1

/* Define to 1 if you have the `signal' function. */
#cmakedefine HAVE_SIGNAL 1

/* Define to 1 if you have the <signal.h> header file. */
#cmakedefine HAVE_SIGNAL_H 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#cmakedefine HAVE_ARPA_INET_H 1

/* Define to 1 if you have the <asm/sigcontext.h> header file. */
#cmakedefine HAVE_ASM_SIGCONTEXT_H 1

/* Define to 1 if you have the <attr/xattr.h> header file. */
#cmakedefine HAVE_ATTR_XATTR_H 1

/* Define to 1 if you have the `backtrace_symbols' function. */
#cmakedefine HAVE_BACKTRACE_SYMBOLS 1

/* Define to 1 if the system has the type `blkcnt_t'. */
#cmakedefine HAVE_BLKCNT_T 1

/* Define to 1 if the system has the type `blksize_t'. */
#cmakedefine HAVE_BLKSIZE_T 1

/* Define to 1 if you have the <checklist.h> header file. */
#cmakedefine HAVE_CHECKLIST_H 1

/* Define to 1 if you have the <nacl/nacl_dyncode.h> header file. */
#cmakedefine HAVE_NACL_NACL_DYNCODE_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#cmakedefine HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#cmakedefine HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#cmakedefine HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <net/if.h> header file. */
#cmakedefine HAVE_NET_IF_H 1

/* Define to 1 if you have the <link.h> header file. */
#cmakedefine HAVE_LINK_H 1

/* Define to 1 if you have the <linux/magic.h> header file. */
#cmakedefine HAVE_LINUX_MAGIC_H 1

/* Define to 1 if you have the <linux/netlink.h> header file. */
#cmakedefine HAVE_LINUX_NETLINK_H 1

/* Define to 1 if you have the <linux/rtc.h> header file. */
#cmakedefine HAVE_LINUX_RTC_H 1

/* Define to 1 if you have the <linux/rtnetlink.h> header file. */
#cmakedefine HAVE_LINUX_RTNETLINK_H 1

/* Define to 1 if you have the <linux/serial.h> header file. */
#cmakedefine HAVE_LINUX_SERIAL_H 1

/* Define to 1 if you have the `lockf' function. */
#cmakedefine HAVE_LOCKF 1

/* Define to 1 if you have the `lutimes' function. */
#cmakedefine HAVE_LUTIMES 1

/* Define to 1 if you have the `madvise' function. */
#cmakedefine HAVE_MADVISE 1

/* Define to 1 if you have the <memory.h> header file. */
#cmakedefine HAVE_MEMORY_H 1

/* Define to 1 if you have the <dirent.h> header file. */
#cmakedefine HAVE_DIRENT_H 1

/* Define to 1 if you have the `dladdr' function. */
#cmakedefine HAVE_DLADDR 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#cmakedefine HAVE_DLFCN_H 1

/* Define to 1 if you have the `dl_iterate_phdr' function. */
#cmakedefine HAVE_DL_ITERATE_PHDR 1

/* Define to 1 if you have the <elf.h> header file. */
#cmakedefine HAVE_ELF_H 1

/* Define to 1 if you have the `endgrent' function. */
#cmakedefine HAVE_ENDGRENT 1

/* Define to 1 if you have the `endpwent' function. */
#cmakedefine HAVE_ENDPWENT 1

/* Define to 1 if you have the `endusershell' function. */
#cmakedefine HAVE_ENDUSERSHELL 1

/* Define to 1 if you have the `epoll_ctl' function. */
#cmakedefine HAVE_EPOLL_CTL 1

/* Define to 1 if you have the <execinfo.h> header file. */
#cmakedefine HAVE_EXECINFO_H 1

/* Define to 1 if you have the `execv' function. */
#cmakedefine HAVE_EXECV 1

/* Define to 1 if you have the `execve' function. */
#cmakedefine HAVE_EXECVE 1

/* Define to 1 if you have the `execvp' function. */
#cmakedefine HAVE_EXECVP 1

/* Define to 1 if you have the `fgetgrent' function. */
#cmakedefine HAVE_FGETGRENT 1

/* Define to 1 if you have the `fgetpwent' function. */
#cmakedefine HAVE_FGETPWENT 1

/* Define to 1 if you have the `finite' function. */
#cmakedefine HAVE_FINITE 1

/* Define to 1 if you have the `fork' function. */
#cmakedefine HAVE_FORK 1

/* Define to 1 if you have the <fstab.h> header file. */
#cmakedefine HAVE_FSTAB_H 1

/* Define to 1 if you have the `fstatat' function. */
#cmakedefine HAVE_FSTATAT 1

/* Define to 1 if you have the `fstatfs' function. */
#cmakedefine HAVE_FSTATFS 1

/* Define to 1 if you have the `fstatvfs' function. */
#cmakedefine HAVE_FSTATVFS 1

/* Define to 1 if you have the `futimens' function. */
#cmakedefine HAVE_FUTIMENS 1

/* Define to 1 if you have the `futimes' function. */
#cmakedefine HAVE_FUTIMES 1

/* Define to 1 if you have the <CommonCrypto/CommonDigest.h> header file. */
#cmakedefine HAVE_COMMONCRYPTO_COMMONDIGEST_H 1

/* Define to 1 if you have the <complex.h> header file. */
#cmakedefine HAVE_COMPLEX_H 1

/* Define to 1 if you have the <curses.h> header file. */
#cmakedefine HAVE_CURSES_H 1

/* Define to 1 if you have the `clock_nanosleep' function. */
#cmakedefine HAVE_CLOCK_NANOSLEEP 1

/* Define to 1 if you have the `confstr' function. */
#cmakedefine HAVE_CONFSTR 1

/* Define to 1 if you have the `getdomainname' function. */
#cmakedefine HAVE_GETDOMAINNAME 1

/* Define to 1 if you have the `getfsstat' function. */
#cmakedefine HAVE_GETFSSTAT 1

/* Define to 1 if you have the `getgrent' function. */
#cmakedefine HAVE_GETGRENT 1

/* Define to 1 if you have the `getgrgid_r' function. */
#cmakedefine HAVE_GETGRGID_R 1

/* Define to 1 if you have the `getgrnam_r' function. */
#cmakedefine HAVE_GETGRNAM_R 1

/* Define to 1 if you have the `gethostid' function. */
#cmakedefine HAVE_GETHOSTID 1

/* Define to 1 if you have the declaration of `__readfsdword', and to 0 if you
   don't. */
#cmakedefine01 HAVE_DECL___READFSDWORD

/* Define to 1 if you have the `getlogin_r' function. */
#cmakedefine HAVE_GETLOGIN_R 1

/* Define to 1 if you have the `getpriority' function. */
#cmakedefine HAVE_GETPRIORITY 1

/* Define to 1 if you have the `getpwent' function. */
#cmakedefine HAVE_GETPWENT 1

/* Define to 1 if you have the `getpwnam_r' function. */
#cmakedefine HAVE_GETPWNAM_R 1

/* Define to 1 if you have the `getpwuid_r' function. */
#cmakedefine HAVE_GETPWUID_R 1

/* Define to 1 if you have the `getresuid' function. */
#cmakedefine HAVE_GETRESUID 1

/* Define to 1 if you have the `getrlimit' function. */
#cmakedefine HAVE_GETRLIMIT 1

/* Define to 1 if you have the `getrusage' function. */
#cmakedefine HAVE_GETRUSAGE 1

/* Define to 1 if you have the <grp.h> header file. */
#cmakedefine HAVE_GRP_H 1

/* Define to 1 if you have the <ieeefp.h> header file. */
#cmakedefine HAVE_IEEEFP_H 1

/* Define to 1 if you have the `inet_aton' function. */
#cmakedefine HAVE_INET_ATON 1

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isfinite' function. */
#cmakedefine HAVE_ISFINITE 1

/* isinf available */
#cmakedefine HAVE_ISINF 1

/* Define to 1 if you have the `kill' function. */
#cmakedefine HAVE_KILL 1

/* Define to 1 if you have the `kqueue' function. */
#cmakedefine HAVE_KQUEUE 1

/* Have getaddrinfo */
#cmakedefine HAVE_GETADDRINFO 1

/* Have gethostbyname */
#cmakedefine HAVE_GETHOSTBYNAME 1

/* Have gethostbyname2 */
#cmakedefine HAVE_GETHOSTBYNAME2 1

/* Have gethostbyname2_r */
#cmakedefine HAVE_GETHOSTBYNAME2_R 1

/* Have getifaddrs */
#cmakedefine HAVE_GETIFADDRS 1

/* Have getnameinfo */
#cmakedefine HAVE_GETNAMEINFO 1

/* Have getprotobyname */
#cmakedefine HAVE_GETPROTOBYNAME 1

/* Define to 1 if you have the `mknodat' function. */
#cmakedefine HAVE_MKNODAT 1

/* Define to 1 if you have the `mkstemp' function. */
#cmakedefine HAVE_MKSTEMP 1

/* Define to 1 if you have the `mmap' function. */
#cmakedefine HAVE_MMAP 1

/* Define to 1 if you have the `mremap' function. */
#cmakedefine HAVE_MREMAP 1

/* byte order of target */
#define TARGET_BYTE_ORDER G_BYTE_ORDER

#ifdef HOST_WIN32
#ifndef HAVE_WINAPI_FAMILY_SUPPORT

#define HAVE_WINAPI_FAMILY_SUPPORT

/* WIN API Family support */
#include <winapifamily.h>

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	#define HAVE_CLASSIC_WINAPI_SUPPORT 1
	#define HAVE_UWP_WINAPI_SUPPORT 0
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
	#define HAVE_CLASSIC_WINAPI_SUPPORT 0
	#define HAVE_UWP_WINAPI_SUPPORT 1
#ifndef HAVE_EXTERN_DEFINED_WINAPI_SUPPORT
	#error Unsupported WINAPI family
#endif
#else
	#define HAVE_CLASSIC_WINAPI_SUPPORT 0
	#define HAVE_UWP_WINAPI_SUPPORT 0
#ifndef HAVE_EXTERN_DEFINED_WINAPI_SUPPORT
	#error Unsupported WINAPI family
#endif
#endif
#endif

#endif

/* Name of package */
#define PACKAGE "mono"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.xamarin.com/enter_bug.cgi?classification=Mono"

/* Define to the full name of this package. */
#define PACKAGE_NAME "mono"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "mono 5.0.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "mono"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "5.0.0"

/* Version number of package */
#define VERSION "5.0.0"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* GC description */
#define DEFAULT_GC_NAME "Included Boehm (with typed GC and Parallel Mark)"

/* String of disabled features */
#define DISABLED_FEATURES "${MONO_DISABLE_FEATURE}"

/* epoll supported */
#cmakedefine HAVE_EPOLL 1

/* Have inet_ntop */
#cmakedefine HAVE_INET_NTOP 1

/* Have inet_pton */
#cmakedefine HAVE_INET_PTON 1

/* GLIBC has CPU_COUNT macro in sched.h */
#cmakedefine GLIBC_HAS_CPU_COUNT 1

/* Define to 1 if `kp_proc' is a member of `struct kinfo_proc'. */
#cmakedefine HAVE_STRUCT_KINFO_PROC_KP_PROC 1

/* ARM v5 */
#cmakedefine HAVE_ARMV5 1

/* ARM v6 */
#cmakedefine HAVE_ARMV6 1

/* ARM v7 */
#cmakedefine HAVE_ARMV7 1

























/* pthread is a pointer */
#cmakedefine PTHREAD_POINTER_ID 1

/* Don't use UWP Windows API support */
#cmakedefine01 HAVE_UWP_WINAPI_SUPPORT

/* Supports C99 array initialization */
#cmakedefine HAVE_ARRAY_ELEM_INIT 1

/* Can get interface list */
#cmakedefine HAVE_SIOCGIFCONF 1

/* Have system zlib */
#cmakedefine HAVE_SYS_ZLIB 1

/* Have system zlib */
#cmakedefine HAVE_ZLIB 1

/* Have mincore */
#cmakedefine HAVE_MINCORE 1

/* Moving collector */
#cmakedefine HAVE_MOVING_COLLECTOR 1

/* Have MSG_NOSIGNAL */
#cmakedefine HAVE_MSG_NOSIGNAL 1

/* Disable AOT Compiler */
/* #undef DISABLE_AOT */

/* Disable support for multiple appdomains. */
/* #undef DISABLE_APPDOMAINS */

/* Disable assembly remapping. */
/* #undef DISABLE_ASSEMBLY_REMAPPING */

/* Disable agent attach support */
/* #undef DISABLE_ATTACH */

/* Disable COM support */
/* #undef DISABLE_COM */

/* Disable runtime debugging support */
/* #undef DISABLE_DEBUG */

/* Disable System.Decimal support */
/* #undef DISABLE_DECIMAL */

/* Disable generics support */
/* #undef DISABLE_GENERICS */

/* ... */
/* #undef DISABLE_HW_TRAPS */

/* Icall tables disabled */
/* #undef DISABLE_ICALL_TABLES */

/* Disable the JIT, only full-aot mode will be supported by the runtime. */
#cmakedefine DISABLE_JIT 1

/* Disable support for huge assemblies */
/* #undef DISABLE_LARGE_CODE */

/* Disable support code for the LLDB plugin. */
/* #undef DISABLE_LLDB */

/* Disable support for .mdb symbol files. */
/* #undef DISABLE_MDB */

/* Disable String normalization support. */
/* #undef DISABLE_NORMALIZATION */

/* Disable Performance Counters. */
/* #undef DISABLE_PERFCOUNTERS */

/* Disable P/Invoke support */
/* #undef DISABLE_PINVOKE */

/* Define to 1 if you have the <libproc.h> header file. */
#cmakedefine HAVE_LIBPROC_H 1

/* Define to 1 if you have the <termios.h> header file. */
#cmakedefine HAVE_TERMIOS_H 1

/* Define to 1 if you have the <term.h> header file. */
#cmakedefine HAVE_TERM_H 1

/* Define to 1 if you have the <winternl.h> header file. */
#cmakedefine HAVE_WINTERNL_H 1

/* Define to 1 if you have the <pathconf.h> header file. */
#cmakedefine HAVE_PATHCONF_H 1

/* Use classic Windows API support */
#define HAVE_CLASSIC_WINAPI_SUPPORT 1

/* Disables the IO portability layer */
#cmakedefine DISABLE_PORTABILITY 1

/* Disable support debug logging */
#cmakedefine DISABLE_LOGGING 1

/* Disable default profiler support */
#cmakedefine DISABLE_PROFILER 1

/* Disable reflection emit support */
#cmakedefine DISABLE_REFLECTION_EMIT 1

/* Disable assembly saving support in reflection emit */
#cmakedefine DISABLE_REFLECTION_EMIT_SAVE 1

/* Disable remoting support (This disables type proxies and make com
   non-functional) */
/* #undef DISABLE_REMOTING */

/* Disable CAS/CoreCLR security */
/* #undef DISABLE_SECURITY */

/* Disable major=copying support in SGEN. */
/* #undef DISABLE_SGEN_MAJOR_COPYING */

/* Disable major=marksweep-fixed support in SGEN. */
/* #undef DISABLE_SGEN_MAJOR_MARKSWEEP_FIXED */

/* Disable major=marksweep-fixed-par support in SGEN. */
/* #undef DISABLE_SGEN_MAJOR_MARKSWEEP_FIXED_PAR */

/* Disable major=marksweep-par support in SGEN. */
/* #undef DISABLE_SGEN_MAJOR_MARKSWEEP_PAR */

/* Disable wbarrier=remset support in SGEN. */
/* #undef DISABLE_SGEN_REMSET */

/* Disable Shadow Copy for AppDomains */
/* #undef DISABLE_SHADOW_COPY */

/* Disable shared perfcounters. */
/* #undef DISABLE_SHARED_PERFCOUNTERS */

/* Disable SIMD intrinsics related optimizations. */
/* #undef DISABLE_SIMD */

/* Disable sockets support */
/* #undef DISABLE_SOCKETS */

/* Disable Soft Debugger Agent. */
/* #undef DISABLE_SOFT_DEBUG */

/* Disable advanced SSA JIT optimizations */
/* #undef DISABLE_SSA */

/* Disables the verifier */
/* #undef DISABLE_VERIFIER */

/* Enable checked build */
/* #undef ENABLE_CHECKED_BUILD */

/* Enable GC checked build */
/* #undef ENABLE_CHECKED_BUILD_GC */

/* Enable metadata checked build */
/* #undef ENABLE_CHECKED_BUILD_METADATA */

/* Enable thread checked build */
/* #undef ENABLE_CHECKED_BUILD_THREAD */

/* Enable DTrace probes */
/* #undef ENABLE_DTRACE */

/* Extension module enabled */
/* #undef ENABLE_EXTENSION_MODULE */

/* Icall export enabled */
/* #undef ENABLE_ICALL_EXPORT */

/* Icall symbol map enabled */
/* #undef ENABLE_ICALL_SYMBOL_MAP */

/* Enable interpreter in the runtime. */
/* #undef ENABLE_INTERPRETER */

/* Have large file support */
#define HAVE_LARGE_FILE_SUPPORT 1

/* Targeting the Android platform */
#cmakedefine PLATFORM_ANDROID 1

/* Enable lazy gc thread creation by the embedding host. */
/* #undef LAZY_GC_THREAD_CREATION */

/* Full version of LLVM libraties */
/* #undef LLVM_VERSION */

/* Have GLIBC_BEFORE_2_3_4_SCHED_SETAFFINITY */
#cmakedefine GLIBC_BEFORE_2_3_4_SCHED_SETAFFINITY 1


