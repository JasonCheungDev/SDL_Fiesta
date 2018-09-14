/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// System support
#include <stdio.h>
#include <string>
#include <vector>
// Standard SDL 
#include <SDL.h>
// OpenGL support (order matters - glew must be first)
#include <glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm\glm.hpp>
// Custom objects 
#include "Shader.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#pragma region Declaration

// ===== Functions ======

// Starts up SDL and creates a window 
bool init();

// Loads media ? 
bool loadMedia();

// Frees media and shuts down SDL
void close();

SDL_Surface* loadSurface(std::string path);

//Shader loading utility programs
void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);
bool initGL(); 

// ===== Variables =====

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// Global window handle 
SDL_Window* gWindow = NULL;
// Surface contained by the window 
SDL_Surface* gScreenSurface = NULL;
// Image to load and show on the screen
SDL_Surface* gHelloWorld = NULL;
// Pointers to images 
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
// Current image shown 
SDL_Surface* gCurrentSurface = NULL;

// OpenGL 
SDL_GLContext gContext = NULL;
GLuint gProgramId = 0;
GLint gVertexPos2DLocation = 01; 
GLuint gVBO = 0;
GLuint gIBO = 0;
GLuint gVAO = 0;
bool gRenderQuad = true;
Shader* gShader;



class Cube
{
	std::vector<glm::vec3> vertices = {
		// Front
		glm::vec3(1, -1, 1), // 0
		glm::vec3(1, 1, 1), // 1
		glm::vec3(-1, 1, 1), // 2
		glm::vec3(-1, -1, 1), // 3

		// Bac
		glm::vec3(-1, -1, -1), // 4
		glm::vec3(-1, 1, -1), // 5
		glm::vec3(1, 1, -1), // 6
		glm::vec3(1, -1, -1), // 7

		// Lef
		glm::vec3(-1, -1, 1), // 8
		glm::vec3(-1, 1, 1), // 9
		glm::vec3(-1, 1, -1), // 10
		glm::vec3(-1, -1, -1), // 11

		// Righ
		glm::vec3(1, -1, -1), // 12
		glm::vec3(1, 1, -1), // 13
		glm::vec3(1, 1, 1), // 14
		glm::vec3(1, -1, 1), // 15

		// To
		glm::vec3(1, 1, 1), // 16
		glm::vec3(1, 1, -1), // 17
		glm::vec3(-1, 1, -1), // 18
		glm::vec3(-1, 1, 1), // 19

		// Botto
		glm::vec3(1, -1, -1), // 20
		glm::vec3(1, -1, 1), // 21
		glm::vec3(-1, -1, 1), // 22
		glm::vec3(-1, -1, -1), // 23
	};

	std::vector<unsigned int> indices = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Back
		4, 5, 6,
		6, 7, 4,

		// Left
		8, 9, 10,
		10, 11, 8,

		// Right
		12, 13, 14,
		14, 15, 12,

		// Top
		16, 17, 18,
		18, 19, 16,

		// Bottom
		20, 21, 22,
		22, 23, 20
	};
};

// AKA Mesh. Object's geometry. 
class Model
{
	std::vector<glm::vec3> vertices; 
	std::vector<GLuint> indices; 
	GLuint indexCount; 
	GLuint VBO, IBO;
};



#pragma endregion



#pragma region Definition 

void render()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Bind program
	gShader->use();
	// glUseProgram(gProgramId);

	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	/*
	//Render quad
	if (gRenderQuad)
	{
		//Bind program
		gShader->use();
		// glUseProgram(gProgramId);

		glBindVertexArray(gVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		////Enable vertex position
		//glEnableVertexAttribArray(0);

		////Set vertex data
		//glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

		////Set index data and render
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		//glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

		//Disable vertex position
		//glDisableVertexAttribArray(0);

		//Unbind program
		glUseProgram(NULL);
	}
	*/
}


void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}


