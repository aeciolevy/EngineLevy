//
// * ENGINE-X
//
// + GameInterface.h
// interface to game from the engine
//

#pragma once

#include "Engine\EngineTypes.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

class exEngineInterface;
union SDL_Event;

//-----------------------------------------------------------------
//-----------------------------------------------------------------

class exGameInterface
{
public:

								// called when the engine is ready for the game to initialize
	virtual void				Initialize( exEngineInterface* pEngine ) = 0;

								// to determine the name of the window
	virtual const char*			GetWindowName() const = 0;

								// to determine clear color
	virtual void				GetClearColor( exColor& color ) const = 0;

								// called per event polled
								// https://wiki.libsdl.org/SDL_Event
	virtual void				OnEvent( SDL_Event* pEvent ) = 0;

								// called when events have all been processed, before simulation has happened
	virtual void				OnEventsConsumed() = 0;

								// run the simulation
	virtual void				Run( float fDeltaT ) = 0;

};
