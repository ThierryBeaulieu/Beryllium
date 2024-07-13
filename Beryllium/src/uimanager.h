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
    AskingForPlayerName,
    None,
};

struct UserFormData
{
    char firstName[128] = "";
    char lastName[128] = "";
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

    bool &DisplayUI(const UI &ui);
    void RemoveUI(const UI &ui);

    void RenderUIs();
    GLuint LoadTexture(const char *filename);

    UserFormData GetDataForm();

private:
    void DisplayButton(UI ui, const char *buttonName, ImTextureID textureId);
    void DisplayInputField(UI ui);

    UIManager();
    ~UIManager();

    std::map<UI, ButtonState> m_UIs;
    GLuint m_textureIdStartNormal;
    GLuint m_textureIdStartHovered;
    GLuint m_textureIdStartActive;
    GLuint m_textureIdTryAgainNormal;
    GLuint m_textureIdTryAgainHovered;
    GLuint m_textureIdTryAgainActive;
    UserFormData m_UserData;
};
