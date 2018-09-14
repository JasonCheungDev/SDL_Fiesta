#include "RenderSystem.h"

#include <glew.h>
#include <gl\GLU.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Draw()
{
	for (int i = 0; i < renderables.size; i++)
	{
		glBindVertexArray(renderables[i].VAO);
		// bind 
		// set model uniform
		// set view uniform 
		// set projection uniform 
		// draw 
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

void RenderSystem::SetCamera(Camera camera)
{
	mainCamera = camera; 
}
