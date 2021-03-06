#ifndef MASTERMINDLIB_H_INCLUDED
#define MASTERMINDLIB_H_INCLUDED

typedef void* MasterLib;

#define MASTERLIB_OK			(0)
#define MASTERLIB_BADARGUMENT		(-1)
#define MASTERLIB_OUT_OF_MEMORY		(-2)

//Take a guess returns
#define MASTERLIB_NOT_FILLED_YET	(-1)

#define MASTERLIB_STILL_PLAYING         (0)
#define MASTERLIB_GAME_OVER             (1)

#define MASTERLIB_LOST_GAME		(0)
#define MASTERLIB_WON_GAME		(1)

typedef enum {
  Guessing,
  Creator
} PlayMode;

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int MastermindLibCreate(MasterLib* api, PlayMode eMode, int nHoles, int nColors, int nTries);
//int MastermindLibCopy(MasterLib* copyapi, MasterLib orig);
int MastermindLibFree(MasterLib* api);

int GetMasterLibError(MasterLib api);
void ClearMasterLibError(MasterLib api);

//////////////////////////////////////////////
//MasterLib related functions
//////////////////////////////////////////////
int SetMasterCreatorAnswer(MasterLib api, int nCount, int arrValues[]);
int GetMasterColorPeg(MasterLib, int nRow, int nCol);
int GetMasterResult(MasterLib api, int nRow, int* pnRed, int* pnWhite);
int PlaceMasterColorPeg(MasterLib, int nSpot, int nColor);
int TakeMasterGuess(MasterLib api, int* pnReds, int* pnWhites);
int IsMasterGameOver(MasterLib api);
int GetMasterWonGame(MasterLib api);
int GetMasterSolutionPeg(MasterLib api, int nSpot);
int GetCurrentTry(MasterLib api);

#endif
