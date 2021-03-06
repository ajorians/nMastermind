#include "Menu.h"
#include "MouseHandling.h"
#include "PlayGraphic.h"
#include "AchieveConfig.h"
#include "StarGraphic.h"
#include "Defines.h"

MainMenu::MainMenu(SDL_Surface* pScreen, Config* pConfig, AchieveConfig* pAchieve, MouseHandling* pMouseHandling)
: m_pScreen(pScreen), m_pMouseHandling(pMouseHandling), m_Background(pScreen/*, pConfig*/), m_eChoice(Play), m_pConfig(pConfig), m_pAchieve(pAchieve), m_nFlashAchievement(0)
{
	m_pPlayGraphic		= nSDL_LoadImage(image_MenuGray);
	m_pStar                 = nSDL_LoadImage(image_Star);
	SDL_SetColorKey(m_pPlayGraphic, SDL_SRCCOLORKEY, SDL_MapRGB(m_pPlayGraphic->format, 255, 255, 255));
	SDL_SetColorKey(m_pStar, SDL_SRCCOLORKEY, SDL_MapRGB(m_pStar->format, 0, 0, 0));

	m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);

	m_bNewAchievement = pAchieve->GetNewAchievements();
}

MainMenu::~MainMenu()
{
	SDL_FreeSurface(m_pPlayGraphic);
	SDL_FreeSurface(m_pStar);
	nSDL_FreeFont(m_pFont);
}

bool MainMenu::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();

	rand();

	SDL_Delay(30);
	
	return true;
}

bool MainMenu::ShouldQuit() const
{
	return m_eChoice == Quit;
}

bool MainMenu::ShowShowOptions() const
{
	return m_eChoice == Options;
}

bool MainMenu::ShouldShowAchievements() const
{
        return m_eChoice == Achieve;
}

bool MainMenu::ShouldShowHelp() const
{
	return m_eChoice == Help;
}

bool MainMenu::PollEvents()
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
				printf( "Key press detected\n" );
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						fprintf(stderr, "Hit Escape!n");
						m_eChoice = Quit;
					return false;
					break;
					
					case SDLK_UP:
					case SDLK_8:
						if( m_eChoice == Help )
							m_eChoice = Options;
						else if( m_eChoice == Options )
							m_eChoice = Achieve;
						else if( m_eChoice == Achieve )
							m_eChoice = Play;
						break;
					
					case SDLK_DOWN:
					case SDLK_2:
						if( m_eChoice == Play )
							m_eChoice = Achieve;
						else if( m_eChoice == Achieve )
							m_eChoice = Options;
						else if( m_eChoice == Options )
							m_eChoice = Help;
						break;

					case SDLK_SPACE:
					case SDLK_RETURN:
					case SDLK_LCTRL:
						return false;
						break;
					
					default:
						break;
				}
				break;
			
			//Called when the mouse moves
			case SDL_MOUSEMOTION:
				break;
			
			case SDL_KEYUP:
				printf( "Key release detected\n" );
				break;
			
			default:
				break;
		}
	}

	int nMX = -1, nMY = -1;
	if( m_pMouseHandling->PollMouse(nMX, nMY) ) {
		MouseButton eMouseButton = m_pMouseHandling->GetMouseButton();
		if( eMouseButton == CenterButton ) {
			return false;
		}
	}

	return true;
}

void MainMenu::UpdateDisplay()
{
	//Draw background
	if( is_classic ) {
		SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 255, 255, 255));
		nSDL_DrawString(m_pScreen, m_pFont, SCREEN_WIDTH/2-30, 15, "nMastermind");
		nSDL_DrawString(m_pScreen, m_pFont, 10, 10, "Play");
		nSDL_DrawString(m_pScreen, m_pFont, 10, 35, "Achievements");
		nSDL_DrawString(m_pScreen, m_pFont, 10, 52, "Options");
		nSDL_DrawString(m_pScreen, m_pFont, 10, 74, "Help");
	} else {
		m_Background.DrawBackground();

		SDL_Rect rectPlay;
                rectPlay.x = 10;//(SCREEN_WIDTH - m_pPlayGraphic->w)/2;
                rectPlay.y = 10;//76;
                rectPlay.w = m_pPlayGraphic->w;
                rectPlay.h = m_pPlayGraphic->h;
                SDL_BlitSurface(m_pPlayGraphic, NULL, m_pScreen, &rectPlay);
	}

	if( m_eChoice == Play )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 0, 7, 60, 23, 1);
	else if( m_eChoice == Achieve )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 0, 29, 142, 23, 1);
	else if( m_eChoice == Options )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 0, 48, 96, 24, 1);
	if( m_eChoice == Help )
		draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 0, 70, 60, 25, 1);

	 m_nFlashAchievement = (m_nFlashAchievement+1)%100;
        if( m_bNewAchievement && (m_nFlashAchievement<50) ) {
                SDL_Rect rectStar;
                rectStar.x = 141;
                rectStar.y = 29;
                rectStar.w = 16;
                rectStar.h = 16;
                SDL_BlitSurface(m_pStar, NULL, m_pScreen, &rectStar);
        }

	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}