bool initGL()
{
	//Success flag
	bool success = true;

	//Generate program
	gShader = new Shader("shaders/vertex.vs", "shaders/fragment.fs");

	// correct stuff 
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// CREATE VBO 
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	*/
	// CREATE VAO & VBO
	// unsigned int VAO;
	glGenVertexArrays(1, &gVAO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);


	// 1. bind Vertex Array Object
	glBindVertexArray(gVAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	/*
	// ----- Create vertex shader------
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		printShaderLog(vertexShader);
		success = false;
		return false; 
	}
	//Attach vertex shader to program
	glAttachShader(gProgramId, vertexShader);


	// ------ Create fragment shader ------
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Get fragment source
	const GLchar* fragmentShaderSource[] =
	{
		"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
	};

	//Set fragment source
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	//Compile fragment source
	glCompileShader(fragmentShader);

	//Check fragment shader for errors
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("Unable to compile fragment shader %d!\n", fragmentShader);
		printShaderLog(fragmentShader);
		success = false;
		return false;
	}
	//Attach fragment shader to program
	glAttachShader(gProgramId, fragmentShader);


	// ----- Link program ------
	glLinkProgram(gProgramId);


	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(gProgramId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", gProgramId);
		printProgramLog(gProgramId);
		success = false;
	}
	else
	{

	}
	*/

	/*
	//Get vertex attribute location
	gVertexPos2DLocation = 0; // glGetAttribLocation(gProgramId, "i_Position");
	if (gVertexPos2DLocation == -1)
	{
		printf("LVertexPos2D is not a valid glsl program variable!\n");
		success = false;
	}
	else
	{
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//VBO data
		//GLfloat vertexData[] =
		//{
		//	-0.5f, -0.5f,
		//	0.5f, -0.5f,
		//	0.5f,  0.5f,
		//	-0.5f,  0.5f
		//};

		////IBO data
		//GLuint indexData[] = { 0, 1, 2, 3 };

		////Create VBO
		//glGenBuffers(1, &gVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		//glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

		////Create IBO
		//glGenBuffers(1, &gIBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	




	}
	*/
	printf("OpenGL full initiailized\n");

	return success;
}

void blitFullscreen(SDL_Surface* surface)
{
	// Apply the image stretched 
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;
	SDL_BlitScaled(surface, NULL, gScreenSurface, &stretchRect);
}

SDL_Surface* loadSurface(std::string path)
{
	// final optimized surface 
	SDL_Surface* optimizedSurface = NULL; 
	
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		// convert surface to screen format 
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

bool init()
{
	// init flag 
	bool success = true;

	// init SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Use OpenGL 3.1 core 
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	// using core as opposed to compatibility or ES 

		// create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		
			// create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false; 
			}
			else
			{
				// init GLEW 
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit(); 
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				// use Vsync 
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				// init OpenGL 
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false; 
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	// gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("04_key_presses/press.bmp");
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("05_optimized_surface_loading_and_soft_stretching/stretch.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}

	//Load up surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("04_key_presses/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	//Load down surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("04_key_presses/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	//Load left surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("04_key_presses/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	//Load right surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("04_key_presses/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}

	return success;
}

void close()
{
	// deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	// destroy window 
	SDL_DestroyWindow(gWindow);	// this also destroys the gScreenSurface 
	gWindow = NULL;

	// quit SDL subsystems 
	SDL_Quit();
}

#pragma endregion




// SDL requires this main function signature to work with multiple platforms 
int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize\n");
		SDL_Delay(5000);
	}
	else
	{
		printProgramLog(gShader->programId);

		// load media 
		if (!loadMedia())
		{
			printf("Failed to load media\n");
			SDL_Delay(5000);
		}
		else
		{
			// wait for user input 
			bool quit = false;
			SDL_Event e;
			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			while (!quit)
			{
				// handle events on queue 
				while (SDL_PollEvent(&e) != 0)
				{
					// user requested to quit 
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						// select surfaces based on key press 
						switch (e.key.keysym.sym)
						{
						case SDLK_q:
						{
							gRenderQuad = !gRenderQuad;
						}
						case SDLK_UP:
						{
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						}
						case SDLK_DOWN:
						{
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						}
						case SDLK_LEFT:
						{
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						}
						case SDLK_RIGHT:
						{
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						}
						default:
						{
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
						}

						// apply current image 
						// SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
						//blitFullscreen(gCurrentSurface);
						//SDL_UpdateWindowSurface(gWindow);
					} 
				} // end while ( event check) 

				render();
				SDL_GL_SwapWindow(gWindow);

			} // end game loop 
		}
	}


	close();

	return 0;
}