#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include "Window.hpp"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace graphic {

	class D3D11 {
		Microsoft::WRL::ComPtr<ID3D11Device> m_dxDevice = nullptr;
		Microsoft::WRL::ComPtr < ID3D11DeviceContext> m_dxDeviceCtx = nullptr;
		Microsoft::WRL::ComPtr < ID3D11RenderTargetView> m_dxRTV = nullptr;
		Microsoft::WRL::ComPtr < IDXGISwapChain> m_sc = nullptr;
	public:
		D3D11(const D3D11&) = delete;
		D3D11& operator=(const D3D11&) = delete;

		D3D11() = default;
		~D3D11() = default;

		bool initializeDX11(Window* ovr);

		Microsoft::WRL::ComPtr < ID3D11Device> getDxDevice() const {
			return m_dxDevice;
		}

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDxDeviceCtx() const {
			return m_dxDeviceCtx;
		}

		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> getDxRTV() const {
			return m_dxRTV;
		}

		Microsoft::WRL::ComPtr<IDXGISwapChain> getSc() const {
			return m_sc;
		}
	};
}