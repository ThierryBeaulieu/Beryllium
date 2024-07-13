#include "engine.h"

Engine::Engine(uint32_t *imageData, GLFWwindow *window)
    : m_ImageData(imageData), m_GameManager(GameManager()), m_PixelWidth(30), m_PaddingWidth(2), m_Window(window)
{
    glGenTextures(1, &m_ImageTexture);
    int nbPixelsWidth = (g_ImageWidth - (g_ImageWidth % m_PixelWidth)) / m_PixelWidth;
    int nbPixelsHeight = (g_ImageHeight - (g_ImageHeight % m_PixelWidth)) / m_PixelWidth;

    m_GameManager.SetGridHeight(nbPixelsHeight);
    m_GameManager.SetGridWidth(nbPixelsWidth);
}

void Engine::RenderSquare(int x, int y, uint32_t color)
{
    for (int i = 0; i < m_PixelWidth; ++i)
    {
        for (int j = 0; j < m_PixelWidth; ++j)
        {
            if (i < m_PaddingWidth || i > (m_PixelWidth - m_PaddingWidth) ||
                j < m_PaddingWidth || j > (m_PixelWidth - m_PaddingWidth))
                continue;
            SetPixel(x + i, y + j, color);
        }
    }
}

void Engine::RenderBackground(uint32_t color)
{
    for (int i = 0; i < g_ImageWidth; ++i)
    {
        for (int j = 0; j < g_ImageHeight; ++j)
        {
            SetPixel(i, j, color);
        }
    }
}

void Engine::HandleInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        m_GameManager.SetDirectionUp();
    }
    else if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        m_GameManager.SetDirectionDown();
    }
    else if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        m_GameManager.SetDirectionLeft();
    }
    else if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        m_GameManager.SetDirectionRight();
    }
}

void Engine::Render()
{
    uint32_t backgroundColor = Engine::GetColorBlack(255);
    RenderBackground(backgroundColor);

    uint32_t tileColor = Engine::GetColorWhite(200);

    for (int i = 0; i < m_GameManager.GetGridWidth(); ++i)
    {
        for (int j = 0; j < m_GameManager.GetGridHeight(); ++j)
        {
            RenderSquare(i * m_PixelWidth, j * m_PixelWidth, tileColor);
        }
    }

    uint32_t foodColor = Engine::GetColorRed(255);
    for (std::pair<int, int> foodCoord : m_GameManager.GetFoodPosition())
    {
        RenderSquare(foodCoord.first * m_PixelWidth, foodCoord.second * m_PixelWidth, foodColor);
    }

    uint32_t snakeColor = Engine::GetColorWhite(255);
    for (std::pair<int, int> snakeCoord : m_GameManager.GetSnakePosition())
    {
        RenderSquare(snakeCoord.first * m_PixelWidth, snakeCoord.second * m_PixelWidth, snakeColor);
    }

    bool showWindow = true;

    glBindTexture(GL_TEXTURE_2D, m_ImageTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_ImageWidth, g_ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ImageData);

    ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Render", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

    ImGui::Image((void *)(intptr_t)m_ImageTexture, ImVec2(g_ImageWidth, g_ImageHeight));

    UIManager &uiManager = UIManager::GetInstance();

    uiManager.RenderUIs();

    ImGui::End();
}

void Engine::Update(const std::chrono::duration<double> &elapsedTime)
{
    HandleInput();
    m_GameManager.Update(elapsedTime);
}

void Engine::SetPixel(int x, int y, uint32_t color)
{
    m_ImageData[g_ImageWidth * y + x] = color;
}

uint32_t Engine::GetPixel(int x, int y)
{
    return m_ImageData[g_ImageWidth * y + x];
}

bool Engine::ImageButtonWithTextures(ImTextureID defaultTexture, ImTextureID hoverTexture, ImTextureID pressedTexture, const ImVec2 &size)
{
    ImGuiIO &io = ImGui::GetIO();
    ImGui::PushID((void *)(intptr_t)defaultTexture);

    ImGui::BeginGroup();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    ImTextureID textureToUse = defaultTexture;
    if (ImGui::IsItemActive())
        textureToUse = defaultTexture;
    else if (ImGui::IsItemActivated())
        textureToUse = pressedTexture;
    else if (ImGui::IsItemHovered())
        textureToUse = hoverTexture;

    bool pressed = ImGui::ImageButton(textureToUse, size, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), 1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGui::PopStyleColor(3);
    ImGui::EndGroup();
    ImGui::PopID();

    return pressed;
}
