#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "imgui.h"

enum class UI
{
    MainMenu,
    GameOver,
    None,
};

struct ButtonState
{
    bool isButtonShown;
    bool isButtonActive;
};

class UIManager
{
public:
    static UIManager &GetInstance()
    {
        static UIManager m_instance;
        return m_instance;
    }

    UIManager(const UIManager &) = delete;
    UIManager &operator=(const UIManager &) = delete;

    bool &DisplayUI(const UI &ui)
    {
        m_UIs[ui].isButtonShown = true;
        return m_UIs[ui].isButtonActive;
    }

    void RemoveUI(const UI &ui)
    {
        m_UIs[ui].isButtonActive = false;
        m_UIs[ui].isButtonShown = false;
    }

    void RenderUIs()
    {
        for (const auto &[key, value] : m_UIs)
        {
            if (key == UI::MainMenu && value.isButtonShown)
                DisplayButton(UI::MainMenu, "Start Game");
            if (key == UI::GameOver && value.isButtonShown)
                DisplayButton(UI::GameOver, "Game Over");
        }
    }

private:
    void DisplayButton(UI ui, const char *buttonName)
    {
        ImVec2 imageStartPos = ImGui::GetItemRectMin();
        ImVec2 imageEndPos = ImGui::GetItemRectMax();
        ImVec2 imageSize = ImVec2(imageEndPos.x - imageStartPos.x, imageEndPos.y - imageStartPos.y);

        ImVec2 buttonSize = ImVec2(168.0f, 68.0f);

        ImVec2 buttonPos = ImVec2(
            imageStartPos.x + (imageSize.x - buttonSize.x) / 2.0f,
            imageStartPos.y + (imageSize.y - buttonSize.y) / 2.0f);

        ImGui::SetCursorPos(buttonPos);

        if (ImGui::Button(buttonName, buttonSize))
        {
            m_UIs[ui].isButtonActive = true;
        }
    }
    UIManager()
    {
    }

    ~UIManager()
    {
    }

    std::map<UI, ButtonState> m_UIs;
};
