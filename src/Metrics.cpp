#include "Metrics.h"
extern "C"
{
        //#include "BinaryLib/BinaryLib.h"
}

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
   return 20 + nBoardX*15;
}

int Metrics::GetYPos(int nBoardY) const
{
   return (SCREEN_HEIGHT-nBoardY*15)-20;
}

int Metrics::GetPieceSize() const
{
   return 15;
}


