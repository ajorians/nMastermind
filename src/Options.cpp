#include "Options.h"
//#include "MouseHandling.h"
#include "Defines.h"

Options::Options(SDL_Surface* pScreen/*, MouseHandling* pMouse*/, Config* pConfig)
: m_pScreen(pScreen)/*, m_pMouse(pMouse)*/, m_nOptionsIndex(0), m_pConfig(pConfig)
{
	m_pFont = nSDL_LoadFont(NSDL_FONT_TINYTYPE, 0, 0, 0);
	nSDL_SetFontSpacing(m_pFont, 0, 2);
}

Options::~Options()
{
	nSDL_FreeFont(m_pFont);
}

bool Options::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();
	
	return true;
}

bool Options::PollEvents()
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
						return false;
					break;

					case SDLK_SPACE:
                                        case SDLK_RETURN:
                                        case SDLK_LCTRL:
						ToggleCurrentOption();
                                                break;

					case SDLK_UP:
                                        case SDLK_8:
                                                Move(OP_Up);
                                                break;

                                        case SDLK_DOWN:
                                        case SDLK_2:
                                                Move(OP_Down);
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
			ToggleCurrentOption();
                }
	}*/

	return true;
}

void Options::ToggleCurrentOption()
{
	if( m_nOptionsIndex == 0 ) {
		m_pConfig->SetHoles(m_pConfig->GetHoles()==4 ? 5 : 4);
	} else if( m_nOptionsIndex == 1 ) {
		int nNewColors = 6;
		switch(m_pConfig->GetColors())
		{
			case 4:
				nNewColors = 5;
			break;

			case 5:
				nNewColors = 6;
			break;

			default:
			case 6:
				nNewColors = 7;
			break;

			case 7:
				nNewColors = 8;
			break;

			case 8:
				nNewColors = 4;
			break;
		}
		m_pConfig->SetColors(nNewColors);
	} else if( m_nOptionsIndex == 2 ) {
		m_pConfig->SetTries(m_pConfig->GetTries() == 10 ? 12 : 10);
        }
}

void Options::Move(Option_Direction eDirection)
{
	if( eDirection == OP_Down && m_nOptionsIndex < 2 ) {
		m_nOptionsIndex++;
	} else if( eDirection == OP_Up && m_nOptionsIndex > 0 ) {
		m_nOptionsIndex--;
	}
}

#define HOLES_Y	      (45)
#define COLORS_Y      (65)
#define TRIES_Y	      (85)
void Options::UpdateDisplay()
{
	//Draw background
	SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 153, 153, 255));
	nSDL_DrawString(m_pScreen, m_pFont, (SCREEN_WIDTH-nSDL_GetStringWidth(m_pFont, "Options:"))/2, 15, "Options:");

	nSDL_DrawString(m_pScreen, m_pFont, 12, HOLES_Y, "Number of holes: %d", m_pConfig->GetHoles());

	if( m_nOptionsIndex == 0 ) {
                if( is_classic ) {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 255, 255), 7, HOLES_Y-5, 312, 25, 1);
                } else {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 7, HOLES_Y-5, 312, 25, 1);
                }
        }

	nSDL_DrawString(m_pScreen, m_pFont, 12, COLORS_Y, "Number of colors: %d", m_pConfig->GetColors());

	if( m_nOptionsIndex == 1 ) {
                if( is_classic ) {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 255, 255), 7, COLORS_Y-5, 312, 25, 1);
                } else {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 7, COLORS_Y-5, 312, 25, 1);
                }
        }

	nSDL_DrawString(m_pScreen, m_pFont, 12, TRIES_Y, "Number of tries: %d", m_pConfig->GetTries());

	if( m_nOptionsIndex == 2 ) {
                if( is_classic ) {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 255, 255), 7, TRIES_Y-5, 312, 25, 1);
                } else {
                        draw_rectangle(m_pScreen, SDL_MapRGB(m_pScreen->format, 255, 0, 0), 7, TRIES_Y-5, 312, 25, 1);
                }
        }

	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}




