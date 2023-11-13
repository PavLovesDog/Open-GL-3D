#ifndef CAMERA
#define CAMERA

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h> // for keyboard input

class Camera {
public:
	// Constructor, with vectors
	Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, float startPitch);


	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw; // Euler Angles
	float Pitch;	
	float MovementSpeed; // Camera options
	float MouseSensitivity;

	//Functions
	glm::mat4 GetViewMatrix(); // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	void ProcessKeyboard(GLFWwindow* window, float deltaTime); // Processes input received from any keyboard-like input system
	void ProcessMouseMovement(float xOffset, float yOffset); // Processes input received from a mouse input system

private:
	void updateCameraVectors();
};

#endif // !CAMERA
