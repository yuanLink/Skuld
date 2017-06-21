#include <string.h>
#include <map>
#include <Skuld/UI/Displayable.h>
#include <Windows.h>
#include <WindowInfo.h>

namespace Skuld
{
	namespace UI
	{
		class WindowsDisplayable : public Displayable
		{
		protected:
			virtual ~WindowsDisplayable();
		private:
			WindowInfo mInfo;
			static uint32_t counter;
		protected:
			WindowsDisplayable(HWND hWnd) : Displayable() { mInfo.hWnd = hWnd; }
		public:
			static WindowsDisplayable* Create(uint32_t width, uint32_t height);
			uint32_t GetWidth();
			uint32_t GetHeight();
			bool MainLoop();
			LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
			const WindowInfo* GetWindowInfo() const;
		};

		uint32_t WindowsDisplayable::counter = 0;
		static std::map<HWND, WindowsDisplayable*> __handlemap;

		HMODULE GetCurrentModule()
		{
			MEMORY_BASIC_INFORMATION mbi;
			static int dummy;
			VirtualQuery(&dummy, &mbi, sizeof(mbi));
			return reinterpret_cast<HMODULE>(mbi.AllocationBase);
		}

		LRESULT WindowsDisplayable::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			return DefWindowProcW(this->mInfo.hWnd, message, wParam, lParam);
		}

		WindowsDisplayable * WindowsDisplayable::Create(uint32_t width, uint32_t height)
		{
			HINSTANCE hInstance = GetCurrentModule();
			if (WindowsDisplayable::counter == 0)
			{
				WNDCLASSEXW wndClass = { 0 };
				wndClass.cbSize = sizeof(WNDCLASSEXW);
				wndClass.style = CS_HREDRAW | CS_VREDRAW;
				wndClass.lpfnWndProc = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
				{
					switch (message)
					{
					case WM_KEYDOWN:
					case WM_KEYUP:
					case WM_LBUTTONDBLCLK:
					case WM_LBUTTONDOWN:
					case WM_LBUTTONUP:
					case WM_MOUSEMOVE:
					case WM_RBUTTONDBLCLK:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONUP:
					case WM_MBUTTONDBLCLK:
					case WM_MBUTTONDOWN:
					case WM_MBUTTONUP:
					case WM_MOUSEWHEEL:
					case WM_MOUSEHWHEEL:
						return __handlemap[hwnd]->WndProc(message, wParam, lParam);
					case WM_DESTROY:
						PostQuitMessage(0);
						return (LRESULT)0;
					default:
						return DefWindowProcW(hwnd, message, wParam, lParam);
					}
				};
				wndClass.hInstance = hInstance;
				wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
				wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				wndClass.lpszMenuName = nullptr;
				wndClass.lpszClassName = L"SystemDisplayable";
				wndClass.hIcon = nullptr;
				if (!RegisterClassExW(&wndClass)) return nullptr;
			}
			counter++;
			HWND hwnd = CreateWindowW(L"SystemDisplayable", L"test",
				WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, width,
				height, nullptr, nullptr, hInstance, nullptr);
			WindowsDisplayable* ret = new (std::nothrow) WindowsDisplayable(hwnd);
			__handlemap[hwnd] = ret;
			ShowWindow(hwnd, SW_SHOW);
			return ret;
		}

		WindowsDisplayable::~WindowsDisplayable()
		{
			__handlemap[mInfo.hWnd] = nullptr;
			DestroyWindow(mInfo.hWnd);
			counter--;
			if (counter == 0)
			{
				UnregisterClassW(L"SystemDisplayable", GetCurrentModule());
			}
		}

		const WindowInfo* WindowsDisplayable::GetWindowInfo() const
		{
			return &mInfo;
		}

		uint32_t WindowsDisplayable::GetWidth()
		{
			RECT rect;
			GetClientRect(mInfo.hWnd, &rect);
			return rect.right - rect.left;
		}

		uint32_t WindowsDisplayable::GetHeight()
		{
			RECT rect;
			GetClientRect(mInfo.hWnd, &rect);
			return rect.bottom - rect.top;
		}

		bool WindowsDisplayable::MainLoop()
		{
			static MSG msg = { 0 };
			if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
			return msg.message == WM_QUIT;
		}

		Displayable * Displayable::Create(uint32_t width, uint32_t height)
		{
			return WindowsDisplayable::Create(width, height);
		}
	}
}