#ifndef METRICS_H
#define METRICS_H

extern "C"
{
        #include <os.h>
}

#define INDICATOR_PORTION	(0.5)

class Metrics
{
public:
   Metrics();
   bool SetBoardDimensions(int nWidth, int nHeight);
   int GetWidth() const;
   int GetHeight() const;
   int GetXPos(int nBoardX) const;
   int GetYPos(int nBoardY) const;
   int GetPieceSize() const;

protected:
   int m_nWidth;
   int m_nHeight;
};

#endif

