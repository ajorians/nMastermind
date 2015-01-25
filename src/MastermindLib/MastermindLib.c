//Public domain :)

#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif
#include "MastermindLib/MastermindLib.h"
#include "Defines.h"

struct MastermindRow
{
  int m_aGuessSpots[5];
  short m_nNumRed;
  short m_nNumWhite;
};

typedef enum {
   GamePlaying,
   GameLost,
   GameWon
} GameState;

struct Mastermind
{
   int m_nLastError;
   PlayMode m_eMode;
   int m_nHoles;
   int m_nColors;
   int m_nTries;
   struct MastermindRow* m_paRows;
   int m_nCurrentRow;
   int m_aAnswer[5];
   GameState m_eState;
};

int MastermindLibCreate(MasterLib* api, PlayMode eMode, int nHoles, int nColors, int nTries)
{
   struct Mastermind* pM;
   int i,j;
   DEBUG_FUNC_NAME;

   pM = malloc(sizeof(struct Mastermind));
   if( pM == NULL ){//Out of memory
      return MASTERLIB_OUT_OF_MEMORY;
   }

   pM->m_eMode = eMode;
   pM->m_nHoles = nHoles;
   pM->m_nColors = nColors;
   pM->m_nTries = nTries;
   pM->m_paRows = malloc(nTries*sizeof(struct MastermindRow));
   if( pM->m_paRows == NULL ){//Out of memory
      free(pM);
      return MASTERLIB_OUT_OF_MEMORY;
   }
   pM->m_nCurrentRow = 0;
   for(i=0; i<nTries; i++)
     for(j=0; j<5; j++) {
       pM->m_paRows[i].m_aGuessSpots[j] = 0;
         pM->m_paRows[i].m_nNumRed = pM->m_paRows[i].m_nNumWhite = 0;
      }
     
   if( pM->m_eMode == Guessing ) {
     pM->m_aAnswer[0] = (rand() % nColors) + 1;
     pM->m_aAnswer[1] = (rand() % nColors) + 1;
     pM->m_aAnswer[2] = (rand() % nColors) + 1;
     pM->m_aAnswer[3] = (rand() % nColors) + 1;
     pM->m_aAnswer[4] = (rand() % nColors) + 1;
     printf("Solution: %d, %d, %d, %d\n", pM->m_aAnswer[0], pM->m_aAnswer[1], pM->m_aAnswer[2], pM->m_aAnswer[3]);
   }
   pM->m_eState = GamePlaying;
   pM->m_nLastError = MASTERLIB_OK;

   *api = pM;

   return MASTERLIB_OK;
}

/*int MastermindLibCopy(MasterLib* copyapi, MasterLib orig)
{
   struct Mastermind* pMOrig;
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pMOrig = (struct Mastermind*)orig;

   pM = malloc(sizeof(struct Mastermind));
   if( pM == NULL ){//Out of memory
      return MASTERLIB_OUT_OF_MEMORY;
   }

   pM->m_nLastError = pMOrig->m_nLastError;

   *copyapi = pM;
   return MASTERLIB_OK;
}*/

int MastermindLibFree(MasterLib* api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = *api;
   free(pM->m_paRows);

   free(pM);
   *api = NULL;
   return MASTERLIB_OK;
}

int GetMastermindLibError(MasterLib api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   return pM->m_nLastError;
}

void ClearMastermindLibError(MasterLib api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   pM->m_nLastError = MASTERLIB_OK;
}

//MastermindLib related functions
int SetMasterCreatorAnswer(MasterLib api, int nCount, int arrValues[])
{
  struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   
   if( pM->m_eMode == Guessing )
     return MASTERLIB_BADARGUMENT;
   
   pM->m_aAnswer[0] = arrValues[0];
   pM->m_aAnswer[1] = arrValues[1];
   pM->m_aAnswer[2] = arrValues[2];
   pM->m_aAnswer[3] = arrValues[3];
   if( nCount >= 5 )
      pM->m_aAnswer[4] = arrValues[4];
   
  return MASTERLIB_OK;
}

int GetMasterColorPeg(MasterLib api, int nRow, int nCol)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;

   return pM->m_paRows[nRow].m_aGuessSpots[nCol];
}

