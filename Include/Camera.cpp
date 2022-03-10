#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_MoveMent direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == UP)
		Position += Up * velocity;
	if (direction == DOWN)
		Position -= Up * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.f)
		Pitch = 89.f;
	if (Pitch < -89.f)
		Pitch = -89.f;

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float offset)
{
	if (Zoom >= 1.f&&Zoom <= 80.f)
		Zoom -= offset;
	if (Zoom <= 1.f)
		Zoom = 1.f;
	if (Zoom >= 80.f)
		Zoom = 80.f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.y = sin(glm::radians(Pitch));
	front.x = cos(glm::radians(Pitch))*cos(glm::radians(Yaw));
	front.z = cos(glm::radians(Pitch))*sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
