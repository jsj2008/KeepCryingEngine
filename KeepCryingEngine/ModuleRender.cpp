#include "ModuleRender.h"

#include "GL/glew.h"
#include "SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Application.h"
#include "ModuleWindow.h"

ModuleRender::ModuleRender()
{}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
	LOG_DEBUG("Creating Renderer context");
	bool ret = true;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	glcontext = SDL_GL_CreateContext(App->window->window);

	LOG_DEBUG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG_DEBUG("Vendor: %s", glGetString(GL_VENDOR));
	LOG_DEBUG("Renderer: %s", glGetString(GL_RENDERER));
	LOG_DEBUG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG_DEBUG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	GLenum err = glewInit();

	setupBigArray();
	setupIndicesArray();

	return ret;
}

update_status ModuleRender::PreUpdate(float deltaTimeS, float realDeltaTimeS)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float deltaTimeS, float realDeltaTimeS)
{
	float half = 0.5f;

	glRotatef(20.0f * deltaTimeS, 0.0f, 1.0f, 0.2f);

	drawCubeDirect();
	//drawCubeBigArray();
	//drawCubeIndices();

	SDL_GL_SwapWindow(App->window->window);
	return update_status::UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	SDL_GL_DeleteContext(glcontext);
	return true;
}

void ModuleRender::setupBigArray()
{
	float half = 0.5f;

	float vertices[36 * 3] = {
		-half, -half, -half,
		half, -half, -half,
		-half, half, -half,
		half, -half, -half,
		half, half, -half,
		-half, half, -half,
		half, -half, -half,
		half, -half, half,
		half, half, -half,
		half, -half, half,
		half, half, half,
		half, half, -half,
		-half, half, -half,
		half, half, -half,
		-half, half, half,
		half, half, -half,
		half, half, half,
		-half, half, half,
		-half, -half, half,
		-half, half, half,
		half, -half, half,
		half, -half, half,
		-half, half, half,
		half, half, half,
		-half, -half, -half,
		-half, half, -half,
		-half, -half, half,
		-half, -half, half,
		-half, half, -half,
		-half, half, half,
		-half, -half, -half,
		-half, -half, half,
		half, -half, -half,
		half, -half, -half,
		-half, -half, half,
		half, -half, half
	};

	glGenBuffers(1, (GLuint*) &(bigArrayCube));
	glBindBuffer(GL_ARRAY_BUFFER, bigArrayCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRender::setupIndicesArray()
{
	float half = 0.5f;
	float uniqueVertex[8 * 3] = {
		-0.5f, 0.0f, 0.0f, //0
		0.5f, 0.0f, 0.0f,  //1
		0.5f, 0.5f, 0.0f,  //2
		-0.5f, 0.5f, 0.0f, //3
		-0.5f, 0.0f, 0.5f, //4
		0.5f, 0.0f, 0.5f,  //5
		0.5f, 0.5f, 0.5f,  //6
		-0.5f, 0.5f, 0.5f  //7
	};

	glGenBuffers(1, (GLuint*) &(vertexArrayBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, uniqueVertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indicesArray[36 * 3] = {
		0, 1, 3,
		1, 2, 3,

		1, 5, 2,
		5, 6, 2,

		5, 4, 6,
		4, 7, 6,

		4, 0, 7,
		0, 3, 7,

		3, 2, 7,
		2, 6, 7,

		4, 5, 0,
		5, 1, 0
	};

	glGenBuffers(1, (GLuint*) &(indicesArrayBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesArrayBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36 * 3, indicesArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ModuleRender::drawCubeDirect()
{
	float half = 0.5f;

	glBegin(GL_TRIANGLES);

	glColor3f(100.0f, 0.0f, 0.0f);
	glVertex3f(-half, -half, -half);
	glVertex3f(half, -half, -half);
	glVertex3f(-half, half, -half);
	glVertex3f(half, -half, -half);
	glVertex3f(half, half, -half);
	glVertex3f(-half, half, -half);

	glColor3f(0.0f, 100.0f, 0.0f);
	glVertex3f(half, -half, -half);
	glVertex3f(half, -half, half);
	glVertex3f(half, half, -half);
	glVertex3f(half, -half, half);
	glVertex3f(half, half, half);
	glVertex3f(half, half, -half);

	glColor3f(0.0f, 0.0f, 100.0f);
	glVertex3f(-half, half, -half);
	glVertex3f(half, half, -half);
	glVertex3f(-half, half, half);
	glVertex3f(half, half, -half);
	glVertex3f(half, half, half);
	glVertex3f(-half, half, half);

	glColor3f(100.0f, 0.0f, 0.0f);
	glVertex3f(-half, -half, half);
	glVertex3f(-half, half, half);
	glVertex3f(half, -half, half);
	glVertex3f(half, -half, half);
	glVertex3f(-half, half, half);
	glVertex3f(half, half, half);

	glColor3f(0.0f, 100.0f, 0.0f);
	glVertex3f(-half, -half, -half);
	glVertex3f(-half, half, -half);
	glVertex3f(-half, -half, half);
	glVertex3f(-half, -half, half);
	glVertex3f(-half, half, -half);
	glVertex3f(-half, half, half);

	glColor3f(0.0f, 0.0f, 100.0f);
	glVertex3f(-half, -half, -half);
	glVertex3f(-half, -half, half);
	glVertex3f(half, -half, -half);
	glVertex3f(half, -half, -half);
	glVertex3f(-half, -half, half);
	glVertex3f(half, -half, half);

	glEnd();
}

void ModuleRender::drawCubeBigArray()
{
	glColor3f(0.0f, 0.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, bigArrayCube);

	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRender::drawCubeIndices()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesArrayBuffer);
	glDrawElements(GL_TRIANGLES, 36 * 3, GL_UNSIGNED_INT, nullptr); //The nullptr means 'take the last binded buffer'

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
