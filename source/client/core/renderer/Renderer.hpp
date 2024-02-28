#pragma once

#ifndef TRILOGY_RENDERER_H
#define TRILOGY_RENDERER_H

#include <kiero.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_stdlib.h>

#include <d3d11.h>

#include <renderer/Render.hpp>


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



class Renderer : public Singleton<Renderer> {
private:
    using renderer_present_t = int32_t(__stdcall*)(IDXGISwapChain*, uint32_t, uint32_t);
    using renderer_resize_t = int32_t(__stdcall*)(IDXGISwapChain*, uint32_t, FLOAT, FLOAT, DXGI_FORMAT, uint32_t);
    using renderer_wndproc_t = int64_t(__stdcall*)(HWND, uint32_t, WPARAM, LPARAM);
public:
    bool is_web_initialized = false;
    bool focus_browser = false;

    renderer_present_t o_present;
    renderer_resize_t o_resize_buffers;
    renderer_wndproc_t o_wnd_proc;


    HWND window = NULL;

    IDXGISwapChain* d3d11_swapchain = NULL;
    ID3D11Device* d3d11_device = NULL;
    ID3D11DeviceContext* d3d11_device_context = NULL;
    ID3D11RenderTargetView* d3d11_render_target;

    std::once_flag is_initialized;

    void initialize();
    void shutdown();

    void initialize_imgui();
};
#endif
