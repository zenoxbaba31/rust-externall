#pragma once
#include <sstream>
#include <filesystem>
#include <fstream>
#include <thread>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <tchar.h>
#include <unordered_map>
#include "Fonts.h"
#include "json.hpp"
void Configsave(std::string CONFIG);
void Configload(std::string CONFIG);
#include "imgui_settings.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <dxgi.h>
#include <dwmapi.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "dwmapi.lib")

#include "skCrypt.h"
#include "xorstr.hpp"
#include "auth.hpp"
#include "globals.h"
#include "math.h"
#include "drawfunctions.h"

#include "driver.h"
#include "mainclasses.h"
#include "entityloop.hpp"

std::string Game_Location;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CleanupRenderTarget();
void CreateRenderTarget();


void DefaultValues()
{

#ifdef SAINT
    main_color = ImColor(66, 245, 191, 255); // TEAL
    main_brand = _memdup(skCrypt("Primitive Software"));
    filecode = _memdup(skCrypt("186515"));
    name = _memdup(skCrypt(""));
    ownerid = _memdup(skCrypt(""));
    secret = _memdup(skCrypt(""));
    version = _memdup(skCrypt("1.0"));
#endif

    url = _memdup(skCrypt(""));
    KeyAuthApp = api(name, ownerid, secret, version, url);

    for (int i = 0; i < 100; i++) {
        globals::EspColor[i] = RGBA(255, 255, 255, 255);
    }
    globals::keybind[Aimbot] = 0x06;

    globals::featuresint[AimFov] = 30;
    globals::featuresint[FovChanger] = 90;
    globals::featuresint[TimeChanger] = 12;

    globals::features[DroppedItemsImage] = true;

    globals::EspColor[PlayerEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[CorpseEsp] = RGBA(255, 0, 0, 255);
    globals::EspColor[ScientistEsp] = RGBA(255, 128, 0, 255);
    globals::EspColor[TeamEsp] = RGBA(0, 255, 0, 255);
    globals::EspColor[Wounded] = RGBA(255, 0, 0, 255);
    globals::EspColor[StoneEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[SulfurEsp] = RGBA(255, 255, 0, 255);
    globals::EspColor[MetalEsp] = RGBA(120, 120, 120, 255);

    globals::EspColor[CollectableStoneEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[CollectableSulfurEsp] = RGBA(255, 255, 0, 255);
    globals::EspColor[CollectableMetalEsp] = RGBA(120, 120, 120, 255);

    globals::EspColor[HempEsp] = RGBA(0, 255, 0, 255);
    globals::EspColor[MilitaryCrateEsp] = RGBA(0, 120, 0, 255);
    globals::EspColor[SupplyDropEsp] = RGBA(255, 255, 128, 255);
    globals::EspColor[AutoTurretEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[ShotgunTrapEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[BoatEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[FlameTurretEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[ShotgunTrapEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[BearTrapEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[BoatEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[SamSiteEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[CarEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[LandMineEsp] = RGBA(0, 255, 255, 255);
    globals::EspColor[ToolCupboardEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[BoarEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[BearEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[WolfEsp] = RGBA(255, 255, 255, 255);
    globals::EspColor[NormalCrate] = RGBA(255, 255, 0, 255);
    globals::EspColor[EliteCrate] = RGBA(0, 128, 255, 255);
    globals::EspColor[PatrolHeli] = RGBA(255, 0, 0, 255);
    globals::EspColor[MinicopterEsp] = RGBA(255, 128, 0, 255);
    globals::EspColor[DroppedItems] = RGBA(0, 255, 255, 255);
}

#include "imgui_freetype.h"


class c_texture
{
public:
    ID3D11ShaderResourceView* texture = nullptr;
    std::string item_name = "";
};

class c_texture_cache
{
private:
    std::vector<c_texture> texture_cache{};

    ID3D11ShaderResourceView* create_texture(ID3D11Device* device, std::string name)
    {

        std::string image_path = Game_Location + (std::string)skCrypt("\\Bundles\\items\\") + name; // todo: make path dynamic

        ID3D11ShaderResourceView* tex = nullptr;

        if (D3DX11CreateShaderResourceViewFromFileA(device, image_path.c_str(), nullptr, nullptr, &tex, nullptr) != S_OK)
            return nullptr;

        return tex;
    }

public:

    c_texture get_texture(std::string item_name)
    {
        for (auto& texture : texture_cache)
            if (texture.item_name == item_name)
                return texture; // check if the texture's already cached

        c_texture ret;
        ret.item_name = item_name;
        ret.texture = create_texture(g_pd3dDevice, item_name + (std::string)skCrypt(".png"));

        if (!ret.texture)
            return ret; // this issue sorts itself out

        texture_cache.push_back(ret); // put the new texture into the cache

        return ret; // return the texture
    }

}; inline c_texture_cache texture_cache;


ImVec2 WindowPos;
ImVec2 WindowPosSize;
ImVec2 WindowPos2;
float scale = 1.0f;
const float ImageSize = 30;
const float ImageSpace = 70;
void playerinfo(std::string targetname, Cloth test[6], Belt test2[7], const std::string& test3)
{
    if (test2[0].Name.empty() && test2[1].Name.empty() && test2[2].Name.empty() && test2[3].Name.empty() && test2[4].Name.empty() && test2[5].Name.empty() && test2[6].Name.empty() &&
        test[0].Name.empty() && test[1].Name.empty() && test[2].Name.empty() && test[3].Name.empty() && test[4].Name.empty() && test[5].Name.empty() && test[6].Name.empty() && test[7].Name.empty()) {
        return;
    }

    const float scaledImageSize = ImageSize * scale;
    const float scaledImageSpace = ImageSpace * scale;
    const float draw_offset = WindowPos.y - (70 * scale) - scaledImageSize;
    const ImVec2 rect_start = { WindowPos.x - scaledImageSize - 70 * scale, draw_offset };
    const ImVec2 rect_end = { WindowPos.x + scaledImageSize + 6 * scaledImageSpace - 70 * scale, draw_offset + 2 * scaledImageSize };
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    bool isDrawn = false;
    ImGui::GetBackgroundDrawList()->AddRectFilled(WindowPos2, ImVec2(WindowPos2.x + WindowPosSize.x, WindowPos2.y + (20 * scale)), ImColor(20, 20, 20, 180));
    DrawString(15 * scale, WindowPos2.x + 10, WindowPos2.y + 2, ImColor(255, 255, 255), false, false, targetname.c_str());

    for (int i = 1; i <= 7; ++i)
    {
        const ImVec2 draw_start = { rect_start.x + (i - 1) * scaledImageSpace, rect_start.y };
        const ImVec2 draw_end = { rect_start.x + i * scaledImageSpace - 2 * scale, rect_end.y };
        const c_texture& texture = texture_cache.get_texture(test[i - 1].Name.c_str());

        if (texture.texture)
        {
            draw_list->AddImage(reinterpret_cast<void*>(texture.texture), draw_start, draw_end, ImVec2(0, 0), ImVec2(1, 1));

            if (test[i - 1].Health != 0)
            {
                ImVec2 draw_end4 = draw_end;
                draw_end4.x -= 63 * scale;
                float health_ratio = test[i - 1].Health / test[i - 1].MaxHealth;
                ImVec2 health_bar_end2 = draw_start;
                health_bar_end2.y += (draw_end4.y - draw_start.y) * (1.0f - health_ratio);

                draw_list->AddRectFilled(draw_start, draw_end4, ImColor(111, 190, 66, 40), 1 * scale);
                draw_list->AddRectFilled(health_bar_end2, draw_end4, ImColor(111, 190, 66, 255), 1 * scale);
            }
        }

        draw_list->AddRect(draw_start, draw_end, ImColor(120, 120, 120, 190), 2 * scale);

        if (i != 7)
        {
            const float draw_offset2 = WindowPos.y - scaledImageSize;
            const ImVec2 rect_start2 = { WindowPos.x - scaledImageSize - 40 * scale, draw_offset2 };
            const ImVec2 rect_end2 = { WindowPos.x + scaledImageSize + 5 * scaledImageSpace - 40 * scale, draw_offset2 + 2 * scaledImageSize };
            const ImVec2 draw_start2 = { rect_start2.x + (i - 1) * scaledImageSpace, rect_start2.y };
            const ImVec2 draw_end2 = { rect_start2.x + i * scaledImageSpace - 2 * scale, rect_end2.y };
            const c_texture& texture2 = texture_cache.get_texture(test2[i - 1].Name.c_str());

            bool is_match = (!test3.empty() && test2[i - 1].Name == test3);
            bool has_health = (test2[i - 1].Health != 0);

            if (texture2.texture && !is_match)
            {
                draw_list->AddImage(reinterpret_cast<void*>(texture2.texture), draw_start2, draw_end2, ImVec2(0, 0), ImVec2(1, 1));

                if (is_match)
                {
                    if (!isDrawn) {
                        draw_list->AddRectFilled(draw_start2, draw_end2, ImColor(50, 80, 160, 140), 2 * scale);
                        isDrawn = true; // Mark that the block has been drawn
                    }
                    draw_list->AddRect(draw_start2, draw_end2, ImColor(120, 120, 120, 190), 2 * scale);

                    if (texture2.texture)
                    {
                        draw_list->AddImage(reinterpret_cast<void*>(texture2.texture), draw_start2, draw_end2, ImVec2(0, 0), ImVec2(1, 1));
                    }
                }

                if (has_health)
                {
                    ImVec2 draw_end3 = draw_end2;
                    draw_end3.x -= 63 * scale;
                    float health_ratio = test2[i - 1].Health / test2[i - 1].MaxHealth;
                    ImVec2 health_bar_end = draw_start2;
                    health_bar_end.y += (draw_end3.y - draw_start2.y) * (1.0f - health_ratio);

                    draw_list->AddRectFilled(draw_start2, draw_end3, ImColor(111, 190, 66, 40), 1 * scale);
                    draw_list->AddRectFilled(health_bar_end, draw_end3, ImColor(111, 190, 66, 255), 1 * scale);
                }

                if (test2[i - 1].Count > 1)
                {
                    float text_x = draw_end2.x - 12 * scale;
                    float text_y = draw_end2.y - 4 * scale;
                    DrawString(20 * scale, text_x, text_y, ImColor(255, 255, 255), true, false, (std::string("x") + std::to_string((int)test2[i - 1].Count)).c_str());
                }
                if (test2[i - 1].WeaponAmmo > 0)
                {
                    float text_x = draw_end2.x - 12 * scale;
                    float text_y = draw_end2.y - 4 * scale;
                    DrawString(20 * scale, text_x, text_y, ImColor(255, 255, 255), true, false, std::to_string((int)test2[i - 1].WeaponAmmo).c_str());
                }
            }

            if (!is_match)
            {
                draw_list->AddRect(draw_start2, draw_end2, ImColor(120, 120, 120, 190), 2 * scale);
            }

        }
    }
}
#include "menu.h"
#include "drawcheat.hpp"


void CreateOverlay()
{
    WNDCLASSEXA wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, skCrypt("RKS SRV"), nullptr };
    ::RegisterClassExA(&wc);
    HWND hwnd = ::CreateWindowA(wc.lpszClassName, skCrypt("RKS SRV"), WS_POPUP, 0, 0, globals::ScreenSize.right, globals::ScreenSize.bottom, nullptr, nullptr, wc.hInstance, nullptr);
    SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);

    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    UpdateWindow(hwnd);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImFontConfig config;

    ImFontConfig config2;
    config.MergeMode = false; //     

    config2.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Bold; //    

    config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_LightHinting; //    
    config.OversampleH = 25; //    
    config.OversampleV = 25; //    
    config.PixelSnapH = true; //     

    io.Fonts->AddFontFromMemoryTTF(&NunitoMedium, sizeof NunitoMedium, 21, &config, io.Fonts->GetGlyphRangesCyrillic());

    medium_font = io.Fonts->AddFontFromMemoryTTF(&NunitoMedium, sizeof NunitoMedium, 18, &config, io.Fonts->GetGlyphRangesCyrillic());

    small_font = io.Fonts->AddFontFromMemoryTTF(&NunitoMedium, sizeof NunitoMedium, 17, &config, io.Fonts->GetGlyphRangesCyrillic());

    icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 18, &config, io.Fonts->GetGlyphRangesCyrillic());

    icon_big_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 23, &config, io.Fonts->GetGlyphRangesCyrillic());

    logo_font = io.Fonts->AddFontFromMemoryTTF(&NunitoMedium, sizeof NunitoMedium, 25, &config2, io.Fonts->GetGlyphRangesCyrillic());

    small_icon_font = io.Fonts->AddFontFromMemoryTTF(&NunitoMedium, sizeof NunitoMedium, 15, &config, io.Fonts->GetGlyphRangesCyrillic());

    arrow_icons = io.Fonts->AddFontFromMemoryTTF(&arrowicon, sizeof arrowicon, 18, &config, io.Fonts->GetGlyphRangesCyrillic());

    ImFontConfig fontConfig;
    main_font = io.Fonts->AddFontFromFileTTF(skCrypt("C:\\Windows\\Fonts\\Arial.ttf"), 14.0f, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImGuiStyle& s = ImGui::GetStyle();

    s.FramePadding = ImVec2(10, 10);
    s.ItemSpacing = ImVec2(10, 5);
    s.FrameRounding = 5.f;
    s.WindowBorderSize = 0.f;
    s.PopupBorderSize = 0.f;
    s.WindowPadding = ImVec2(0, 0);
    s.ChildBorderSize = 0.0001f;
    s.Colors[ImGuiCol_WindowBg] = winbg_color;
    s.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 0.f);
    s.WindowShadowSize = 0;
    s.PopupRounding = 5.f;
    s.PopupBorderSize = 1.3f;
    s.ScrollbarSize = 4.f;
    s.ScrollbarRounding = 10.f;

    bool done = false;
    for (;;) {
        HWND GameWnd = FindWindow(skCrypt("UnityWndClass"), skCrypt("Rust"));
        if (!GameWnd)
            done = true;

        RECT rc;
        POINT xy;
        ZeroMemory(&rc, sizeof(RECT));
        ZeroMemory(&xy, sizeof(POINT));
        GetClientRect(hwnd, &rc);
        ClientToScreen(hwnd, &xy);
        rc.left = xy.x;
        rc.top = xy.y;

        ImGuiIO& io = ImGui::GetIO();
        io.ImeWindowHandle = GameWnd;
        io.DeltaTime = 1.0f / 60.0f;

        POINT p;
        GetCursorPos(&p);
        io.MousePos.x = p.x - xy.x;
        io.MousePos.y = p.y - xy.y;
        if (GetAsyncKeyState(0x1)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].x = io.MousePos.y;
        }
        else io.MouseDown[0] = false;

        HWND hwnd_active = GetForegroundWindow();
        if (hwnd_active == GameWnd)
        {
            HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
            ::SetWindowPos(hwnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            char fpsinfo[256];
            sprintf_s(fpsinfo, (skCrypt("FPS %.3f").decrypt()), ImGui::GetIO().Framerate);
            DrawString(20, 150, 150, ImColor(255, 255, 255), true, true, fpsinfo);

            if (globals::features[AimFov] && (globals::features[Aimbot] || globals::features[Silent])) ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(globals::ScreenMidSize.right, globals::ScreenMidSize.bottom), globals::featuresint[AimFov], (ImColor)globals::EspColor[AimFov], 96, 1);
            if (globals::features[Crosshair])
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(globals::ScreenMidSize.right + 1.0f + 1, globals::ScreenMidSize.bottom - 1), ImVec2(globals::ScreenMidSize.right + 4.0f + 1.0f + 1, globals::ScreenMidSize.bottom + 1), (ImColor)globals::EspColor[Crosshair]);
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(globals::ScreenMidSize.right - 1.0f - 1, globals::ScreenMidSize.bottom + 1), ImVec2(globals::ScreenMidSize.right - 4.0f - 1.0f - 1, globals::ScreenMidSize.bottom - 1), (ImColor)globals::EspColor[Crosshair]);
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(globals::ScreenMidSize.right - 1, globals::ScreenMidSize.bottom + 1.0f + 1), ImVec2(globals::ScreenMidSize.right + 1, globals::ScreenMidSize.bottom + 4.0f + 1.0f + 1), (ImColor)globals::EspColor[Crosshair]);
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(globals::ScreenMidSize.right + 1, globals::ScreenMidSize.bottom - 1.0f - 1), ImVec2(globals::ScreenMidSize.right - 1, globals::ScreenMidSize.bottom - 4.0f - 1.0f - 1), (ImColor)globals::EspColor[Crosshair]);
            }
            if (globals::features[PlayerInfo] && globals::showmenu)
            {
                s.WindowRounding = 0.f;
                ImGui::SetNextWindowSize(ImVec2(487 * scale, 5 * scale));
                ImGui::Begin(skCrypt(" PLAYERINFO"), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                ImVec2 xy = ImGui::GetWindowPos();
                ImVec2 xeye = ImGui::GetWindowSize();
                ImGui::Text(skCrypt("PLAYER INFO"));

                WindowPosSize = ImGui::GetWindowSize();
                WindowPos2 = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 10 * scale);
                WindowPos = ImVec2(ImGui::GetWindowPos().x + 100 * scale, ImGui::GetWindowPos().y + 130 * scale);
                ImGui::End();

                float draw_offset = WindowPos.y - (70 * scale) - ImageSize * scale;
                ImVec2 rect_start = { WindowPos.x - ImageSize * scale - 70 * scale, draw_offset };
                ImVec2 rect_end = { WindowPos.x + ImageSize * scale + 6 * ImageSpace * scale - 70 * scale, draw_offset + 2 * ImageSize * scale };

                float draw_offset2 = WindowPos.y - ImageSize * scale;
                ImVec2 rect_start2 = { WindowPos.x - ImageSize * scale - 40 * scale, draw_offset2 };
                ImVec2 rect_end2 = { WindowPos.x + ImageSize * scale + 5 * ImageSpace * scale - 40 * scale, draw_offset2 + 2 * ImageSize * scale };

                for (int i = 1; i <= 7; ++i) {
                    ImVec2 draw_start = { rect_start.x + (i - 1) * ImageSpace * scale, rect_start.y };
                    ImVec2 draw_end = { rect_start.x + i * ImageSpace * scale - 2 * scale, rect_end.y };
                    ImGui::GetBackgroundDrawList()->AddRectFilled(draw_start, draw_end, ImColor(120, 120, 120, 120));

                    if (i != 7) {
                        ImVec2 draw_start2 = { rect_start2.x + (i - 1) * ImageSpace * scale, rect_start2.y };
                        ImVec2 draw_end2 = { rect_start2.x + i * ImageSpace * scale - 2 * scale, rect_end2.y };
                        ImGui::GetBackgroundDrawList()->AddRectFilled(draw_start2, draw_end2, ImColor(120, 120, 120, 120));
                    }
                }
            }

            ImGui::PushFont(main_font);
            DrawCheat();
            ImGui::PopFont();

            if (GetAsyncKeyState(VK_INSERT) & 1) { globals::showmenu = !globals::showmenu; }

            if (globals::showmenu)
                DrawMenu(s);
        }
        ImGui::Render();

        const float clear_color_with_alpha[4] = { 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(0, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassA(wc.lpszClassName, wc.hInstance);

    return;
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}


bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = globals::ScreenSize.right;
    sd.BufferDesc.Height = globals::ScreenSize.bottom;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


using json = nlohmann::json;

std::string ReadFromJson(std::string path, std::string section)
{
    if (!std::filesystem::exists(path))
        return skCrypt("File Not Found").decrypt();
    std::ifstream file(path);
    json data = json::parse(file);
    return data[section];
}

bool CheckIfJsonKeyExists(std::string path, std::string section)
{
    if (!std::filesystem::exists(path))
        return skCrypt("File Not Found").decrypt();
    std::ifstream file(path);
    json data = json::parse(file);
    return data.contains(section);
}

bool WriteToJson(std::string path, std::string name, std::string value, bool userpass, std::string name2, std::string value2)
{
    json file;
    if (!userpass)
    {
        file[name] = value;
    }
    else
    {
        file[name] = value;
        file[name2] = value2;
    }

    std::ofstream jsonfile(path, std::ios::out);
    jsonfile << file;
    jsonfile.close();
    if (!std::filesystem::exists(path))
        return false;

    return true;
}

bool DriverStopped(std::string camper)
{
    SC_HANDLE scm_handle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    SC_HANDLE service_handle = OpenServiceA(scm_handle, camper.c_str(), SERVICE_QUERY_STATUS);
    SERVICE_STATUS service_status;
    QueryServiceStatus(service_handle, &service_status);
    if (service_status.dwCurrentState == SERVICE_RUNNING)
    {
        CloseServiceHandle(service_handle);
        CloseServiceHandle(scm_handle);
        return true;
    }
    else
    {
        CloseServiceHandle(service_handle);
        CloseServiceHandle(scm_handle);
        return false;
    }
}

void SetWindowSettings(int x, int y)
{
    const HWND hwnd = GetConsoleWindow();
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(handle, &info);
    const COORD new_size = { info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1 };
    MoveWindow(hwnd, 0, 0, x, y, FALSE);
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    RECT window_pos;
    window_pos.right = ((float)desktop.right / 2 - 550 / 2);
    window_pos.bottom = ((float)desktop.bottom / 2 - 400 / 2);
    const LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, lStyle & ~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX));
    const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &info);
    const COORD scrollbar = { info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1 };
    if (console == 0)
        throw 0;
    SetConsoleScreenBufferSize(console, scrollbar);
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, 245, LWA_ALPHA);
    CONSOLE_CURSOR_INFO info2{ 100, false };
    SetConsoleCursorInfo(handle, &info2);
    SetWindowPos(hwnd, HWND_NOTOPMOST, window_pos.right, window_pos.bottom, x, y, SWP_NOSIZE | SWP_NOZORDER);
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    const HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    mode &= ~(ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
    SetConsoleMode(hStdin, mode);
    if (DriverStopped((std::string)skCrypt("wdfilter")))
    {
        std::cout << skCrypt("CLOSE WINDOWS DEFENDER!") << std::endl;
        Sleep(5000);
        exit(0);
    }
}


struct Config {
    bool features[globals::MaxFeatures] = { false };
    int featuresint[globals::MaxFeatures] = { 0 };  
    int keybind[globals::MaxFeatures] = { 0 };
    ImColor EspColor[globals::MaxFeatures];         
    int bChamsmaterial = 0;                

    // JSON'a dönüþtürme
    json to_json() const {
        json j;
        for (int i = 0; i < globals::MaxFeatures; ++i) {
            j["features"][i] = features[i];
            j["featuresInt"][i] = featuresint[i];
            j["colorValues"][i] = { EspColor[i].Value.x, EspColor[i].Value.y, EspColor[i].Value.z, EspColor[i].Value.w };
        }
        j["bChamsmaterial"] = bChamsmaterial;
        return j;
    }

    // JSON'dan yükleme
    static Config from_json(const json& j) {
        Config config;
        for (int i = 0; i < globals::MaxFeatures; ++i) {
            config.features[i] = j["features"][i].get<bool>();
            config.featuresint[i] = j["featuresInt"][i].get<int>();
            auto colorArray = j["colorValues"][i].get<std::array<float, 4>>();
            config.EspColor[i] = ImColor(colorArray[0], colorArray[1], colorArray[2], colorArray[3]);
        }
        config.bChamsmaterial = j["bChamsmaterial"].get<int>();
        return config;
    }
};

void saveConfig(const Config& config, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << config.to_json().dump(4);
        file.close();
    }
}

Config loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    json j;

    if (file.is_open()) {
        file >> j;
        file.close();
        return Config::from_json(j);
    }
    else {
        return Config();
    }
}

void Configsave(std::string CONFIG)
{
    Config config;
    for (int i = 0; i < globals::MaxFeatures; i++) {
        config.features[i] = globals::features[i];
        config.keybind[i] = globals::keybind[i];
        config.featuresint[i] = globals::featuresint[i];
        config.EspColor[i] = globals::EspColor[i];
    }
    config.bChamsmaterial = globals::bChamsmaterial;
    saveConfig(config, CONFIG);
}

void Configload(std::string CONFIG)
{
    Config config = loadConfig(CONFIG);
    for (int i = 0; i < globals::MaxFeatures; i++) {
        globals::features[i] = config.features[i];
        globals::keybind[i] = config.keybind[i];
        globals::featuresint[i] = config.featuresint[i];
        globals::EspColor[i] = config.EspColor[i];
    }
    globals::bChamsmaterial = config.bChamsmaterial;
}
