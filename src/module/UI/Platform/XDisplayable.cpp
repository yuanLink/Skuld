#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Skuld/UI/Displayable.h>
#include <WindowInfo.h>

namespace Skuld
{
	namespace UI
	{
		class XDisplayable : public Displayable
		{
		protected:
			virtual ~XDisplayable();
		private:
			WindowInfo mInfo;
			uint32_t width, height;
		protected:
			XDisplayable(Display* display, Window window, uint32_t width, uint32_t height) :
				width(width), height(height), Displayable() {
				mInfo.display = display;
				mInfo.window = window;
			}
		public:
			static XDisplayable* Create(uint32_t width, uint32_t height);
			uint32_t GetWidth();
			uint32_t GetHeight();
			bool MainLoop();
			const WindowInfo* GetWindowInfo() const;
		};

		XDisplayable * XDisplayable::Create(uint32_t width, uint32_t height)
		{
			Display* display = XOpenDisplay(NULL);
			XSetWindowAttributes attributes;
			memset(&attributes, 0, sizeof(attributes));
			attributes.background_pixel = XWhitePixel(display, 0);
			Window window = XCreateWindow(display, XDefaultRootWindow(display), 0, 0, width, height, 2,
				DefaultDepth(display, 0), InputOutput, DefaultVisual(display, 0), CWBackPixel, &attributes);
			XSelectInput(display, window, 0xffffff);
			XMapWindow(display, window);
			XFlush(display);


			Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
			XSetWMProtocols(display, window, &wmDeleteWindow, 1);
			return new XDisplayable(display, window, width, height);
		}

		const WindowInfo* XDisplayable::GetWindowInfo() const
		{
			return &mInfo;
		}

		XDisplayable::~XDisplayable()
		{
			XCloseDisplay(mInfo.display);
		}

		uint32_t XDisplayable::GetWidth()
		{
			return this->width;
		}

		uint32_t XDisplayable::GetHeight()
		{
			return this->height;
		}

		bool XDisplayable::MainLoop()
		{
			XEvent event;
			static bool exit_loop = false;
			while (XPending(mInfo.display))
			{
				XNextEvent(mInfo.display, &event);
				switch (event.type)
				{
				case KeyRelease:
					if (event.xkey.keycode == XKeysymToKeycode(mInfo.display, XK_Escape))
						XDestroyWindow(mInfo.display, mInfo.window);
					break;
				case DestroyNotify:
					exit_loop = true;
					break;
				case ClientMessage:
					if (event.xclient.data.l[0] == XInternAtom(mInfo.display, "WM_DELETE_WINDOW", False))
						exit_loop = true;
					break;
				default:
					break;
				}
			}
			return exit_loop;
		}

		Displayable * Displayable::Create(uint32_t width, uint32_t height)
		{
			return XDisplayable::Create(width, height);
		}
	}
}