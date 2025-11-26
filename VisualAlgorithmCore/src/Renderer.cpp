//
// Created by Ushio on 2025-11-16.
//

#include "Renderer.h"

Renderer::Renderer()
{
    UINT device_flags = 0;
#ifdef _DEBUG
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    // D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,deviceFlags, nullptr, 0,
    // D3D11_SDK_VERSION, ,swapChain.GetAddressOf(), );
}