int GetMasterResult(MasterLib api, int nRow, int* pnRed, int* pnWhite)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;

   if( pnRed ) *pnRed = pM->m_paRows[nRow].m_nNumRed;
   if( pnWhite ) *pnWhite = pM->m_paRows[nRow].m_nNumWhite;

   return MASTERLIB_OK;
}

int PlaceMasterColorPeg(MasterLib api, int nSpot, int nColor)
{
  struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   if( IsMasterGameOver(api) != GamePlaying )
      return MASTERLIB_BADARGUMENT;

   pM = (struct Mastermind*)api;

   //printf("row: %d, spot: %d with color %d\n", pM->m_nCurrentRow, nSpot, nColor);
   
   if( nSpot < 0 || nSpot > (pM->m_nHoles-1) )
     return MASTERLIB_BADARGUMENT;
   
   if( nColor < 0 | nColor > pM->m_nColors )
     return MASTERLIB_BADARGUMENT;
   
   pM->m_paRows[pM->m_nCurrentRow].m_aGuessSpots[nSpot] = nColor;
   
   return MASTERLIB_OK;
}

int TakeMasterGuess(MasterLib api, int* pnReds, int* pnWhites)
{
   struct Mastermind* pM;
   int i, j;
   int nReds, nWhites;
   DEBUG_FUNC_NAME;

   if( IsMasterGameOver(api) != GamePlaying )
      return MASTERLIB_BADARGUMENT;

   pM = (struct Mastermind*)api;
   
   nReds = 0, nWhites = 0;
   
   for(i=0; i<pM->m_nHoles; i++)
     if( pM->m_paRows[pM->m_nCurrentRow].m_aGuessSpots[i] == 0 )
       return MASTERLIB_NOT_FILLED_YET;
     
   int arrCounted[] = { 0,0,0,0,0 };
   //Look for Red & White pegs
   for(i=0; i<pM->m_nHoles; i++) {
      if( pM->m_paRows[pM->m_nCurrentRow].m_aGuessSpots[i] == pM->m_aAnswer[i] ) {
         nReds++;
         arrCounted[i] = 1;
      }
      else {
         for(j=0; j<pM->m_nHoles; j++) {
            if( i == j )
               continue;
            if( pM->m_paRows[pM->m_nCurrentRow].m_aGuessSpots[j] != pM->m_aAnswer[j] && pM->m_paRows[pM->m_nCurrentRow].m_aGuessSpots[j] == pM->m_aAnswer[i] && arrCounted[j] == 0 ) {
               arrCounted[j] = 1;
               nWhites++;
               break;
            }
         }
      }
   }
   printf("Reds: %d, Whites: %d\n", nReds, nWhites);
   
   pM->m_paRows[pM->m_nCurrentRow].m_nNumRed = nReds;
   pM->m_paRows[pM->m_nCurrentRow].m_nNumWhite = nWhites;
 
   pM->m_nCurrentRow++;

   if( nReds == pM->m_nHoles )
      pM->m_eState = GameWon;
   else if( pM->m_nCurrentRow >= pM->m_nTries )
      pM->m_eState = GameLost;
   
   if( pnReds ) *pnReds = nReds;
   if( pnWhites ) *pnWhites = nWhites;
   
   return MASTERLIB_OK;
}

int IsMasterGameOver(MasterLib api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;

   if( pM->m_eState == GameLost || pM->m_eState == GameWon )
      return MASTERLIB_GAME_OVER;

   return MASTERLIB_STILL_PLAYING;
}

int GetMasterWonGame(MasterLib api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   if( pM->m_eState == GameLost )
      return MASTERLIB_LOST_GAME;

   if( pM->m_eState == GameWon )
      return MASTERLIB_WON_GAME;

   return 2;//TODO: Add define
}

int GetMasterSolutionPeg(MasterLib api, int nSpot)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;

   if( nSpot < 0 || nSpot > (pM->m_nHoles-1) )
      return MASTERLIB_BADARGUMENT;

   return pM->m_aAnswer[nSpot];
}

int GetCurrentTry(MasterLib api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   return pM->m_nCurrentRow;
}
