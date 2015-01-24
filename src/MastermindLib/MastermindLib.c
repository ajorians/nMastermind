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
  short m_nNumWhite;
  short m_nNumBlack;
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
/*int GetHeartsGameOver(MasterLib api)
{
   struct Hearts* pH;
   int nPlayerIndex;
   DEBUG_FUNC_NAME;

   pH = (struct Hearts*)api;

   for(nPlayerIndex=0; nPlayerIndex < GetNumHeartsPlayers(api); nPlayerIndex++) {
      if( GetHeartsPlayerScore(api, nPlayerIndex) >= pH->m_nScoreLimit )
         return HEARTSLIB_GAME_OVER;
   }

   return HEARTSLIB_STILL_PLAYING;
}*/

