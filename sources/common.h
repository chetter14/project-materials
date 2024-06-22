// common.h

#ifndef _COMMON_H_511DE8DC_DFC3_42bf_85F5_2CDDF53F78F1_
#define _COMMON_H_511DE8DC_DFC3_42bf_85F5_2CDDF53F78F1_

namespace ctm {
namespace pradis {
namespace plugin {

// the structure contains a members of common scopes
// from the itgdll
struct Common {
    double * TIME;
    double * STEP;
    double * STEP01;
    double * STEP02;
    double * SMIN;
    double * DABSI;
    double * DRLTI;
    double * STEPMD;
    double * TIMEND;
    char* NAME;
    int* nLength;
    int * NSTEP;
    int * SYSPRN;
    int * NITER;
    int * ITR;
    short * CODE;
    short * NUMINT;
    short * NUMPP;
    short * CODSTP;
    short * CODGRF;
    short * NEWINT;
    short * MINSTP;
    double * RLMAX;
    double * RLMIN;
    double * MSHEPS;
    double * INTMAX;
    double * PI;
    double * REZB;
    double * REZC;
    double * REZD;        
    double * RELYX;
    int * XNMPXL;
    int * YNMPXL;
    int * XNMSMB;
    int * YNMSMB;
    int * NCOLOR;
    int * NMVPAG;
    int * MODES;
    int * IK4;
    int * IS4;
};

}
}
}

#define PLUGIN_DECLARE_COMMON extern ctm::pradis::plugin::Common gSolverCommon;
#define PLUGIN_COMMON gSolverCommon

#endif