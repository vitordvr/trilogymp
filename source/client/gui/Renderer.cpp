#include "StdInc.h"
#include "Debug.h"
#include "Renderer.hpp"
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero.h"
#include "imgui.h"
#include "imgui_impl_win32.h"   
#include "imgui_impl_dx11.h"


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall h_renderer_wndproc(const HWND handle, UINT message, WPARAM word_param, LPARAM long_param) {
    static Renderer* renderer = Renderer::GetSingletonPtr();

    if (ImGui::GetIO().MouseDrawCursor)
    {
        if (ImGui_ImplWin32_WndProcHandler(handle, message, word_param, long_param))
            return true;

        return 1l;
    }

    return CallWindowProc(renderer->o_wnd_proc, handle, message, word_param, long_param);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    static Renderer* renderer = Renderer::GetSingletonPtr();
    static Draw* imgui_render = Draw::GetSingletonPtr();


    if (pSwapChain)
        renderer->d3d11_swapchain = pSwapChain;


    std::call_once(renderer->is_initialized, [&] {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&renderer->d3d11_device))) {
            renderer->d3d11_device->GetImmediateContext(&renderer->d3d11_device_context);

            DXGI_SWAP_CHAIN_DESC swapchain_desc;
            pSwapChain->GetDesc(&swapchain_desc);

            renderer->window = swapchain_desc.OutputWindow;

            ID3D11Texture2D* d3d11_back_buffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&d3d11_back_buffer);

            if (d3d11_back_buffer) {
                renderer->d3d11_device->CreateRenderTargetView(d3d11_back_buffer, NULL, &renderer->d3d11_render_target);
                d3d11_back_buffer->Release();
            }

            imgui_render->initialize(renderer->window, pSwapChain);

            renderer->o_wnd_proc = (WNDPROC)SetWindowLongPtr(renderer->window, GWLP_WNDPROC, (LONG_PTR)h_renderer_wndproc);
        }
        else TSA_LOG_CRIT("Failed to create renderer in present hook.");
    });
            

    imgui_render->begin_scene();
    ImGui::Begin("trilogy:mp");
    ImGui::Text("Its TrilogyMP!");
    ImGui::End();



    imgui_render->end_scene();
    imgui_render->render();


    renderer->d3d11_device_context->OMSetRenderTargets(1, &renderer->d3d11_render_target, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    ImGuiIO& io = ImGui::GetIO();

    io.WantCaptureMouse = renderer->focus_browser;
    io.MouseDrawCursor = renderer->focus_browser;

    return oPresent(pSwapChain, SyncInterval, Flags);
}


HRESULT h_renderer_resize(IDXGISwapChain* dxgi_swapchain, UINT buffer_count, FLOAT width, FLOAT height, DXGI_FORMAT new_format, UINT swapchain_flags)
{
    static Renderer* renderer = Renderer::GetSingletonPtr();
    static Draw* imgui_render = Draw::GetSingletonPtr();


    if (renderer->d3d11_render_target) {
        renderer->d3d11_device_context->OMSetRenderTargets(0, 0, 0);
        renderer->d3d11_render_target->Release();
    }

    auto result = renderer->o_resize_buffers(dxgi_swapchain, buffer_count, width, height, new_format, swapchain_flags);


    ID3D11Texture2D* d3d11_surface;
    dxgi_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11_surface);

    if (d3d11_surface) {
        renderer->d3d11_device->CreateRenderTargetView(d3d11_surface, NULL, &renderer->d3d11_render_target);
        d3d11_surface->Release();

        renderer->d3d11_device_context->OMSetRenderTargets(1, &renderer->d3d11_render_target, NULL);

        D3D11_VIEWPORT d3d11_viewport;
        d3d11_viewport.Width = width;
        d3d11_viewport.Height = height;
        d3d11_viewport.MinDepth = 0.0f;
        d3d11_viewport.MaxDepth = 1.0f;
        d3d11_viewport.TopLeftX = 0;
        d3d11_viewport.TopLeftY = 0;

        renderer->d3d11_device_context->RSSetViewports(1, &d3d11_viewport);
    }

    return result;
}



void Renderer::initialize()
{
    bool init_hook = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent);
            kiero::bind(13, (void**)&o_resize_buffers, h_renderer_resize);

            init_hook = true;
            TSA_LOG_DEBUG("The renderer got initialized.");
        }
    } while (!init_hook);
}

void Renderer::shutdown()
{
    // Useless?
    kiero::shutdown();
}
