//
// * ENGINE-X
//
// + EngineInterface.h
// interface to the engine for the game
//

#pragma once

#include "EngineTypes.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const int kEngineVersion = 1;			// modify when API changes

//-----------------------------------------------------------------
//-----------------------------------------------------------------

class exGameInterface;

//-----------------------------------------------------------------
//-----------------------------------------------------------------

class exEngineInterface
{
public:

								// causes all initialization to occur and the main loop to start 
	virtual void				Run( exGameInterface* pGameInterface ) = 0;

								// draw a line
	virtual void				DrawLine( const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer ) = 0;

								// draw a filled box
	virtual void				DrawBox( const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer ) = 0;

								// draw a box outline
	virtual void				DrawLineBox( const exVector2& v2P1, const exVector2& v2P2, const exColor& color, int nLayer ) = 0;

								// draw a filled circle
	virtual void				DrawCircle( const exVector2& v2Center, float fRadius, const exColor& color, int nLayer ) = 0;

								// draw a circle outline
	virtual void				DrawLineCircle( const exVector2& v2Center, float fRadius, const exColor& color, int nLayer ) = 0;

								// load a font, >= 0 upon success, negative upon failure
	virtual int					LoadFont( const char* szFile, int nPTSize ) = 0;

								// draw text with a given loaded font
	virtual void				DrawText( int nFontID, const exVector2& v2Position, const char* szText, const exColor& color, int nLayer ) = 0;

};

//-----------------------------------------------------------------
//-----------------------------------------------------------------

int AccessEngineVersion();
exEngineInterface* AccessEngine();
