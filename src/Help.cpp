#include "Help.h"

MastermindHelp::MastermindHelp(SDL_Surface* pScreen)
: m_pScreen(pScreen)
{
	m_pFont = nSDL_LoadFont(NSDL_FONT_VGA, 0, 0, 0);

}

MastermindHelp::~MastermindHelp()
{
	nSDL_FreeFont(m_pFont);
}

bool MastermindHelp::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();
	
	return true;
}

bool MastermindHelp::PollEvents()
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
					case SDLK_RETURN:
					case SDLK_SPACE:
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
				break;
			
			default:
				break;
		}
	}
	return true;
}

void MastermindHelp::UpdateDisplay()
{
	SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 153, 153, 255));

	nSDL_DrawString(m_pScreen, m_pFont, 15, 20, 
"Mastermind is a critical thinking game.\n\
The object is to find out the color of the pegs\n\
and the order they are in.  You do this by trying\n\
various combinations of color pegs.\n\
\n\
The resulting white and black indicators mean\n\
particular things.  The black indicator means\n\
the a peg is both the correct color and in the\n\
correct position.  The white indicator means\n\
the color was correct but is in the wrong position.\n\
No indicators mean all pegs are wrong.\n\
\n\
Based on only that you need to solve this!\n\
\n\
Good luck!");

	//int nLeft = 55;

	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}




