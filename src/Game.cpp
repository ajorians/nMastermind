#include "Game.h"
#include "BoardBackground.h"
#include "Config.h"
//#include "MouseHandling.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

Game::Game(SDL_Surface* pScreen/*, MouseHandling* pMouse*/, Config* pConfig)
: m_pScreen(pScreen)/*, m_pMouse(pMouse)*/, m_pConfig(pConfig)
#ifndef USE_GRAPHIC_YOU_WIN
, m_YouWinMessage(pScreen)
#endif
, m_Selector(pScreen, pConfig, &m_Metrics)
{
	MastermindLibCreate(&m_Master, Guessing);

	m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);

	m_pBackground = nSDL_LoadImage(image_cardbackground);

	m_Metrics.SetBoardDimensions(4, 10);
}

Game::~Game()
{
	nSDL_FreeFont(m_pFont);
	SDL_FreeSurface(m_pBackground);
}

bool Game::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;

	//Update screen
	UpdateDisplay();

	SDL_Delay(30);
	
	return true;
}

bool Game::PollEvents()
{
	SDL_Event event;
	
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                  */
	while( SDL_PollEvent( &event ) )
	{
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch( event.type )
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						fprintf(stderr, "Hit Escape!n");
					return false;
					break;

					case SDLK_RETURN:
					case SDLK_LCTRL:
					case SDLK_RCTRL:
						EvaluatePegs();
					break;
					
					case SDLK_RIGHT:
					case SDLK_6:
						Move(Right);
					break;
					
					case SDLK_LEFT:
					case SDLK_4:
						Move(Left);
					break;

					case SDLK_UP:
					case SDLK_8:
						Move(Up);
					break;

					case SDLK_DOWN:
					case SDLK_2:
						Move(Down);
					break;

					case SDLK_CLEAR:
					case SDLK_BACKSPACE:
						RemoveCurrentPeg();
					break;

					default:
						break;
				}
				break;
			
			//Called when the mouse moves
			case SDL_MOUSEMOTION:
				break;
			
			case SDL_KEYUP:
				break;
			
			default:
				break;
		}
	}

	/*int nMX = -1, nMY = -1;
        if( m_pMouse->PollMouse(nMX, nMY) ) {
                MouseButton eMouseButton = m_pMouse->GetMouseButton();
                if( eMouseButton == CenterButton ) {
			SelectCard();
		}
	}*/

	return true;
}

void Game::UpdateDisplay()
{
#if 0
   SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 255, 0, 0));
#else
   SDL_BlitSurface(m_pBackground, NULL, m_pScreen, NULL);
#endif

   for(int y=0; y<10; y++) {
      for(int x=0; x<4; x++) {
         int nX = x*15+20, nY = SCREEN_HEIGHT-(y*15)-20, nWidth = 15, nHeight = 15;
         int nPiece = GetMasterColorPeg(m_Master, y, x);
         //if( nPiece == 0 )
            //continue;
         DisplayPiece(nX, nY, nPiece);
      }

      int nRed, nWhite;
      GetMasterResult(m_Master, y, &nRed, &nWhite);
      DrawRedWhite(80, SCREEN_HEIGHT-(y*15)-20, nRed, nWhite);
      //nSDL_DrawString(m_pScreen, m_pFont, 80, SCREEN_HEIGHT-(y*15)-20, "%d - %d", nRed, nWhite);
   }

   if( MASTERLIB_GAME_OVER == IsMasterGameOver(m_Master) ) {
      ShowSolution();
   } else {
      m_Selector.DrawSelector();
   }

#ifndef USE_GRAPHIC_YOU_WIN
   if( !m_YouWinMessage.Animate() )
#endif
   {
      SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   }
}

void Game::Move(Direction eDirection)
{
   if( MASTERLIB_GAME_OVER == IsMasterGameOver(m_Master) )
      return;

   if( eDirection == Left || eDirection == Right ) {
      m_Selector.Move(eDirection);
   }
   if( eDirection == Up ) {
      int n = GetMasterColorPeg(m_Master, m_Selector.GetCurrentY(), m_Selector.GetCurrentX());
      n++;
      if( n > 6 )
         n = 0;
      PlaceMasterColorPeg(m_Master, m_Selector.GetCurrentX(), n);
   }
   if( eDirection == Down ) {
      int n = GetMasterColorPeg(m_Master, m_Selector.GetCurrentY(), m_Selector.GetCurrentX());
      n--;
      if( n < 0 )
         n = 6;
      PlaceMasterColorPeg(m_Master, m_Selector.GetCurrentX(), n);
   }
}

void Game::EvaluatePegs()
{
   if( MASTERLIB_OK == TakeMasterGuess(m_Master, NULL, NULL) ) {
      m_Selector.IncrementRow();
      if( MASTERLIB_GAME_OVER == IsMasterGameOver(m_Master) ) {
#ifndef USE_GRAPHIC_YOU_WIN
         m_YouWinMessage.CreateMessage("You Win!!!\n******");
#endif
      }
   }
}

void Game::RemoveCurrentPeg()
{
}

void Game::DrawRedWhite(int nX, int nY, int nRed, int nWhite)
{
   for(int i=0; i<nRed; i++) {
      filledEllipseRGBA(m_pScreen, nX+4, nY+4, 4, 4, 0, 0, 0, 255);
      nX += 10;
   }
   for(int i=0; i<nRed; i++) {
      filledEllipseRGBA(m_pScreen, nX+4, nY+4, 4, 4, 255, 255, 255, 255);
      nX += 10;
   }
}

void Game::ShowSolution()
{
   DisplayPiece(20, 0, GetMasterSolutionPeg(m_Master, 0));
   DisplayPiece(35, 0, GetMasterSolutionPeg(m_Master, 1));
   DisplayPiece(50, 0, GetMasterSolutionPeg(m_Master, 2));
   DisplayPiece(65, 0, GetMasterSolutionPeg(m_Master, 3));
}

void Game::DisplayPiece(int nX, int nY, int nPiece)
{
   int nR, nG, nB;
   switch(nPiece) {
      default:
      case 0:
         nR = 0, nG = 0, nB = 0;
      break;

      case 1:
         nR = 255, nG = 0, nB = 0;
      break;

      case 2:
         nR = 0, nG = 255, nB = 0;
      break;

      case 3:
         nR = 0, nG = 0, nB = 255;
      break;

      case 4:
         nR = 255, nG = 255, nB = 0;
      break;

      case 5:
         nR = 255, nG = 0, nB = 255;
      break;

      case 6:
         nR = 0, nG = 255, nB = 255;
      break;
   }

   filledEllipseRGBA(m_pScreen, nX+7, nY+7, 7, 7, nR, nG, nB, 255);
}
