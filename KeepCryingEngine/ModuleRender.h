#ifndef _MODULERENDER_H_
#define _MODULERENDER_H_

#define HAVE_M_PI

#include <SDL.h>
#include <GL/glew.h>
#include <float3.h>

#include "Module.h"

class ModuleRender : public Module
{
public:
	ModuleRender();
	virtual ~ModuleRender();

	bool Init() override;
	update_status PreUpdate(float deltaTimeS, float realDeltaTimeS) override;
	update_status Update(float deltaTimeS, float realDeltaTimeS) override;
	update_status PostUpdate(float deltaTimeS, float realDeltaTimeS) override;
	bool CleanUp() override;

	void DrawCross(const float3& pos, float scale) const;

private:
	void DrawGrid() const;

	void SetUpLight() const;

public:
	GLfloat globalAmbient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

private:	
	SDL_GLContext glcontext = nullptr;
};

#endif // !_MODULERENDER_H_