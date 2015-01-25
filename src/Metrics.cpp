#include "Metrics.h"

#define SIZE_PIECE	(20)

Metrics::Metrics()
{
}

bool Metrics::SetBoardDimensions(int nWidth, int nHeight)
{
   m_nWidth = nWidth;
   m_nHeight = nHeight;

   return true;
}

int Metrics::GetWidth() const { return m_nWidth; }
int Metrics::GetHeight() const { return m_nHeight; }

int Metrics::GetXPos(int nBoardX) const
{
   return 50 + nBoardX*SIZE_PIECE;
}

int Metrics::GetYPos(int nBoardY) const
{
   return (SCREEN_HEIGHT-nBoardY*SIZE_PIECE)-20;
}

int Metrics::GetPieceSize() const
{
   return SIZE_PIECE;
}


