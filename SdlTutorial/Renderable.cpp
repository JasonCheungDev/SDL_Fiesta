#include "Renderable.h"
#include <glew.h>
#include <gl\GLU.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

Renderable::Renderable(Shader shader, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices)
{
	// CREATE VBO 

	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	*/
	// CREATE VAO & VBO
	// unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
	// std::vector<double> v;
	// double* a = &v[0];
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertices.size, &vertices[0], GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 3 * indices.size, &indices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);	// unbind VAO 
}

Renderable::~Renderable()
{
}

glm::mat4 Renderable::GetModelMatrix()
{
	glm::mat4 m = glm::mat4();
	m = glm::translate(m, position);
	m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
	m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));
	m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1));
	return m;
}
