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
    ${CMAKE_SOURCE_DIR}/src/soundbuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/sounddevice.cpp
    ${CMAKE_SOURCE_DIR}/src/soundsource.cpp
)

# Add header files for the main project
set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/engine.h
    ${CMAKE_SOURCE_DIR}/src/gamemanager.h
    ${CMAKE_SOURCE_DIR}/src/soundbuffer.h
    ${CMAKE_SOURCE_DIR}/src/sounddevice.h
    ${CMAKE_SOURCE_DIR}/src/soundsource.h
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
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# Link the ImGui and GLFW library to the main executable
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL)
target_link_libraries(${PROJECT_NAME} PRIVATE sndfile)

# Specify additional include directories for the main executable
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/ImGui)

# https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)
                        
# Link OpenGL (platform-dependent)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/wood_impact.ogg ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds/wood_impact.ogg COPYONLY)

set(SOUNDS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/sounds)
file(MAKE_DIRECTORY ${SOUNDS_OUTPUT_DIR})
configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/wood_impact.ogg ${SOUNDS_OUTPUT_DIR}/wood_impact.ogg COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/power_up.wav ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds/power_up.wav COPYONLY)
set(SOUNDS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/sounds)
file(MAKE_DIRECTORY ${SOUNDS_OUTPUT_DIR})
configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/power_up.wav ${SOUNDS_OUTPUT_DIR}/power_up.wav COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/death_sound.wav ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds/death_sound.wav COPYONLY)
set(SOUNDS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/sounds)
file(MAKE_DIRECTORY ${SOUNDS_OUTPUT_DIR})
configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/death_sound.wav ${SOUNDS_OUTPUT_DIR}/death_sound.wav COPYONLY)


configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/begin_sound.wav ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds/begin_sound.wav COPYONLY)
set(SOUNDS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/sounds)
file(MAKE_DIRECTORY ${SOUNDS_OUTPUT_DIR})
configure_file(${CMAKE_SOURCE_DIR}/assets/sounds/begin_sound.wav ${SOUNDS_OUTPUT_DIR}/begin_sound.wav COPYONLY)