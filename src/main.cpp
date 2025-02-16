#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Model.h"
#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// Global variables for time management
float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f;    // Time of last frame
float orbitRadius = 5.0f;  // Radius of the light's orbital path

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OBJ Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Configure mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Hide and capture cursor
    glfwSetCursorPosCallback(window, mouse_callback);  // Set callback for mouse movement

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Create and compile shaders
    Shader shader("../src/shaders/vertex_shader.glsl", "../src/shaders/fragment_shader.glsl");          // Main shader for the model
    Shader sphereShader("../src/shaders/sphere_vertex.glsl", "../src/shaders/sphere_fragment.glsl");    // Shader for the light sphere
    
    // Load 3D model and create sphere
    Model womanModel("assets/woman1.obj", "assets/woman1.mtl");  // Load the woman model with its material
    Sphere sphere(20, 20);  // Create a sphere for the light source
    sphere.setupSphere();   // Setup sphere's VAO, VBO, and EBO
    
    float rotationSpeed = 0.5f;  // Speed of light's orbital rotation

    // Enable OpenGL features
    glEnable(GL_DEPTH_TEST);   // Enable depth testing
    glEnable(GL_CULL_FACE);    // Enable face culling
    glCullFace(GL_BACK);       // Cull back faces

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Update frame timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Camera movement controls
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(GLFW_KEY_W, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(GLFW_KEY_S, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(GLFW_KEY_A, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(GLFW_KEY_D, deltaTime);
        
        // Light rotation speed controls
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) rotationSpeed += 0.05f;  // Increase rotation speed
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) rotationSpeed -= 0.05f;  // Decrease rotation speed

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // Dark gray background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create view and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Draw the orbiting light sphere
        sphereShader.use();
        static float orbitAngle = 0.0f;
        orbitAngle += rotationSpeed * deltaTime;  // Update orbit angle
        
        // Calculate light position in orbit
        glm::vec3 lightPos(orbitRadius * cos(orbitAngle), 0.0f, orbitRadius * sin(orbitAngle));

        // Set uniforms for sphere shader
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);
        sphereShader.setVec3("objectColor", glm::vec3(1.0f));  // White color for light source

        // Draw sphere in wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::mat4 sphereModel = glm::mat4(1.0f);
        sphereModel = glm::translate(sphereModel, lightPos);
        sphereModel = glm::scale(sphereModel, glm::vec3(0.5f));  // Scale the sphere
        sphereShader.setMat4("model", sphereModel);
        sphere.Draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw the woman model with lighting
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);  // Camera position for specular lighting
        shader.setVec3("lightColor", glm::vec3(1.0f));  // White light
        shader.setVec3("lightPos", lightPos);  // Update light position
        
        // Set model matrix and draw
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.05f));  // Scale the model down
        shader.setMat4("model", model);
        womanModel.Draw(shader);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &sphere.VAO);
    glDeleteBuffers(1, &sphere.VBO);
    glDeleteBuffers(1, &sphere.EBO);
    glfwTerminate();
    return 0;
}