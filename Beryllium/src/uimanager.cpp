#include "uimanager.h"

UIManager::UIManager()
{
    m_textureIdStartNormal = LoadTexture("sprites/button_start_game_normal.png");
    m_textureIdStartHovered = LoadTexture("sprites/button_start_game_hovered.png");
    m_textureIdStartActive = LoadTexture("sprites/button_start_game_active.png");
    m_textureIdTryAgainNormal = LoadTexture("sprites/button_try_again_normal.png");
    m_textureIdTryAgainHovered = LoadTexture("sprites/button_try_again_hovered.png");
    m_textureIdTryAgainActive = LoadTexture("sprites/button_try_again_active.png");
}

UIManager::~UIManager()
{
    glDeleteTextures(1, &m_textureIdStartNormal);
    glDeleteTextures(1, &m_textureIdStartHovered);
    glDeleteTextures(1, &m_textureIdStartActive);
    glDeleteTextures(1, &m_textureIdTryAgainNormal);
    glDeleteTextures(1, &m_textureIdTryAgainHovered);
    glDeleteTextures(1, &m_textureIdTryAgainActive);
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
            DisplayButton(UI::MainMenu, "Start Game", (ImTextureID)(intptr_t)m_textureIdStartNormal);
        if (key == UI::GameOver && value.isButtonShown)
            DisplayButton(UI::GameOver, "Try Again", (ImTextureID)(intptr_t)m_textureIdTryAgainNormal);
    }
}

void UIManager::DisplayButton(UI ui, const char *buttonName, ImTextureID textureId)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 buttonSize = ImVec2(210.0f, 62.0f);

    ImVec2 buttonPos = ImVec2(
        (windowSize.x - buttonSize.x) / 2.0f,
        (windowSize.y - buttonSize.y) / 2.0f);

    // Save current style
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 originalColor = style.Colors[ImGuiCol_Button];
    ImVec4 originalHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
    ImVec4 originalActiveColor = style.Colors[ImGuiCol_ButtonActive];
    float originalFrameRounding = style.FrameRounding;
    float originalFrameBorderSize = style.FrameBorderSize;

    // Set new style
    style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0);        // Transparent background
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0); // Transparent hovered background
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);  // Transparent active background
    style.FrameRounding = 0.0f;                                // No rounding
    style.FrameBorderSize = 0.0f;                              // No border

    ImGui::SetCursorPos(buttonPos);

    bool isHovered = ImGui::IsItemHovered();
    bool isActive = ImGui::IsItemActive();

    m_UIs[ui].isButtonActive = ImGui::ImageButton(textureId, buttonSize);

    if (isHovered)
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    else
        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

    // Restore original style
    style.Colors[ImGuiCol_Button] = originalColor;
    style.Colors[ImGuiCol_ButtonHovered] = originalHoveredColor;
    style.Colors[ImGuiCol_ButtonActive] = originalActiveColor;
    style.FrameRounding = originalFrameRounding;
    style.FrameBorderSize = originalFrameBorderSize;
}