//
// * ENGINE-X
// * SAMPLE GAME
//
// + Main.cpp
// entry point
//

#include "stdafx.h"
#include <windows.h>
#include "Engine\EngineInterface.h"
#include "GameInterface.h"
#include "EngineXGL.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

// this is could be better, think design patterns as to why
//Pong gGame;

class exGame : public exGameInterface
{

	// called when the engine is ready for the game to initialize
	virtual void				Initialize(exEngineInterface* pEngine) {}

	// to determine the name of the window
	virtual const char*			GetWindowName() const { return "hello"; }

	// to determine clear color
	virtual void				GetClearColor(exColor& color) const 
	{
	
	}

	// called per event polled
	// https://wiki.libsdl.org/SDL_Event
	virtual void				OnEvent(SDL_Event* pEvent) {}

	// called when events have all been processed, before simulation has happened
	virtual void				OnEventsConsumed() {}

	// run the simulation
	virtual void				Run(float fDeltaT) {}

};

//-----------------------------------------------------------------
//-----------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//// never allow two instances
	//if (hPrevInstance)
	//{
	//	return 0;
	//}

	//// check version
	//if (AccessEngineVersion() != kEngineVersion)
	//{
	//	return 0;
	//}

	//// find the engine
	//exEngineInterface* pEngine = AccessEngine();

	//if (pEngine == nullptr)
	//{
	//	return 0;
	//}

	//// tell it to run
	////pEngine->Run(&gGame);

	
	exEngine engine;

	exGame game;
	engine.Run(&game);

	return 0;
}

