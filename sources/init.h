// init.h

#ifndef _INIT_H_99D3B011_B5CC_41bb_BEB9_04F593ECDBB6_
#define _INIT_H_99D3B011_B5CC_41bb_BEB9_04F593ECDBB6_

#ifdef _WIN32                // Platform
    #define PLUGIN_INIT_CC __stdcall
    #define PLUGIN_INIT_API _declspec( dllexport )
#elif defined __unix__        // Platform
    #define PLUGIN_INIT_CC
    #define PLUGIN_INIT_API
#else                       // Platform
    #error "Unsupported platform"
#endif

#include "common.h"

ctm::pradis::plugin::Common gSolverCommon;

extern "C" PLUGIN_INIT_API void PLUGIN_INIT_CC INIT(
    double * TIME,
    double * STEP,
    double * STEP01,
    double * STEP02,
    double * SMIN,
    double * DABSI,
    double * DRLTI,
    double * STEPMD,
    double * TIMEND,
    char* NAME,
    int* nLength,
    int * NSTEP,
    int * SYSPRN,
    int * NITER,
    int * ITR,
    short * CODE,
    short * NUMINT,
    short * NUMPP,
    short * CODSTP,
    short * CODGRF,
    short * NEWINT,
    short * MINSTP,
    double * RLMAX,
    double * RLMIN,
    double * MSHEPS,
    double * INTMAX,
    double * PI,
    double * REZB,
    double * REZC,
    double * REZD,
    double * RELYX,
    int * XNMPXL,
    int * YNMPXL,
    int * XNMSMB,
    int * YNMSMB,
    int * NCOLOR,
    int * NMVPAG,
    int * MODES,
    int * IK4,
    int * IS4) {

    ctm::pradis::plugin::Common com = { TIME,
            STEP,STEP01,STEP02,SMIN,DABSI,DRLTI,STEPMD,TIMEND,
            NAME,nLength,NSTEP,SYSPRN,NITER,ITR,CODE,NUMINT,
            NUMPP,CODSTP,CODGRF,NEWINT,MINSTP,RLMAX,RLMIN,
            MSHEPS,INTMAX,PI,REZB,REZC,REZD,RELYX,XNMPXL,
            YNMPXL,XNMSMB,YNMSMB,NCOLOR,NMVPAG,MODES,IK4,IS4 };

    PLUGIN_COMMON = com;

}

#endif