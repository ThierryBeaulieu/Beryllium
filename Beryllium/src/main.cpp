#include <stdio.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <thread>

#include "engine.h"
#include "constants.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
    GLFWwindow *window = glfwCreateWindow(1600, 980, "Snake", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float constexpr fimageWidth = static_cast<float>(g_imageWidth);
    float constexpr fimageHeight = static_cast<float>(g_imageHeight);
    size_t constexpr imageSize = g_imageWidth * g_imageHeight;
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

    GameUI gameState;

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
        accumulator += frame;

        std::chrono::time_point<std::chrono::high_resolution_clock> const beginUpdate = std::chrono::high_resolution_clock::now();
        while (accumulator >= dt)
        {
            engine.Update(gameState, dt);
            accumulator -= dt;
        }

        std::chrono::time_point<std::chrono::high_resolution_clock> const endUpdate = std::chrono::high_resolution_clock::now();
        updateUs = std::chrono::duration_cast<std::chrono::microseconds>(endUpdate - beginUpdate);

        std::chrono::time_point<std::chrono::high_resolution_clock> const beginRender = std::chrono::high_resolution_clock::now();
        engine.Render(gameState);
        std::chrono::time_point<std::chrono::high_resolution_clock> const endRender = std::chrono::high_resolution_clock::now();

        renderUs = std::chrono::duration_cast<std::chrono::seconds>(endRender - beginRender);

        {
            float constexpr ema_coefficient = 0.05f;
            float const updateMs = static_cast<float>(updateUs.count()) / 1000.0f;
            float const renderMs = static_cast<float>(renderUs.count()) / 1000.0f;

            bool showPerformanceWindow = true;
            ImGui::SetNextWindowPos(ImVec2(fimageWidth + 30.0f, 20.0f), ImGuiCond_Once, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Performance", &showPerformanceWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
            ImGui::Text("Average Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
