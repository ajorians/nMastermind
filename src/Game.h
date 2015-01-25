#ifndef GAME_H
#define GAME_H

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
	#include "MastermindLib/MastermindLib.h"
}

#include "Defines.h"
#include "Direction.h"
#include "Metrics.h"
#include "Selector.h"

#ifndef USE_GRAPHIC_YOU_WIN
#include "Message.h"
#endif

class Config;
//class MouseHandling;

class Game
{
public:
	Game(SDL_Surface* pScreen /*MouseHandling* pMouse*/, Config* pConfig);
	~Game();

	bool Loop();
	
protected:
	bool PollEvents();
	void UpdateDisplay();
	void Move(Direction eDirection);
	void EvaluatePegs();
	void RemoveCurrentPeg();
        void DrawRedWhite(int nX, int nY, int nRed, int nWhite);
	void ShowSolution();

	void DisplayPiece(int nX, int nY, int nPiece);

protected:
	SDL_Surface	*m_pScreen;//Does not own
	//MouseHandling	*m_pMouse;//Does not own
	Config		*m_pConfig;//Does not own
	SDL_Surface	*m_pBackground;
#ifndef USE_GRAPHIC_YOU_WIN
	Message		m_YouWinMessage;
#endif
	nSDL_Font *m_pFont;
	MasterLib m_Master;
	//PieceControl	m_Pieces;
	Metrics		m_Metrics;
	Selector	m_Selector;
};

#endif
