#include "Camera.h"

#include <glew.h>
#include <gl\GLU.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

Camera::Camera()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(45.0f), (float)1920.0f / 1080.0f, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 v = glm::mat4(1.0f);	// this is the identity matrix 
	v = glm::translate(v, position);
	v = glm::rotate(v, rotation.z, glm::vec3(0, 0, 1));
	v = glm::rotate(v, rotation.y, glm::vec3(0, 1, 0));
	v = glm::rotate(v, rotation.x, glm::vec3(1, 0, 0));
	return glm::inverse(v);
}
