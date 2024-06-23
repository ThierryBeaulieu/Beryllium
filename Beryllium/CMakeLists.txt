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
#     │   ├── include/
#     │   │   └── GLFW/
#     │   │       ├── glfw3.h
#     │   │       └── glfw3native.h
#     │   └── lib-vc2022/
#     │       └── glfw3.lib
#     └── ImGui/
#         ├── imgui_demo.cpp
#         ├── imgui_draw.cpp
#         ├── imgui_impl_glfw.cpp
#         ├── imgui_impl_glfw.h
#         └── imgui_widgets.cpp


cmake_minimum_required(VERSION 3.10)
project(Beryllium)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files for the main project
set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/engine.cpp
    ${CMAKE_SOURCE_DIR}/src/gamemanager.cpp
)

# Add header files for the main project
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/engine.h
    ${CMAKE_SOURCE_DIR}/src/gamemanager.h
)

# Include directories (avoid writing it every time)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/vendor/GFLW/include)
include_directories(${CMAKE_SOURCE_DIR}/vendor/ImGui)

# Add ImGui as a static library
add_library(ImGui STATIC
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/ImGui/imgui_impl_glfw.cpp
)

# Include GLFW headers for ImGui
target_include_directories(ImGui PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/include)
target_link_libraries(ImGui PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib-vc2022/glfw3.lib)

# Add the main executable
add_executable(Beryllium ${SRC_FILES} ${HEADER_FILES})

# Link the ImGui and GLFW library to the main executable
target_link_libraries(Beryllium PRIVATE ImGui)
target_link_libraries(Beryllium PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib-vc2022/glfw3.lib)

# Specify additional include directories for the main executable
target_include_directories(Beryllium PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(Beryllium PRIVATE ${CMAKE_SOURCE_DIR}/vendor/ImGui)