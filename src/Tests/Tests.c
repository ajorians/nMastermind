#ifdef _TINSPIRE
#include <nspireio.h>
#else
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#endif
#include "MastermindLib/MastermindLib.h"

#ifdef _TINSPIRE
#define PRINT  nio_printf
#else
#define PRINT  printf
#endif

#ifdef _TINSPIRE
#define PRINT_FUNC PRINT("%s", __func__);
#else
#define PRINT_FUNC PRINT("%s", __FUNCTION__);
#endif

#define TEST_SUCCEEDED	(0)
#define TEST_FAILED	(1)

#define TEST_PERSISTING

#ifdef _TINSPIRE
int g_nRelocatedGlobals = 0;
#endif

int TestConstruction()
{
   MasterLib api;
   PRINT_FUNC;
   if( MASTERLIB_OK != MastermindLibCreate(&api, Guessing, 4, 6, 10) )
      return TEST_FAILED;

   if( MASTERLIB_OK != MastermindLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

/*int TestCopy()
{
   HeartsLib api;
   int i;
   HeartsLib copy;
   PRINT_FUNC;
   if( HEARTSLIB_OK != HeartsLibCreate(&api, 100, 0, Normal4Players) )
      return TEST_FAILED;

   if( HEARTSLIB_OK != HeartsLibCopy(&copy, api) )
      return TEST_FAILED;

   if( HEARTSLIB_OK != HeartsLibFree(&copy) )
      return TEST_FAILED;

   if( HEARTSLIB_OK != HeartsLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}*/

int TestCreate()
{
   MasterLib api;
   PRINT_FUNC;
   if( MASTERLIB_OK != MastermindLibCreate(&api, Creator, 4, 6, 10) )
      return TEST_FAILED;
   
   int arrAnswer[] = {1,2,3,4};
   if( MASTERLIB_OK != SetMasterCreatorAnswer(api, 4, arrAnswer) )
      return TEST_FAILED;

   if( MASTERLIB_OK != MastermindLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestPlacePegs()
{
   MasterLib api;
   int i;
   PRINT_FUNC;
   if( MASTERLIB_OK != MastermindLibCreate(&api, Creator, 4, 6, 10) )
      return TEST_FAILED;
   
   int arrAnswer[] = {1,2,3,4};
   if( MASTERLIB_OK != SetMasterCreatorAnswer(api, 4, arrAnswer) )
      return TEST_FAILED;
   
   for(i=0; i<4; i++)
      if( MASTERLIB_OK != PlaceMasterColorPeg(api, i, 0) )
         return TEST_FAILED;
      
   for(i=0; i<4; i++)
      if( MASTERLIB_OK != PlaceMasterColorPeg(api, i, i+1) )
         return TEST_FAILED;

   if( MASTERLIB_OK != MastermindLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestGuessing1()
{
   MasterLib api;
   int nReds, nWhites;
   PRINT_FUNC;
   
   nReds = 0, nWhites = 0;
   
   if( MASTERLIB_OK != MastermindLibCreate(&api, Creator, 4, 6, 10) )
      return TEST_FAILED;
   
   int arrAnswer[] = {1,2,3,4};
   if( MASTERLIB_OK != SetMasterCreatorAnswer(api, 4, arrAnswer) )
      return TEST_FAILED;
   
   if( MASTERLIB_OK == TakeMasterGuess(api, &nReds, &nWhites) )//Can take guess until laid out pegs
         return TEST_FAILED;
   
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 0, 4) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 1, 3) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 2, 2) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 3, 1) )
         return TEST_FAILED;
   
   if( MASTERLIB_OK != TakeMasterGuess(api, &nReds, &nWhites) )
         return TEST_FAILED;
   
   if( nReds != 0 )
      return TEST_FAILED;
   
   if( nWhites != 4 )
      return TEST_FAILED;
   
   ///
   
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 0, 1) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 1, 2) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 2, 4) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 3, 3) )
         return TEST_FAILED;
   
   if( MASTERLIB_OK != TakeMasterGuess(api, &nReds, &nWhites) )
         return TEST_FAILED;
   
   if( nReds != 2 )
      return TEST_FAILED;
   
   if( nWhites != 2 )
      return TEST_FAILED;
   
   ///
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 0, 1) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 1, 3) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 2, 3) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 3, 4) )
         return TEST_FAILED;
   
   if( MASTERLIB_OK != TakeMasterGuess(api, &nReds, &nWhites) )
         return TEST_FAILED;
   
   if( nReds != 3 )
      return TEST_FAILED;
   
   if( nWhites != 0 )
      return TEST_FAILED;

   if( MASTERLIB_OK != MastermindLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestGuessing2()
{
   MasterLib api;
   int nReds, nWhites;
   PRINT_FUNC;
   
   nReds = 0, nWhites = 0;
   
   if( MASTERLIB_OK != MastermindLibCreate(&api, Creator, 4, 6, 10) )
      return TEST_FAILED;
   
   int arrAnswer[] = {1,3,3,4};
   if( MASTERLIB_OK != SetMasterCreatorAnswer(api, 4, arrAnswer) )
      return TEST_FAILED;
   
   if( MASTERLIB_OK == TakeMasterGuess(api, &nReds, &nWhites) )//Can take guess until laid out pegs
         return TEST_FAILED;
   
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 0, 3) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 1, 3) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 2, 5) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 3, 5) )
         return TEST_FAILED;
   
   if( MASTERLIB_OK != TakeMasterGuess(api, &nReds, &nWhites) )
         return TEST_FAILED;
   
   if( nReds != 1 )
      return TEST_FAILED;
   
   if( nWhites != 1 )
      return TEST_FAILED;

   if( MASTERLIB_OK != MastermindLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

int TestGuessing3()
{
   MasterLib api;
   int nReds, nWhites;
   PRINT_FUNC;

   nReds = 0, nWhites = 0;

   if( MASTERLIB_OK != MastermindLibCreate(&api, Creator, 4, 6, 10) )
      return TEST_FAILED;

   int arrAnswer[] = {5,2,1,2};
   if( MASTERLIB_OK != SetMasterCreatorAnswer(api, 4, arrAnswer) )
      return TEST_FAILED;

   if( MASTERLIB_OK == TakeMasterGuess(api, &nReds, &nWhites) )//Can't take guess until laid out pegs
         return TEST_FAILED;

   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 0, 1) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 1, 1) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 2, 2) )
         return TEST_FAILED;
   if( MASTERLIB_OK != PlaceMasterColorPeg(api, 3, 5) )
         return TEST_FAILED;

   if( MASTERLIB_OK != TakeMasterGuess(api, &nReds, &nWhites) )
         return TEST_FAILED;

   if( nReds != 0 )
      return TEST_FAILED;

   if( nWhites != 3 )
      return TEST_FAILED;

   if( MASTERLIB_OK != MastermindLibFree(&api) )
      return TEST_FAILED;

   return TEST_SUCCEEDED;
}

typedef int (*testfunc)();
   testfunc g_Tests[] =
   {
      TestConstruction,
      //TestCopy,
      TestCreate,
      TestPlacePegs,
      TestGuessing1,
      TestGuessing2,
      TestGuessing3
   };

void RunTests()
{
#ifdef _TINSPIRE
   if( !g_nRelocatedGlobals ) {
      nl_relocdata((unsigned*)g_Tests, sizeof(g_Tests)/sizeof(g_Tests[0]));
      g_nRelocatedGlobals = 1;
   }
#endif

   unsigned i;
   for(i=0; i<sizeof(g_Tests)/sizeof(testfunc); i++) {

      int nRet = g_Tests[i]();
      if( nRet == TEST_SUCCEEDED ) {
         PRINT("........OK\n");
      } else {
         PRINT("........FAIL\n");
      }
   }
}


