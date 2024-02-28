#include "StdInc.h"
#include "StdInc.h"
#include "renderer.hpp"
#include <app/Debug.h>


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

/**
  * Need to split it later into custom imgui classes
  * - or get rid of imgui later & switch to cef/rmlui/ultralight lol
  */
void imgui_inline_text(std::string key, std::string value) {
    ImGui::Text(key.c_str());
    ImGui::SameLine();
    ImGui::Text(value.c_str());
}

HRESULT __stdcall h_renderer_present(IDXGISwapChain* dxgi_swapchain, UINT sync_interval, UINT flags)
{
    static Renderer* renderer = Renderer::GetSingletonPtr();
    static Render* imgui_render = Render::GetSingletonPtr();

    if (dxgi_swapchain)
        renderer->d3d11_swapchain = dxgi_swapchain;

    std::call_once(renderer->is_initialized, [&] {
        if (SUCCEEDED(dxgi_swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&renderer->d3d11_device)))
        {
            renderer->d3d11_device->GetImmediateContext(&renderer->d3d11_device_context);

            DXGI_SWAP_CHAIN_DESC swapchain_desc;
            dxgi_swapchain->GetDesc(&swapchain_desc);

            renderer->window = swapchain_desc.OutputWindow;

            ID3D11Texture2D* d3d11_back_buffer;
            dxgi_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&d3d11_back_buffer);

      
            if (d3d11_back_buffer) {
                renderer->d3d11_device->CreateRenderTargetView(d3d11_back_buffer, NULL, &renderer->d3d11_render_target);
                d3d11_back_buffer->Release();
            }

            imgui_render->initialize(renderer->window, dxgi_swapchain);

            renderer->o_wnd_proc = (WNDPROC)SetWindowLongPtr(renderer->window, GWLP_WNDPROC, (LONG_PTR)h_renderer_wndproc);

        }
        else TSA_LOG_ERR("Failed to create renderer in present hook.");
        });

    imgui_render->begin_scene();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.5f });
    ImGui::Begin("(trilogy-mp): debug");
    {

        ImGui::Text("Nickname:");
    }
    ImGui::End();
    ImGui::PopStyleColor();

    RECT window_size;
    GetClientRect(renderer->window, &window_size);
    // renderer::features::c_dev_chat::instance()->m_chat_messages.push_back({ "", "(c_renderer::h_renderer_present): test", 3000 });


    imgui_render->end_scene();
    imgui_render->render();

    renderer->d3d11_device_context->OMSetRenderTargets(1, &renderer->d3d11_render_target, NULL);


    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    io.WantCaptureMouse = renderer->focus_browser;
    io.MouseDrawCursor = renderer->focus_browser;

    return renderer->o_present(dxgi_swapchain, sync_interval, flags);
}

HRESULT h_renderer_resize(IDXGISwapChain* dxgi_swapchain, UINT buffer_count, FLOAT width, FLOAT height, DXGI_FORMAT new_format, UINT swapchain_flags)
{
    static Renderer* renderer = Renderer::GetSingletonPtr();
    static Render* imgui_render = Render::GetSingletonPtr();


    if (renderer->d3d11_render_target) {
        renderer->d3d11_device_context->OMSetRenderTargets(0, 0, 0);
        renderer->d3d11_render_target->Release();
    }

    // imgui_render->reset(width, height);

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


void Renderer::initialize_imgui()
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);
}

void Renderer::initialize()
{
    /**
      * Kinda ugly, rework?
      */
    bool init_hook = false;
    do
    {
        if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&o_present, h_renderer_present);
            kiero::bind(13, (void**)&o_resize_buffers, h_renderer_resize);

            TSA_LOG_DEBUG("The renderer got initialized.");
            init_hook = true;
        }
    } while (!init_hook);
}

void Renderer::shutdown()
{
    // Useless?
    kiero::shutdown();
}
