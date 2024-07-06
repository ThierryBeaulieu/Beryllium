#include "engine.h"

Engine::Engine(uint32_t *imageData, GLFWwindow *window)
    : m_imageData(imageData), m_gameManager(GameManager()), m_pixelWidth(30), m_paddingWidth(2), m_window(window)
{
    glGenTextures(1, &m_imageTexture);
    int nbPixelsWidth = (g_imageWidth - (g_imageWidth % m_pixelWidth)) / m_pixelWidth;
    int nbPixelsHeight = (g_imageHeight - (g_imageHeight % m_pixelWidth)) / m_pixelWidth;

    m_gameManager.SetGridHeight(nbPixelsHeight);
    m_gameManager.SetGridWidth(nbPixelsWidth);
}

void Engine::RenderSquare(int x, int y, uint32_t color)
{
    for (int i = 0; i < m_pixelWidth; ++i)
    {
        for (int j = 0; j < m_pixelWidth; ++j)
        {
            if (i < m_paddingWidth || i > (m_pixelWidth - m_paddingWidth) ||
                j < m_paddingWidth || j > (m_pixelWidth - m_paddingWidth))
                continue;
            SetPixel(x + i, y + j, color);
        }
    }
}

void Engine::RenderBackground(uint32_t color)
{
    for (int i = 0; i < g_imageWidth; ++i)
    {
        for (int j = 0; j < g_imageHeight; ++j)
        {
            SetPixel(i, j, color);
        }
    }
}

void Engine::RenderUI()
{
    bool showWindow = true;
    for (const UserInterface &ui : m_gameManager.GetUserInterfaces())
    {
        ImGui::SetNextWindowPos(ImVec2(ui.coord_x, ui.coord_y), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
        ImGui::Begin(ui.name, &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
        ImGui::Button("click me", ImVec2(200, 100));
        ImGui::End();
    }
}

void Engine::HandleInput()
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        m_gameManager.SetDirectionUp();
    }
    else if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        m_gameManager.SetDirectionDown();
    }
    else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        m_gameManager.SetDirectionLeft();
    }
    else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        m_gameManager.SetDirectionRight();
    }
    else if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_gameManager.ResetGame();
    }
}

void Engine::Render()
{
    uint32_t backgroundColor = Engine::GetColorBlack(255);
    RenderBackground(backgroundColor);

    uint32_t tileColor = Engine::GetColorWhite(200);

    for (int i = 0; i < m_gameManager.GetGridWidth(); ++i)
    {
        for (int j = 0; j < m_gameManager.GetGridHeight(); ++j)
        {
            RenderSquare(i * m_pixelWidth, j * m_pixelWidth, tileColor);
        }
    }

    uint32_t foodColor = Engine::GetColorRed(255);
    for (std::pair<int, int> foodCoord : m_gameManager.GetFoodPosition())
    {
        RenderSquare(foodCoord.first * m_pixelWidth, foodCoord.second * m_pixelWidth, foodColor);
    }

    uint32_t snakeColor = Engine::GetColorWhite(255);
    for (std::pair<int, int> snakeCoord : m_gameManager.GetSnakePosition())
    {
        RenderSquare(snakeCoord.first * m_pixelWidth, snakeCoord.second * m_pixelWidth, snakeColor);
    }

    bool showWindow = true;

    glBindTexture(GL_TEXTURE_2D, m_imageTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_imageWidth, g_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Render", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Image((void *)(intptr_t)m_imageTexture, ImVec2(g_imageWidth, g_imageHeight));

    // Calculate the center position
    ImVec2 image_center = ImVec2(g_imageWidth * 0.5f, g_imageHeight * 0.5f);
    ImVec2 button_size = ImVec2(158.0f, 68.0f); // Adjust button size as needed
    ImVec2 button_pos = ImVec2(image_center.x - button_size.x * 0.5f, image_center.y - button_size.y * 0.5f);

    // Set cursor position to the calculated center position for the button
    ImGui::SetCursorPos(button_pos);

    GLuint buttonDefaultTexture = LoadTexture("sprites/button_start_dark_red.png");
    GLuint buttonHoverTexture = LoadTexture("sprites/button_start_light_red.png");
    GLuint buttonPressedTexture = LoadTexture("sprites/button_start_dark_red.png");

    if (ImageButtonWithTextures((void *)(intptr_t)buttonDefaultTexture, (void *)(intptr_t)buttonHoverTexture, (void *)(intptr_t)buttonPressedTexture, button_size))
    {
        // Button clicked
    }

    ImGui::End();
}

void Engine::Update()
{
    HandleInput();
    m_gameManager.Update();
}

void Engine::SetPixel(int x, int y, uint32_t color)
{
    m_imageData[g_imageWidth * y + x] = color;
}

uint32_t Engine::GetPixel(int x, int y)
{
    return m_imageData[g_imageWidth * y + x];
}

GLuint Engine::LoadTexture(const char *filename)
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
        textureToUse = pressedTexture;
    else if (ImGui::IsItemHovered())
        textureToUse = hoverTexture;

    bool pressed = ImGui::ImageButton(textureToUse, size);

    ImGui::PopStyleColor(3);
    ImGui::EndGroup();
    ImGui::PopID();

    return pressed;
}
