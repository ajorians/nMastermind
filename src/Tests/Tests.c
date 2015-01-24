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
   int i;
   PRINT_FUNC;
   if( MASTERLIB_OK != MastermindLibCreate(&api) )
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

typedef int (*testfunc)();
   testfunc g_Tests[] =
   {
      TestConstruction,
      //TestCopy,
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


