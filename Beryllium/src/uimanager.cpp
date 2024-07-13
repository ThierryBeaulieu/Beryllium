#include "uimanager.h"

UIManager::UIManager()
{
    m_textureIdStart = LoadTexture("sprites/button_start_light_red.png");
    m_textureIdTryAgain = LoadTexture("sprites/button_start_dark_red.png");
}

UIManager::~UIManager()
{
    glDeleteTextures(1, &m_textureIdStart);
    glDeleteTextures(1, &m_textureIdTryAgain);
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

GLuint UIManager::LoadTexture(const char *filename)
{
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return textureID;
}

void UIManager::RenderUIs()
{
    for (const auto &[key, value] : m_UIs)
    {
        if (key == UI::MainMenu && value.isButtonShown)
            DisplayButton(UI::MainMenu, "Start Game", (ImTextureID)(intptr_t)m_textureIdStart);
        if (key == UI::GameOver && value.isButtonShown)
            DisplayButton(UI::GameOver, "Try Again", (ImTextureID)(intptr_t)m_textureIdTryAgain);
    }
}

void UIManager::DisplayButton(UI ui, const char *buttonName, ImTextureID textureID)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 buttonSize = ImVec2(168.0f, 68.0f);

    ImVec2 buttonPos = ImVec2(
        (windowSize.x - buttonSize.x) / 2.0f,
        (windowSize.y - buttonSize.y) / 2.0f);

    ImGui::SetCursorPos(buttonPos);
    m_UIs[ui].isButtonActive = ImGui::ImageButton(textureID, buttonSize);
}