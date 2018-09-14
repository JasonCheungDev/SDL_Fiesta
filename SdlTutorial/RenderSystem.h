#pragma once
#include <vector>
#include "Renderable.h"
#include "Camera.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	// Draws all (visible) objects 
	void Draw();

	// I think this should be a pointer? 
	// Adds a renderable to be drawn 
	void AddRenderable(Renderable r);

	// Removes a renderable to no longer be drawn 
	void RemoveRenderable(Renderable r);

	// I think this should be a pointer? 
	// Sets the main camera to render from 
	void SetCamera(Camera camera);

private: 
	std::vector<Renderable> renderables; 
	Camera mainCamera; 
};

