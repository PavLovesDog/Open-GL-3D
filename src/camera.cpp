#include "camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, float startPitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Yaw(startYaw), Pitch(startPitch) {
    Position = startPosition;
    WorldUp = startUp;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= Right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

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