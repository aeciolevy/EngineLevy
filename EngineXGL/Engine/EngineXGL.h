#pragma once

#include "Engine\EngineInterface.h"
#include "../SDL.h"
#include "GLEW.h"
#include "../Game/Header/Game.h"

class exEngine : public exEngineInterface
{
public:
	exEngine() {};
	
	void				Initialize();
	void				Loop();


	void				InitializeShaders();
	// causes all initialization to occur and the main loop to start 
	void				Run(exGameInterface* pGameInterface) override;

	// draw a line
	void				DrawLine(const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer) override {};

	// draw a filled box
	void				DrawBox(const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer) override;

	// draw a box outline
	void				DrawLineBox(const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer) override {};

	// draw a filled circle
	void				DrawCircle(const exVector2& v2Center, float fRadius, const exColor& color, int nLayer) override {};

	// draw a circle outline
	void				DrawLineCircle(const exVector2& v2Center, float fRadius, const exColor& color, int nLayer) override {};

	void				OnFrame(float fDeltaT);

	void				ConsumeEvents();

	// load a font, >override upon success, negative upon failure
	int					LoadFont(const char* szFile, int nPTSize) override 
	{
		return 0;
	};

	// draw text with a given loaded font
	void				DrawText(int nFontID, const exVector2& v2Position, const char* szText, const exColor& color, int nLayer) override {};

private:
	SDL_Window * mWindow;
	exGameInterface* mGame;
};