#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif defined(__MACOSX__)
#import <Cocoa/Cocoa.h>
#elif defined(__ANDROID__)
#include <android/native_window_jni.h>
#elif defined(__linux__) && !defined(__ANDROID__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#else
#error "Unknown Platform"
#endif

namespace Skuld
{
	struct WindowInfo
	{
#ifdef _WIN32
		HWND hWnd;
#elif defined(__MACOSX__)
		NSWindow* window;
#elif defined(__ANDROID__)
		ANativeWindow* window;
#elif defined(__linux__)
		Display* display;
		Window window;
#endif
	};
}