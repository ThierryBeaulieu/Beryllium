#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"

enum class UI
{
    BeginGame,
    GameOver,
    None,
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

    void DisplayUI(const UI &ui)
    {
        ImVec2 imageStartPos = ImGui::GetItemRectMin();
        ImVec2 imageEndPos = ImGui::GetItemRectMax();
        ImVec2 imageSize = ImVec2(imageEndPos.x - imageStartPos.x, imageEndPos.y - imageStartPos.y);

        ImVec2 buttonSize = ImVec2(168.0f, 68.0f);

        ImVec2 buttonPos = ImVec2(
            imageStartPos.x + (imageSize.x - buttonSize.x) / 2.0f,
            imageStartPos.y + (imageSize.y - buttonSize.y) / 2.0f);

        ImGui::SetCursorPos(buttonPos);

        const char *buttonContent = "Start Game";

        if (ImGui::Button(buttonContent, buttonSize))
        {
            // m_gameManager.StartGame();
        }
    }

    void
    RemoveUI(const UI &ui)
    {
    }

private:
    UIManager()
    {
    }

    ~UIManager()
    {
    }
};
