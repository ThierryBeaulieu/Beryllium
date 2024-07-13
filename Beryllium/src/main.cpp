#include <GLFW/glfw3.h>

#include <stdio.h>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <thread>

#include "engine.h"
#include "constants.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "scoremanager.h"

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char **)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1175, 675, "Snake", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != nullptr);

    float constexpr fimageWidth = static_cast<float>(g_ImageWidth);
    float constexpr fimageHeight = static_cast<float>(g_ImageHeight);
    size_t constexpr imageSize = g_ImageWidth * g_ImageHeight;
    uint32_t *imageData = nullptr;

#ifdef _WIN32
    // Windows platform
    imageData = static_cast<uint32_t *>(_aligned_malloc(imageSize * sizeof(uint32_t), 32));
    if (!imageData)
    {
        fprintf(stderr, "Failed to allocate aligned memory.\n");
        return 1;
    }
#else
    // macOS and other Unix-like platforms
    if (posix_memalign(reinterpret_cast<void **>(&imageData), 32, imageSize * sizeof(uint32_t)) != 0)
    {
        fprintf(stderr, "Failed to allocate aligned memory.\n");
        return 1;
    }
#endif

    Engine engine(imageData, window);
    std::chrono::duration<double> dt = std::chrono::milliseconds(1000) / 60.0;
    auto previous = std::chrono::steady_clock::now();
    std::chrono::duration<double> accumulator = std::chrono::milliseconds(0);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GameState gameState;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        std::chrono::microseconds updateUs;
        std::chrono::microseconds renderUs;

        const std::chrono::time_point<std::chrono::high_resolution_clock> current = std::chrono::steady_clock::now();
        const std::chrono::duration<double> frame = current - previous;
        previous = current;

        engine.Update(dt);
        engine.Render();

        float lineHeight = ImGui::GetTextLineHeightWithSpacing();
        float windowPadding = 10.0f; // Padding around the text

        {
            bool showScoreWindow = true;
            ImGui::SetNextWindowSize(ImVec2(210.0f, lineHeight + windowPadding), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(fimageWidth + 50.0f, 20.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Current Score", &showScoreWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
            ScoreManager &scoreManager = ScoreManager::GetInstance();
            ImGui::Text("Current Score : %d", scoreManager.GetPlayerScore());
            ImGui::End();
        }

        {
            bool showScoreWindow = true;
            ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            ScoreManager &scoreManager = ScoreManager::GetInstance();
            std::vector<Score> highScores = scoreManager.GetHighScores();

            int nbScoresToDisplay = highScores.size() < 5 ? highScores.size() : 5;

            int minHeight = 0;
            if (highScores.size() == 0)
            {
                minHeight = 1;
            }
            else if (highScores.size() > 5)
            {
                minHeight = 5;
            }
            else
            {
                minHeight = highScores.size();
            }

            float windowHeight = (minHeight + 1) * lineHeight + windowPadding * 2; // +1 for "Leader board" text

            ImGui::SetNextWindowSize(ImVec2(210.0f, windowHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(fimageWidth + 50.0f, 60.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));

            ImGui::Begin("Highest scores", &showScoreWindow, windowFlags);
            ImGui::Text("Leader board");
            ImGui::Separator();

            if (highScores.size() == 0)
            {
                ImGui::Text("No score available");
            }
            else
            {
                for (int i = 0; i < nbScoresToDisplay; ++i)
                {
                    const auto &score = highScores[i];
                    std::string scoreText = score.GetFirstName() + " " + score.GetLastName() + " : " + std::to_string(score.GetValue());
                    ImGui::Text("%s", scoreText.c_str());
                }
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetWindowSize(window, &display_w, &display_h);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
