#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

@interface WindowDelegate : NSObject<NSWindowDelegate>{

};
- (BOOL) windowShouldClose:(NSNotification *)notification;
@end

@implementation WindowDelegate
- (BOOL) windowShouldClose:(NSNotification *)notification
{
    [NSApp terminate:self];
    return YES;
}
@end

@interface MacOSDisplayable : NSObject{
    NSWindow* _window;
    WindowDelegate* _delegate;
}
+ (MacOSDisplayable*) CreateMacOSDisplayable:(NSRect)rc;
- (MacOSDisplayable*) initWithWindow :(NSWindow*)Window Delegate:(WindowDelegate*)delegate;
- (void) release;
- (NSWindow*) windowHandle;
@end

@implementation MacOSDisplayable
- (NSWindow*) windowHandle{
    return _window;
}
- (void) release{
    [_window release];
    [_delegate release];
}
+ (MacOSDisplayable*) CreateMacOSDisplayable:(NSRect)rc{
    NSUInteger style = NSTitledWindowMask | NSResizableWindowMask | NSClosableWindowMask;
    NSBackingStoreType mBackingStoreType = NSBackingStoreBuffered;
    NSWindow* win = [[NSWindow alloc] initWithContentRect:rc
        styleMask:style backing:mBackingStoreType defer:NO];
    [win setTitle:@"HelloWorld"];
    WindowDelegate* delegate = [[WindowDelegate alloc] init];
    [win setDelegate:delegate];
    [win makeKeyAndOrderFront:win];
    [win makeMainWindow];
    MacOSDisplayable* ret = [[MacOSDisplayable alloc] initWithWindow:win Delegate:delegate];
    return ret;
}
- (MacOSDisplayable*) initWithWindow :(NSWindow*)Window Delegate:(WindowDelegate*)delegate{
    self = [super init];
    _window = Window;
    _delegate = delegate;
    return self;
}
@end

#include <Skuld/UI/Displayable.h>
#include <WindowInfo.h>

namespace Skuld
{
	namespace UI
	{
		class macOSDisplayable : public Displayable
		{
		protected:
			virtual ~macOSDisplayable();
		public:
			MacOSDisplayable* window;
			WindowInfo mInfo;
			static NSApplication* app;
			static bool looped;

			macOSDisplayable(MacOSDisplayable* window) :window(window) {
				mInfo.window = [window windowHandle];
			}
			uint32_t GetWidth();
			uint32_t GetHeight();
			const WindowInfo* GetWindowInfo() const;
			bool MainLoop();
		};

		Displayable* Displayable::Create(uint32_t width, uint32_t height)
		{
			if (macOSDisplayable::app == nullptr) macOSDisplayable::app = [NSApplication sharedApplication];
			return new macOSDisplayable([MacOSDisplayable CreateMacOSDisplayable:NSMakeRect(0, 0, width, height)]);
		}

		uint32_t macOSDisplayable::GetWidth()
		{
			NSRect rc = [[window windowHandle] contentLayoutRect];
			return rc.size.width;
		}

		uint32_t macOSDisplayable::GetHeight()
		{
			NSRect rc = [[window windowHandle] contentLayoutRect];
			return rc.size.height;
		}

		NSApplication* macOSDisplayable::app = nullptr;
		bool macOSDisplayable::looped = false;

		bool macOSDisplayable::MainLoop()
		{
			if (macOSDisplayable::looped == false)
				[NSApp run];
			return false;
		}

		macOSDisplayable::~macOSDisplayable()
		{
			[window release];
		}

		const WindowInfo* macOSDisplayable::GetWindowInfo() const
		{
			return &mInfo;
		}

	}
}