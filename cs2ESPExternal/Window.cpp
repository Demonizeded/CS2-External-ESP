#include "Window.hpp"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam
);

namespace graphic {
	typedef BOOL(WINAPI* SWDA)(HWND, DWORD); 
	
	struct WindowSearch {
		DWORD pid;
		HWND hwnd;
	};

	Window::~Window(){
		if (m_OHwnd) {
			DestroyWindow(m_OHwnd);
			m_OHwnd = nullptr;
		}
		UnregisterClassW(wc.lpszClassName, wc.hInstance);
	}

	LRESULT CALLBACK Window::Wndproc(HWND OHwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (uMsg == WM_CLOSE) {
			PostQuitMessage(0);
			return 0;
		}
		
		LRESULT result = ImGui_ImplWin32_WndProcHandler(OHwnd, uMsg, wParam, lParam);
		if (result != 0) return result;

		return DefWindowProcW(OHwnd, uMsg, wParam, lParam);
	}

	bool Window::createOverlay() {
		wc = {};
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.hInstance = GetModuleHandleW(NULL);
		wc.lpszClassName = L"NotepadExtendedv8";
		wc.lpfnWndProc = Window::Wndproc;
		wc.style = 0;

		RegisterClassExW(&wc);

		m_OHwnd = CreateWindowExW(
			WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
			L"NotepadExtendedv8",
			L"NotepadExtended",
			WS_POPUP,
			0, 0,
			getWidth(), getHeight(),
			nullptr, nullptr,
			wc.hInstance,
			nullptr
		);

		if (!m_OHwnd) return false;

		MARGINS margins = { -1, -1, -1, -1 };
		DwmExtendFrameIntoClientArea(m_OHwnd, &margins);

		if (!SetLayeredWindowAttributes(m_OHwnd, 0, 255, LWA_ALPHA)) return false;
		if (!setStreamProof()) return false;

		ShowWindow(m_OHwnd, SW_SHOW);
		UpdateWindow(m_OHwnd);

		return true;
	}

	bool Window::setStreamProof() {
		auto fn = (SWDA)(GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetWindowDisplayAffinity"));
		if (fn) fn(m_OHwnd, 0x11);
		else return false;
		return true;
	}

	void Window::switchTransparent() {
		LONG_PTR windowStyles = GetWindowLongPtrW(Window::getOHwnd(), GWL_EXSTYLE);

		if (config::showMenu) windowStyles &= ~WS_EX_TRANSPARENT;
		else windowStyles |= WS_EX_TRANSPARENT;
	
		SetWindowLongPtrW(Window::getOHwnd(),GWL_EXSTYLE, windowStyles);
		SetWindowPos(Window::getOHwnd(), nullptr, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

	bool Window::checkMessages() {
		MSG msg;
		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return false;
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		return true;
	}

	HWND Window::getWindowByPID(DWORD targetPid) const {
		WindowSearch search = { targetPid, nullptr };

		EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
			auto* s = reinterpret_cast<WindowSearch*>(lParam);
			DWORD windowPid = 0;
			GetWindowThreadProcessId(hwnd, &windowPid);

			if (windowPid == s->pid && IsWindowVisible(hwnd)) {
				char className[128];
				if (GetClassNameA(hwnd, className, sizeof(className)) && strstr(className, "SDL")) {
					s->hwnd = hwnd;
					return FALSE;
				}
			}
			return TRUE;
			}, reinterpret_cast<LPARAM>(&search));

		return search.hwnd;
	}

	void Window::setGHWND(HWND hwnd) {
		m_GHwnd = hwnd;
	}
}
