# Folder structure

# Beryllium/
# ├── CMakeLists.txt
# ├── src/
# │   ├── engine.cpp
# │   ├── engine.h
# │   ├── gamemanager.cpp
# │   └── gamemanager.h
# └── vendor/
#     ├── GLFW/
#     └── ImGui/
#         ├── imgui_demo.cpp
#         ├── imgui_draw.cpp
#         ├── imgui_impl_glfw.cpp
#         ├── imgui_impl_glfw.h
#         ├── ...
#         └── imgui_widgets.cpp

cmake_minimum_required(VERSION 3.10)
project(Beryllium)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files for the main project
set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/engine.cpp
    ${CMAKE_SOURCE_DIR}/src/gamemanager.cpp
    ${CMAKE_SOURCE_DIR}/src/scoremanager.cpp
    ${CMAKE_SOURCE_DIR}/src/soundbuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/sounddevice.cpp
    ${CMAKE_SOURCE_DIR}/src/soundsource.cpp
    ${CMAKE_SOURCE_DIR}/src/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/src/uimanager.cpp
)

# Add header files for the main project
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/engine.h
    ${CMAKE_SOURCE_DIR}/src/constants.h
    ${CMAKE_SOURCE_DIR}/src/gamemanager.h
    ${CMAKE_SOURCE_DIR}/src/scoremanager.h
    ${CMAKE_SOURCE_DIR}/src/soundbuffer.h
    ${CMAKE_SOURCE_DIR}/src/sounddevice.h
    ${CMAKE_SOURCE_DIR}/src/soundsource.h
    ${CMAKE_SOURCE_DIR}/src/stb_image.h
    ${CMAKE_SOURCE_DIR}/src/uimanager.h
)

# Include directories (avoid writing it every time)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/vendor/GFLW/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor/AL/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor/libsndfile/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor/ImGui)

# Add ImGui as a static library
add_library(ImGui STATIC
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_impl_opengl3.cpp
)

# Add GLFW as a submodule
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/GLFW)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/AL)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/libsndfile)

# Link the GLFW to ImGui
target_link_libraries(ImGui PRIVATE glfw)

# Add the main executable
add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} ${HEADER_FILES})

# Link the ImGui and GLFW library to the main executable
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL)
target_link_libraries(${PROJECT_NAME} PRIVATE sndfile)


# Specify additional include directories for the main executable
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/ImGui)

# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t
    $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)
                        
# Link OpenGL (platform-dependent)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# Copy the assets directory to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)