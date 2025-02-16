#include "Camera.h"
#include <iostream>

// Define global camera instance
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));

Camera::Camera(glm::vec3 position) 
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      MovementSpeed(5.0f),
      MouseSensitivity(0.01f),
      Yaw(-90.0f),
      Pitch(0.0f) {
    Position = position;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(int key, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (key == GLFW_KEY_W) Position += Front * velocity;
    if (key == GLFW_KEY_S) Position -= Front * velocity;
    if (key == GLFW_KEY_A) Position -= Right * velocity;
    if (key == GLFW_KEY_D) Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = 400, lastY = 300;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX);
    float yoffset = (lastY - ypos); // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}