#pragma once
#include <Windows.h>
#include <dwmapi.h>
#include "config.hpp"

#pragma comment(lib, "dwmapi.lib")

namespace graphic {

	class Window{
		HWND m_OHwnd = nullptr;
		HWND m_GHwnd = nullptr;
		WNDCLASSEXW wc = {};
	public:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window() = default;
		~Window();

		static LRESULT CALLBACK Wndproc(HWND OHwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool createOverlay();
		bool setStreamProof();
		void switchTransparent();

		bool checkMessages();
		HWND getWindowByPID(DWORD targetPid) const;

		HWND getOHwnd() const { return m_OHwnd; }
		HWND getGHwnd() const { return m_GHwnd; }

		void setGHWND(HWND hwnd);

		static int getWidth() { return GetSystemMetrics(SM_CXSCREEN); }
		static int getHeight() { return GetSystemMetrics(SM_CYSCREEN); }
	};
}