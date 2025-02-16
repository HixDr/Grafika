cmake_minimum_required(VERSION 3.10)

# Project name
project(SphereLighting)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# Include directories
include_directories(src)

# Add source files
set(SOURCES
    src/main.cpp
    src/Sphere.cpp
    src/Shader.cpp
    src/Model.cpp 
    src/Texture.cpp 
    src/Camera.cpp 
)

# Add shader files (optional for IDE visibility)
set(SHADERS
    src/shaders/vertex_shader.glsl
    src/shaders/fragment_shader.glsl
    src/shaders/sphere_vertex.glsl
    src/shaders/sphere_fragment.glsl
)

# Add assets directory (optional for IDE visibility)
set(ASSETS
    assets/head.tga
    assets/eye.tga
    assets/woman1.obj
    assets/woman1.mtl
    assets/Black.png
)

# Find OpenGL, GLEW, and GLFW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::GLEW glfw)

# Copy assets to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
