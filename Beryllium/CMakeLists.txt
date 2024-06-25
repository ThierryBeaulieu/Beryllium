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
    ${CMAKE_SOURCE_DIR}/src/soundmanager.cpp
)

# Add header files for the main project
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/engine.h
    ${CMAKE_SOURCE_DIR}/src/gamemanager.h
    ${CMAKE_SOURCE_DIR}/src/soundmanager.h
)

# Include directories (avoid writing it every time)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/vendor/GFLW/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor/AL/include)
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

# Link the GLFW to ImGui
target_link_libraries(ImGui PRIVATE glfw)

# Add the main executable
add_executable(Beryllium ${SRC_FILES} ${HEADER_FILES})

# Link the ImGui and GLFW library to the main executable
target_link_libraries(Beryllium PRIVATE ImGui)
target_link_libraries(Beryllium PRIVATE glfw)
target_link_libraries(Beryllium PRIVATE OpenAL)

# Specify additional include directories for the main executable
target_include_directories(Beryllium PRIVATE ${CMAKE_SOURCE_DIR}/vendor/ImGui)

# Link OpenGL (platform-dependent)
find_package(OpenGL REQUIRED)
target_link_libraries(Beryllium PRIVATE OpenGL::GL)