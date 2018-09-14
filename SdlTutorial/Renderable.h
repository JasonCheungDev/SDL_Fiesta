#pragma once

#include <vector>
#include <glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include "Shader.h"

class Renderable
{
public: 
	unsigned int VAO; 
	glm::vec3 position;
	glm::vec3 rotation; 

public:
	Renderable(Shader shader, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);
	~Renderable();

	glm::mat4 GetModelMatrix();
};

