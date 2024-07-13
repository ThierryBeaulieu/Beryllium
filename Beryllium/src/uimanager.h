#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

#include "imgui.h"
#include "stb_image.h"

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

    bool DisplayUI(const UI &ui);
    void RemoveUI(const UI &ui);

    void RenderUIs();
    GLuint LoadTexture(const char *filename);

private:
    void DisplayButton(UI ui, const char *buttonName);

    UIManager();
    ~UIManager();

    std::map<UI, ButtonState> m_UIs;
    GLuint m_textureIdStartNormal;
    GLuint m_textureIdStartHovered;
    GLuint m_textureIdStartActive;
    GLuint m_textureIdTryAgainNormal;
    GLuint m_textureIdTryAgainHovered;
    GLuint m_textureIdTryAgainActive;
};
