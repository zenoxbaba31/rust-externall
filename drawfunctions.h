#pragma once

inline ImFont* main_font = nullptr;

void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ImDrawList* drawList = ImGui::GetBackgroundDrawList(), ...)
{
    va_list va_alist;
    char buf[1024] = { 0 };
    va_start(va_alist, pText);
    _vsnprintf_s(buf, sizeof(buf), pText, va_alist);
    va_end(va_alist);
    std::string text = buf;
    if (bCenter)
    {
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        ImFont* font = main_font;
        ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text.c_str());
        offsetX = textSize.x * 0.5f;
        offsetY = textSize.y;
        x -= offsetX;
        y -= offsetY;
    }
    if (stroke)
    {
        drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImColor(25, 25, 25, 255), text.c_str());
        drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImColor(25, 25, 25, 255), text.c_str());
        drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImColor(25, 25, 25, 255), text.c_str());
        drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImColor(25, 25, 25, 255), text.c_str());
    }
    drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), color, text.c_str());
}

void DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, int width)
{
    //ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x.x + 1, x.y + 1), ImVec2(y.x + 1, y.y + 1), ImColor(25, 25, 25, 255), width);
    //ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x.x - 1, x.y - 1), ImVec2(y.x - 1, y.y - 1), ImColor(25, 25, 25, 255), width);
    ImGui::GetBackgroundDrawList()->AddLine(x, y, color, width);
}

void DrawRect(int x, int y, int w, int h, ImColor color, int thickness)
{
    //ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x + 1, y + 1), ImVec2(x + w + 1, y + h + 1), ImColor(25, 25, 25, 255), 0, 0, thickness);
    //ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w - 1, y + h - 1), ImColor(25, 25, 25, 255), 0, 0, thickness);
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
}

auto Draw2DBox(float x, float y, float w, float h, ImColor color, int thickness) -> void
{
    DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, thickness); // top 
    DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + thickness), color, thickness); // left
    DrawLine(ImVec2(x + w, y - 1.3f), ImVec2(x + w, y + h + thickness), color, thickness);  // right
    DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, thickness);   // bottom 
}

void DrawCorneredBox(int X, int Y, int W, int H, ImColor color, int thickness)
{
    float lineW = (W / 3);
    float lineH = (H / 3);
    DrawLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
    DrawLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
    DrawLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
    DrawLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
    DrawLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
    DrawLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
    DrawLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
    DrawLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
}
