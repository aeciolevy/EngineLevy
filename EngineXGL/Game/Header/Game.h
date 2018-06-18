#pragma once
#include "GameInterface.h"
#include "../../Engine/EngineTypes.h"

class Game : public exGameInterface
{
public:

	Game();
	virtual						~Game();

	virtual void				Initialize(exEngineInterface* pEngine);

	virtual const char*			GetWindowName() const;
	virtual void				GetClearColor(exColor& color) const;

	virtual void				OnEvent(SDL_Event* pEvent);
	virtual void				OnEventsConsumed();

	virtual void				CheckCollision();

	virtual void				Run(float fDeltaT);

private:

	exEngineInterface * mEngine;

};