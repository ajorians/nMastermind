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
  int m_aGuessSpots[4];
  short m_nNumRed;
  short m_nNumWhite;
};

struct Mastermind
{
   int m_nLastError;
   PlayMode m_eMode;
   struct MastermindRow m_aRows[10];
   int m_nCurrentRow;
   int m_aAnswer[4];
};

int MastermindLibCreate(MasterLib* api, PlayMode eMode)
{
   struct Mastermind* pM;
   int i,j;
   DEBUG_FUNC_NAME;

   pM = malloc(sizeof(struct Mastermind));
   if( pM == NULL ){//Out of memory
      return MASTERLIB_OUT_OF_MEMORY;
   }

   pM->m_eMode = eMode;
   pM->m_nCurrentRow = 0;
   for(i=0; i<10; i++)
     for(j=0; j<4; j++)
       pM->m_aRows[i].m_aGuessSpots[j] = 0;
     
   if( pM->m_eMode == Guessing ) {
     pM->m_aAnswer[0] = (rand() % 5) + 1;
     pM->m_aAnswer[1] = (rand() % 5) + 1;
     pM->m_aAnswer[2] = (rand() % 5) + 1;
     pM->m_aAnswer[3] = (rand() % 5) + 1;
   }
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
   
  return MASTERLIB_OK;
}

int PlaceMasterColorPeg(MasterLib api, int nSpot, int nColor)
{
  struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   
   if( nSpot < 0 || nSpot > 3 )
     return MASTERLIB_BADARGUMENT;
   
   if( nColor < 0 | nColor > 6 )//0 is ok and hard coded 6 for now
     return MASTERLIB_BADARGUMENT;
   
   pM->m_aRows[pM->m_nCurrentRow].m_aGuessSpots[nSpot] = nColor;
   
   return MASTERLIB_OK;
}

int TakeMasterGuess(MasterLib api, int* pnReds, int* pnWhites)
{
   struct Mastermind* pM;
   int i, j;
   int nReds, nWhites;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;
   
   nReds = 0, nWhites = 0;
   
   for(i=0; i<4; i++)
     if( pM->m_aRows[pM->m_nCurrentRow].m_aGuessSpots[i] == 0 )
       return MASTERLIB_NOT_FILLED_YET;
     
   int arrCounted[] = { 0,0,0,0 };
   //Look for reds
   for(i=0; i<4; i++) {
      if( pM->m_aRows[pM->m_nCurrentRow].m_aGuessSpots[i] == pM->m_aAnswer[i] ) {
         nReds++;
         arrCounted[i] = 1;
      }
   }
   
   //Look for whites
   for(i=0; i<4; i++) {
      for(j=0; j<4; j++) {
         if( i == j )
            continue;
         if( pM->m_aRows[pM->m_nCurrentRow].m_aGuessSpots[i] == pM->m_aAnswer[j] && arrCounted[j] == 0 ) {
            nWhites++;
            arrCounted[j] = 1;
         }
      }
   }
   
   pM->m_aRows[pM->m_nCurrentRow].m_nNumRed = nReds;
   pM->m_aRows[pM->m_nCurrentRow].m_nNumWhite = nWhites;
 
   pM->m_nCurrentRow++;
   
   if( pnReds ) *pnReds = nReds;
   if( pnWhites ) *pnWhites = nWhites;
   
   return MASTERLIB_OK;
}

int IsMasterGameOver(MasterLib api)
{
   struct Mastermind* pM;
   DEBUG_FUNC_NAME;

   pM = (struct Mastermind*)api;

   return MASTERLIB_STILL_PLAYING;
}

