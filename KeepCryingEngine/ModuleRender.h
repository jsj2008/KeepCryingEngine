#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_

#define HAVE_M_PI
#define _CRTDBG_MAP_ALLOC

#include <SDL.h>
#include <float3.h>

#include "Module.h"


class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	update_status PreUpdate(float deltaTimeS, float realDeltaTimeS) override;
	update_status Update(float deltaTimeS, float realDeltaTimeS) override;
	update_status PostUpdate(float deltaTimeS, float realDeltaTimeS) override;
	bool CleanUp() override;

	void DrawCross(const float3& pos, float scale) const;

private:
	void SetUpBigArray() const;
	void SetUpIndicesArray() const;
	void SetUpSphere(float radius, unsigned int rings, unsigned int sectors);
	void DrawCubeDirect(float x, float y, float z) const;
	void DrawCubeBigArray(float x, float y, float z) const;
	void DrawCubeIndices(float x, float y, float z) const;
	void DrawSphere(float x, float y, float z) const;

	void SetUpGrid() const;
	void DrawGrid() const;

	void SetUpCheckerTexture();

private:
	uint bigArrayCube = 0;
	uint vertexArrayBuffer = 0;
	uint indicesArrayBuffer = 0;
	uint sphereIndex = 0;
	uint sphereVertex = 0;

	int sphereIndicesSize = 0;

	SDL_GLContext glcontext = nullptr;

	uint chekerTextureId = 0;

	static const uint CHECKERS_HEIGHT;
	static const uint CHECKERS_WIDTH;
};

#endif // !_MODULERENDER_H_