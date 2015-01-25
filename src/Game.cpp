#include "Game.h"
#include "BoardBackground.h"
#include "Config.h"
#include "AchieveConfig.h"
//#include "MouseHandling.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

Game::Game(SDL_Surface* pScreen/*, MouseHandling* pMouse*/, Config* pConfig, AchieveConfig* pAchieve)
: m_pScreen(pScreen)/*, m_pMouse(pMouse)*/, m_pConfig(pConfig), m_pAchieve(pAchieve)
#ifndef USE_GRAPHIC_YOU_WIN
, m_YouWinMessage(pScreen)
#endif
, m_Selector(pScreen, pConfig, &m_Metrics)
{
	MastermindLibCreate(&m_Master, Guessing, m_pConfig->GetHoles(), m_pConfig->GetColors(), m_pConfig->GetTries());

	m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);

	m_pBackground = nSDL_LoadImage(image_MastermindBoard);

	m_Metrics.SetBoardDimensions(m_pConfig->GetHoles(), m_pConfig->GetTries());
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

   for(int y=0; y<m_pConfig->GetTries(); y++) {
      nSDL_DrawString(m_pScreen, m_pFont, m_Metrics.GetXPos(0)-20, m_Metrics.GetYPos(y)+3, "%d", y+1);
      for(int x=0; x<m_pConfig->GetHoles(); x++) {
         int nPiece = GetMasterColorPeg(m_Master, y, x);
         //if( nPiece == 0 )
            //continue;
         DisplayPiece(m_Metrics.GetXPos(x), m_Metrics.GetYPos(y), nPiece);
      }

      int nRed, nWhite;
      GetMasterResult(m_Master, y, &nRed, &nWhite);
      DrawRedWhite(m_Metrics.GetXPos(m_pConfig->GetHoles()-1)+(m_pConfig->GetHoles()==4?m_Metrics.GetPieceSize():(m_Metrics.GetPieceSize()/2)), m_Metrics.GetYPos(y), nRed, nWhite);
   }

   int nX = SCREEN_WIDTH-m_Metrics.GetPieceSize()*4;
   int nY = SCREEN_HEIGHT-m_Metrics.GetPieceSize()*3-7;
   if( m_pConfig->GetColors() < 5 ) {
      nX += m_Metrics.GetPieceSize()/2;
   }
   else if( m_pConfig->GetColors() > 6 ) {
      nX -= m_Metrics.GetPieceSize()/2;
   }
   DisplayPiece(nX, nY, 1);
   DisplayPiece(nX+m_Metrics.GetPieceSize(), nY, 2);
   DisplayPiece(m_pConfig->GetColors() < 5 ? nX : (nX+m_Metrics.GetPieceSize()*2), m_pConfig->GetColors() < 5 ? (nY+m_Metrics.GetPieceSize()) : nY, 3);
   DisplayPiece(m_pConfig->GetColors() < 5 ? (nX+m_Metrics.GetPieceSize()) : nX, nY+m_Metrics.GetPieceSize(), 4);
   if( m_pConfig->GetColors() > 4 ) {
      DisplayPiece(nX+m_Metrics.GetPieceSize(), nY+m_Metrics.GetPieceSize(), 5);
   }
   if( m_pConfig->GetColors() > 5 ) {
      DisplayPiece(nX+m_Metrics.GetPieceSize()*2, nY+m_Metrics.GetPieceSize(), 6);
   }
   if( m_pConfig->GetColors() > 6 ) {
      DisplayPiece(nX+m_Metrics.GetPieceSize()*3, nY, 7);
   }
   if( m_pConfig->GetColors() > 7 ) {
      DisplayPiece(nX+m_Metrics.GetPieceSize()*3, nY+m_Metrics.GetPieceSize(), 8);
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
      if( n > m_pConfig->GetColors() )
         n = 0;
      PlaceMasterColorPeg(m_Master, m_Selector.GetCurrentX(), n);
   }
   if( eDirection == Down ) {
      int n = GetMasterColorPeg(m_Master, m_Selector.GetCurrentY(), m_Selector.GetCurrentX());
      n--;
      if( n < 0 )
         n = m_pConfig->GetColors();
      PlaceMasterColorPeg(m_Master, m_Selector.GetCurrentX(), n);
   }
}

void Game::EvaluatePegs()
{
   if( MASTERLIB_OK == TakeMasterGuess(m_Master, NULL, NULL) ) {
      m_Selector.IncrementRow();
      if( MASTERLIB_GAME_OVER == IsMasterGameOver(m_Master) ) {
#ifndef USE_GRAPHIC_YOU_WIN
         if( MASTERLIB_WON_GAME == GetMasterWonGame(m_Master) ) {
            m_YouWinMessage.CreateMessage("You Win!!!\n******");
         } else {
            m_YouWinMessage.CreateMessage("No Moves\nGame Over");
         }
#endif
	m_pAchieve->LookForAchievements(m_Master);
      }
   }
}

void Game::RemoveCurrentPeg()
{
   PlaceMasterColorPeg(m_Master, m_Selector.GetCurrentX(), 0);
}

int Game::GetRedWhiteX(int nTotal, int nX)
{
   int x = nX + nTotal*10;
   if( nTotal >= 4 && m_pConfig->GetHoles()>4 )
      x = nX + 5 + (nTotal-3)*10;
   return x;
}

int Game::GetRedWhiteY(int nTotal, int nY)
{
   int y = nY;
   if( nTotal >= 4 && m_pConfig->GetHoles()>4 )
      y += 10;
   return y;
}

void Game::DrawRedWhite(int nX, int nY, int nRed, int nWhite)
{
   int nTotal = 0;
   for(int i=0; i<nRed; i++) {
      nTotal++;
      filledEllipseRGBA(m_pScreen, GetRedWhiteX(nTotal, nX)+4, GetRedWhiteY(nTotal, nY)+6, 4, 4, 0, 0, 0, 255);
   }
   for(int i=0; i<nWhite; i++) {
      nTotal++;
      filledEllipseRGBA(m_pScreen, GetRedWhiteX(nTotal, nX)+4, GetRedWhiteY(nTotal, nY)+6, 4, 4, 255, 255, 255, 255);
   }
}

void Game::ShowSolution()
{
   int nX = m_Metrics.GetXPos(3) + 20*5+5;
   int nY = 50;
   DisplayPiece(nX, nY, GetMasterSolutionPeg(m_Master, 0));
   DisplayPiece(nX+m_Metrics.GetPieceSize(), nY, GetMasterSolutionPeg(m_Master, 1));
   DisplayPiece(nX+m_Metrics.GetPieceSize()*2, nY, GetMasterSolutionPeg(m_Master, 2));
   DisplayPiece(nX+m_Metrics.GetPieceSize()*3, nY, GetMasterSolutionPeg(m_Master, 3));
   if( m_pConfig->GetHoles() >= 5 )
      DisplayPiece(nX+m_Metrics.GetPieceSize()*4, nY, GetMasterSolutionPeg(m_Master, 4));
}

void Game::DisplayPiece(int nX, int nY, int nPiece)
{
   int nR, nG, nB;
   switch(nPiece) {
      default:
      case 0:
         nR = 208, nG = 208, nB = 208;
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

      case 7:
         nR = 255, nG = 255, nB = 255;
      break;

      case 8:
         nR = 0, nG = 0, nB = 0;
      break;
   }

   filledEllipseRGBA(m_pScreen, nX+7, nY+7, 7, 7, nR, nG, nB, 255);
}
