#include "ModuleTime.h"

#include <SDL.h>

#include "Application.h"

ModuleTime::ModuleTime()
{}

ModuleTime::~ModuleTime()
{}

update_status ModuleTime::Update()
{
	uint currentTimeMs = SDL_GetTicks();

	editorDeltaTime = (currentTimeMs - lastTimeMs) / 1000.0f;
	editorDeltaTime = fminf(editorDeltaTime, App->configuration.maxRealDeltaTimeS);

	if(App->state == TimeState::PLAYING)
	{
		realTimeDeltaTime = editorDeltaTime;
		deltaTime = timeScale * realTimeDeltaTime;	

		time += deltaTime;
		realTimeSinceStartup += realTimeDeltaTime;

		frameCount = 1 / deltaTime;
	}

	lastTimeMs = currentTimeMs;

	return update_status::UPDATE_CONTINUE;
}

void ModuleTime::Play()
{
	time = 0;
	realTimeSinceStartup = 0;

	App->state = TimeState::PLAYING;
}

void ModuleTime::Pause()
{
	deltaTime = 0;
	realTimeDeltaTime = 0;

	App->state = TimeState::PAUSED;
}

void ModuleTime::Unpause()
{
	App->state = TimeState::PLAYING;
}

void ModuleTime::Stop()
{
	deltaTime = 0;
	realTimeDeltaTime = 0;

	App->state = TimeState::STOPED;
}

float ModuleTime::GetFrameCount() const
{
	return frameCount;
}

float ModuleTime::GetTime() const
{
	return time;
}

float ModuleTime::GetTimeScale() const
{
	return timeScale;
}

float ModuleTime::GetDeltaTime() const
{
	return deltaTime;
}

float ModuleTime::GetRealTimeSinceStartup() const
{
	return realTimeSinceStartup;
}

float ModuleTime::GetRealTimeDeltaTime() const
{
	return realTimeDeltaTime;
}

float ModuleTime::GetEditorDeltaTime() const
{
	return editorDeltaTime;
}

void ModuleTime::SetTimeScale(float timeScale)
{
	this->timeScale = fmaxf(fminf(timeScale, 1.0f), 0.0f);
}

float ModuleTime_GetDeltaTimeInternal()
{
	return App->time->GetDeltaTime();
}

float ModuleTime_GetRealDeltaTimeInternal()
{
	return App->time->GetRealTimeDeltaTime();
}