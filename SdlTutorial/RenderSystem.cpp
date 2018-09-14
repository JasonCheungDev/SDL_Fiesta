#include "RenderSystem.h"

#include <stdio.h>
#include <glew.h>
#include <gl\GLU.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"


RenderSystem::RenderSystem()
{
	// shader should be independent 
	shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Draw()
{
	if (mainCamera == NULL)
	{
		printf("ERROR: CAMERA IS NULL");
		return;
	}

	GLint model = glGetUniformLocation(shader->programId, "u_Model");
	GLint view = glGetUniformLocation(shader->programId, "u_View");
	GLint projection = glGetUniformLocation(shader->programId, "u_Projection");


	for (int i = 0; i < renderables.size(); i++)
	{
		shader->use();

		auto m = renderables[i].GetModelMatrix();
		auto v = mainCamera->GetViewMatrix();
		auto p = mainCamera->GetProjectionMatrix();

		glUniformMatrix4fv(model, 1, false, &renderables[i].GetModelMatrix()[0][0]);
		glUniformMatrix4fv(view, 1, false, &mainCamera->GetViewMatrix()[0][0]);
		glUniformMatrix4fv(projection, 1, false, &mainCamera->GetProjectionMatrix()[0][0]);

		glBindVertexArray(renderables[i].VAO);
		glDrawElements(GL_TRIANGLES, renderables[i].indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void RenderSystem::AddRenderable(Renderable r)
{
	renderables.push_back(r);
}

void RenderSystem::RemoveRenderable(Renderable r)
{
	// TODO
}

void RenderSystem::SetCamera(Camera* camera)
{
	mainCamera = camera; 
}
