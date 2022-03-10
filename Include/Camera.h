#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_MoveMent {
	FORWARD,
	BACKWARD,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

const float YAW            = -90.f;
const float PITCH          = 0.f;
const float SPEED          = 2.5f;
const float SENSITIVITY    = 0.1f;
const float ZOOM           = 45.f;


class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW, float pitch = PITCH) :Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_MoveMent direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float offset);

private:
	void updateCameraVectors();
};

#endif // !CAMERA_H
