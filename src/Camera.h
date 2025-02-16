#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    float MovementSpeed;
    float MouseSensitivity;
    float Yaw;
    float Pitch;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f));
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(int key, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);

private:
    void updateCameraVectors();
};

// Declaration of global camera instance
extern Camera camera;

// Mouse callback prototype
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#endif // CAMERA_H