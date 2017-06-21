#pragma once

#if defined(SKULD_BUILD_DYNAMIC)
#	if defined(_WIN32)
#		if defined(SKULD_PLUGIN) || defined(SKULD_EXTRA)
#			define SKULD_EXPORT
#		else
#			define SKULD_EXPORT __declspec(dllexport)
#		endif
#	else
#		if defined(SKULD_PLUGIN) || defined(SKULD_EXTRA)
#			define SKULD_EXPORT 
#		else
#			define SKULD_EXPORT __attribute__ ((visibility("default")))
#		endif
#	endif
#elif defined(SKULD_USE_DYNAMIC)
#	if defined(_WIN32)
#		define SKULD_EXPORT __declspec(dllimport)
#	else
#		define SKULD_EXPORT  __attribute__ ((visibility("default")))
#	endif
#else
#	define SKULD_EXPORT
#endif

#if defined(SKULD_PLUGIN) && defined(SKULD_BUILD_DYNAMIC)
#	if defined(_WIN32)
#		define SKULD_PLUGIN_EXPORT __declspec(dllexport)
#	else
#		define SKULD_PLUGIN_EXPORT __attribute__ ((visibility("default")))
#	endif
#else
#	define SKULD_PLUGIN_EXPORT
#endif

#if defined(SKULD_BUILD_DYNAMIC) && defined(SKULD_EXTRA)
#	if defined(_WIN32)
#		define SKULD_EXTRA_EXPORT __declspec(dllexport)
#	else
#		define SKULD_EXTRA_EXPORT __attribute__ ((visibility("default")))
#	endif
#elif defined(SKULD_USE_DYNAMIC)
#	if defined(_WIN32)
#		define SKULD_EXTRA_EXPORT __declspec(dllimport)
#	else
#		define SKULD_EXTRA_EXPORT
#	endif
#else
#	define SKULD_EXTRA_EXPORT
#endif

#include <stdint.h>
#include <stddef.h>