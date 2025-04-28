#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

enum camMovement {
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	RUN,
	WALK
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float MAXSPEED = 6.0f;
const float SENSITIVITY = 0.08f;
const float FOV = 45.0f;

class Camera {
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;
	float MovementSpeed;
	float Sensitivity;
	float Fov;

	Camera(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		Sensitivity(SENSITIVITY), Fov(FOV) {
		Pos = pos;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Pos, Pos + Front, Up);
	}

	void ProcessKeyboard(camMovement dir, float deltaTime) {
		float vel = MovementSpeed * deltaTime;

		switch (dir) {
		case FORWARD:
			Pos += Front * vel;
			break;
		case BACK:
			Pos -= Front * vel;
			break;
		case LEFT:
			Pos -= Right * vel;
			break;
		case RIGHT:
			Pos += Right * vel;
			break;
		case UP:
			Pos += Up * vel;
			break;
		case DOWN:
			Pos -= Up * vel;
			break;
		case RUN:
			MovementSpeed = MAXSPEED;
			break;
		case WALK:
			MovementSpeed = SPEED;
			break;
		}
		//std::cout << glm::to_string(Pos) << std::endl;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= Sensitivity;
		yoffset *= Sensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) {
		Fov -= yoffset;
		if (Fov < 1.0f)
			Fov = 1.0f;
		if (Fov > 45.0f)
			Fov = 45.0f;
	}

private:
	void updateCameraVectors() {
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif