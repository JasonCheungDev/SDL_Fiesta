#pragma once

#include <glew.h>
#include <gl\GLU.h>
#include "glm\glm.hpp"

class Camera
{
public: 
	glm::vec3 position;
	glm::vec3 rotation; 

public:
	Camera();
	~Camera();
	glm::mat4 GetProjectionMatrix(); 
	glm::mat4 GetViewMatrix(); 
};

