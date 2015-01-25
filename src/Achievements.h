#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
}

#include "ProgressMeter.h"

class AchieveConfig;
//class MouseHandling;

enum Achievement_Direction{A_Up, A_Down, A_Left, A_Right};

class Achievements
{
public:
	Achievements(SDL_Surface* pScreen, AchieveConfig* pAchieveConfig/*, MouseHandling* pMouse*/);
	~Achievements();

	bool Loop();
	
protected:
	bool PollEvents();
	void UpdateDisplay();
	void Move(Achievement_Direction eDirection);
	void UpdateDoneAmounts();

protected:
	SDL_Surface	*m_pScreen;//Does not own
	enum AchievementChoice{None, PlayedAGame, Played10Games, WonAGame, Won5Games, Win8Colors, Win5Holes, WinLastTry};
	AchievementChoice m_eChoice;
	nSDL_Font	*m_pFont;
	nSDL_Font	*m_pFontRed;
	AchieveConfig	*m_pAchieveConfig;
	//MouseHandling   *m_pMouse;
	SDL_Surface	*m_imgPlayedAGame;
	SDL_Surface	*m_imgPlayed10Games;
	SDL_Surface	*m_imgWonAGame;
	SDL_Surface	*m_imgWon5Games;
	SDL_Surface	*m_imgWin8Colors;
	SDL_Surface	*m_imgWin5Holes;
	SDL_Surface	*m_imgWinLastTry;

	SDL_Surface	*m_imgStar;

	ProgressMeter	m_Progress;
};

#endif
