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
#include "../EngineXGL/Game/Header/GameInterface.h"
#include "Engine\EngineXGL.h"
#include "Game\Header\Game.h"
//-----------------------------------------------------------------
//-----------------------------------------------------------------

// this is could be better, think design patterns as to why

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

	Game game;
	engine.Run(&game);

	return 0;
}

