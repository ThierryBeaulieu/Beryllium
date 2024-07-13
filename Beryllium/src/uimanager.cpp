#include "uimanager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

bool UIManager::DisplayUI(const UI &ui)
{
    m_UIs[ui].isButtonShown = true;
    return m_UIs[ui].isButtonActive;
}

void UIManager::RemoveUI(const UI &ui)
{
    m_UIs[ui].isButtonActive = false;
    m_UIs[ui].isButtonShown = false;
}

void UIManager::RenderUIs()
{
    for (const auto &[key, value] : m_UIs)
    {
        if (key == UI::MainMenu && value.isButtonShown)
            DisplayButton(UI::MainMenu, "Start Game");
        if (key == UI::GameOver && value.isButtonShown)
            DisplayButton(UI::GameOver, "Try Again");
    }
}

void UIManager::DisplayButton(UI ui, const char *buttonName)
{
    ImVec2 imageStartPos = ImGui::GetItemRectMin();
    ImVec2 imageEndPos = ImGui::GetItemRectMax();
    ImVec2 imageSize = ImVec2(imageEndPos.x - imageStartPos.x, imageEndPos.y - imageStartPos.y);

    ImVec2 buttonSize = ImVec2(168.0f, 68.0f);

    ImVec2 buttonPos = ImVec2(
        imageStartPos.x + (imageSize.x - buttonSize.x) / 2.0f,
        imageStartPos.y + (imageSize.y - buttonSize.y) / 2.0f);

    ImGui::SetCursorPos(buttonPos);
    m_UIs[ui].isButtonActive = ImGui::Button(buttonName, buttonSize);
}
