#include "D3D11Ctx.hpp"

namespace graphic {
    bool D3D11::initializeDX11(Window* ovr) {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = ovr->getWidth();
        sd.BufferDesc.Height = ovr->getHeight();
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = ovr->getOHwnd();
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL ft;
        HRESULT hrDeviceAndSwapChain = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &m_sc, &m_dxDevice, &ft, &m_dxDeviceCtx);
        if (FAILED(hrDeviceAndSwapChain)) return false;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer = nullptr;
        HRESULT hrSwapChain = m_sc->GetBuffer(0, IID_PPV_ARGS(&buffer));
        if (FAILED(hrSwapChain)) return false;

        HRESULT hrDevice = m_dxDevice->CreateRenderTargetView(buffer.Get(), nullptr, &m_dxRTV);
        if (FAILED(hrDevice)) return false;

        return m_dxRTV != nullptr;
    }
}