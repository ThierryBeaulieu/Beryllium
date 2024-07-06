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
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
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

    // Load the button texture
    GLuint buttonTexture;
    int buttonWidth, buttonHeight;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Render", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

    // todo bind another texture but on top of the first one?

    Shader ourShader("shaders/shader.vs", "shaders/shader.fs");

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("assets/sprites/start_button.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    // background texture
    glBindTexture(GL_TEXTURE_2D, m_imageTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_imageWidth, g_imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);

    // add the button texture here

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Render", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Image((void *)(intptr_t)m_imageTexture, ImVec2(g_imageWidth, g_imageHeight));
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
