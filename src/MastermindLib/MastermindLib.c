//Public domain :)

#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif
#include "MastermindLib/MastermindLib.h"
#include "Defines.h"

struct Mastermind
{
   int m_nLastError;
};

int MastermindLibCreate(MasterLib* api)
{
   struct MasterMind* pM;
   DEBUG_FUNC_NAME;

   pM = malloc(sizeof(struct Mastermind));
   if( pM == NULL ){//Out of memory
      return MASTERLIB_OUT_OF_MEMORY;
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
   pM->m_nLastError = HEARTSLIB_OK;
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

