#ifndef MASTERMINDLIB_H_INCLUDED
#define MASTERMINDLIB_H_INCLUDED

typedef void* MasterLib;

#define MASTERLIB_OK			(0)
#define MASTERLIB_BADARGUMENT		(-1)
#define MASTERLIB_OUT_OF_MEMORY		(-2)

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int MastermindLibCreate(MasterLib* api);
//int MastermindLibCopy(MasterLib* copyapi, MasterLib orig);
int MastermindLibFree(MasterLib* api);

int GetMasterLibError(MasterLib api);
void ClearMasterLibError(MasterLib api);

//////////////////////////////////////////////
//MasterLib related functions
//////////////////////////////////////////////
//int GetHeartsPlayerScore(MasterLib api, int nPlayerIndex);

#endif
