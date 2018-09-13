#pragma once

#include <glew.h>
#include <gl\GLU.h>

class Shader
{
public:
	unsigned int programId;

public:
	Shader(const GLchar* vertexPath, const GLchar* fragPath);
	~Shader();

	void use(); 
};

