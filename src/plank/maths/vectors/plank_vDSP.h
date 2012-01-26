/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-12
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of University of the West of England, Bristol nor 
   the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL UNIVERSITY OF THE WEST OF ENGLAND, BRISTOL BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 
 This software makes use of third party libraries. For more information see:
 doc/license.txt included in the distribution.
 -------------------------------------------------------------------------------
 */

#ifndef PLANK_VDSP_H
#define PLANK_VDSP_H

#if !DOXYGEN

#ifdef PLANK_VEC_CUSTOM
    #error only one custom vectorised libary may be specified
#endif

#define PLANK_VEC_CUSTOM
//#include "plank_Vectors.h"
#include <Accelerate/Accelerate.h>

//------------------------------- float ----------------------------------------

static inline void pl_VectorFillF_N1 (float *result, float a, PlankUL N) 
{ 
    vDSP_vfill (&a, result, 1, N); 
}

static inline void pl_VectorClearF_N (float *result, PlankUL N) 
{ 
    vDSP_vclr (result, 1, N);
}

static inline void pl_VectorRampF_N1 (float *result, float a, float b, PlankUL N) 
{ 
    vDSP_vramp (&a, &b, result, 1, N); 
}

static inline void pl_VectorLineF_N1 (float *result, float a, float b, PlankUL N) 
{ 
    vDSP_vgen (&a, &b, result, 1, N); 
}

static inline void pl_VectorMoveF_NN (float *result, const float* a, PlankUL N) 
{ 
    vDSP_mmov ((float*)a, result, N, 1, N, N); 
}

static inline void pl_VectorNegF_NN (float *result, const float* a, PlankUL N) 
{ 
    vDSP_vneg ((float*)a, 1, result, 1, N); 
}

static inline void pl_VectorAbsF_NN (float *result, const float* a, PlankUL N) 
{ 
    vDSP_vabs ((float*)a, 1, result, 1, N); 
}


#ifdef PLANK_IOS
// no vForce on iOS
static inline void pl_VectorReciprocalF_NN (float *result, const float* a, PlankUL N) 
{ 
    float one = 1.f;
    vDSP_vfill (&one, result, 1, N); 
    vDSP_vdiv (result, 1, (float*)a, 1, result, 1, N); 
}

PLANK_VECTORUNARYOP_DEFINE(Log2,F)
PLANK_VECTORUNARYOP_DEFINE(Sin,F)
PLANK_VECTORUNARYOP_DEFINE(Cos,F)
PLANK_VECTORUNARYOP_DEFINE(Tan,F)
PLANK_VECTORUNARYOP_DEFINE(Asin,F)
PLANK_VECTORUNARYOP_DEFINE(Acos,F)
PLANK_VECTORUNARYOP_DEFINE(Atan,F)
PLANK_VECTORUNARYOP_DEFINE(Sinh,F)
PLANK_VECTORUNARYOP_DEFINE(Cosh,F)
PLANK_VECTORUNARYOP_DEFINE(Tanh,F)
PLANK_VECTORUNARYOP_DEFINE(Sqrt,F)
PLANK_VECTORUNARYOP_DEFINE(Log,F)
PLANK_VECTORUNARYOP_DEFINE(Log10,F)
PLANK_VECTORUNARYOP_DEFINE(Exp,F)
PLANK_VECTORUNARYOP_DEFINE(Ceil,F)
PLANK_VECTORUNARYOP_DEFINE(Floor,F)

PLANK_VECTORBINARYOP_DEFINE(Pow,F)
PLANK_VECTORBINARYOP_DEFINE(Atan2,F)
#else
// use vForce on MacOS
static inline void pl_VectorReciprocalF_NN (float *result, const float* a, PlankUL N)   { const int n = N; vvrecf (result, a, &n); }
PLANK_VECTORUNARYOP_DEFINE(Log2,F)
static inline void pl_VectorSinF_NN (float *result, const float* a, PlankUL N)          { const int n = N; vvsinf (result, a, &n); }
static inline void pl_VectorCosF_NN (float *result, const float* a, PlankUL N)          { const int n = N; vvcosf (result, a, &n); }
static inline void pl_VectorTanF_NN (float *result, const float* a, PlankUL N)          { const int n = N; vvtanf (result, a, &n); }
static inline void pl_VectorAsinF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvasinf (result, a, &n); }
static inline void pl_VectorAcosF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvacosf (result, a, &n); }
static inline void pl_VectorAtanF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvatanf (result, a, &n); }
static inline void pl_VectorSinhF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvsinhf (result, a, &n); }
static inline void pl_VectorCoshF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvcoshf (result, a, &n); }
static inline void pl_VectorTanhF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvtanhf (result, a, &n); }
static inline void pl_VectorSqrtF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvsqrtf (result, a, &n); }
static inline void pl_VectorLogF_NN (float *result, const float* a, PlankUL N)          { const int n = N; vvlogf (result, a, &n); }
static inline void pl_VectorLog10F_NN (float *result, const float* a, PlankUL N)        { const int n = N; vvlog10f (result, a, &n); }
static inline void pl_VectorExpF_NN (float *result, const float* a, PlankUL N)          { const int n = N; vvexpf (result, a, &n); }
static inline void pl_VectorCeilF_NN (float *result, const float* a, PlankUL N)         { const int n = N; vvceilf (result, a, &n); }
static inline void pl_VectorFloorF_NN (float *result, const float* a, PlankUL N)        { const int n = N; vvfloorf (result, a, &n); }

static inline void pl_VectorPowF_NNN (float *result, const float* a, const float *b, PlankUL N)        
{ 
    const int n = N;
    vvpowf (result, a, b, &n); 
}

static inline void pl_VectorPowF_NN1 (float *result, const float* a, float b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfill (&b, result, 1, N); 
    vvpowf (result, a, result, &n); 
}

static inline void pl_VectorPowF_N1N (float *result, float a, const float* b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfill (&a, result, 1, N); 
    vvpowf (result, result, b, &n); 
}

static inline void pl_VectorAtan2F_NNN (float *result, const float* a, const float *b, PlankUL N)        
{ 
    const int n = N;
    vvatan2f (result, a, b, &n); 
}

static inline void pl_VectorAtan2F_NN1 (float *result, const float* a, float b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfill (&b, result, 1, N); 
    vvatan2f (result, a, result, &n); 
}

static inline void pl_VectorAtan2F_N1N (float *result, float a, const float* b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfill (&a, result, 1, N); 
    vvatan2f (result, result, b, &n); 
}
#endif

PLANK_VECTORBINARYOP_DEFINE(IsEqualTo,F)
PLANK_VECTORBINARYOP_DEFINE(IsNotEqualTo,F)
PLANK_VECTORBINARYOP_DEFINE(IsGreaterThan,F)
PLANK_VECTORBINARYOP_DEFINE(IsGreaterThanOrEqualTo,F)
PLANK_VECTORBINARYOP_DEFINE(IsLessThan,F)
PLANK_VECTORBINARYOP_DEFINE(IsLessThanOrEqualTo,F)
PLANK_VECTORBINARYOP_DEFINE(SumSqr,F)
PLANK_VECTORBINARYOP_DEFINE(DifSqr,F)
PLANK_VECTORBINARYOP_DEFINE(SqrSum,F)
PLANK_VECTORBINARYOP_DEFINE(SqrDif,F)
PLANK_VECTORBINARYOP_DEFINE(AbsDif,F)

PLANK_VECTORBINARYOPVECTOR_DEFINE(Thresh,F)

static inline void pl_VectorThreshF_NN1 (float *result, const float* a, float b, PlankUL N) 
{ 
    vDSP_vthres ((float*)a, 1, &b, result, 1, N);
}

PLANK_SCALARBINARYOPVECTOR_DEFINE(Thresh,F)

static inline void pl_VectorSquaredF_NN (float *result, const float* a, PlankUL N) 
{ 
    vDSP_vsq ((float*)a, 1, result, 1, N); 
}

static inline void pl_VectorCubedF_NN (float *result, const float* a, PlankUL N) 
{ 
    vDSP_vsq ((float*)a, 1, result, 1, N); 
    vDSP_vmul ((float*)a, 1, result, 1, result, 1, N);
}

PLANK_VECTORUNARYOP_DEFINE(Frac,F)
PLANK_VECTORUNARYOP_DEFINE(Sign,F)
PLANK_VECTORUNARYOP_DEFINE(M2F,F)
PLANK_VECTORUNARYOP_DEFINE(F2M,F)
PLANK_VECTORUNARYOP_DEFINE(A2dB,F)
PLANK_VECTORUNARYOP_DEFINE(dB2A,F)
PLANK_VECTORUNARYOP_DEFINE(D2R,F)
PLANK_VECTORUNARYOP_DEFINE(R2D,F)
PLANK_VECTORUNARYOP_DEFINE(Distort,F)
PLANK_VECTORUNARYOP_DEFINE(Zap,F)


static inline void pl_VectorAddF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vadd ((float*)a, 1, (float*)b, 1, result, 1, N); 
}

static inline void pl_VectorAddF_NN1 (float *result, const float* a, float b, PlankUL N)        
{ 
    vDSP_vsadd ((float*)a, 1, &b, result, 1, N); 
}

static inline void pl_VectorAddF_N1N (float *result, float a, const float* b, PlankUL N)        
{ 
    vDSP_vsadd ((float*)b, 1, &a, result, 1, N); 
}

static inline void pl_VectorSubF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    // due to bug in vDSP_vsub
    float mone = -1.f;
    vDSP_vsmul ((float*)b, 1, &mone, result, 1, N); 
    vDSP_vadd ((float*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorSubF_NN1 (float *result, const float* a, float b, PlankUL N)        
{ 
    float nb = -b; 
    vDSP_vsadd ((float*)a, 1, &nb, result, 1, N); 
}

static inline void pl_VectorSubF_N1N (float *result, float a, const float* b, PlankUL N)        
{ 
    float na = -a; 
    vDSP_vsadd ((float*)b, 1, &na, result, 1, N); 
}

static inline void pl_VectorMulF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vmul ((float*)a, 1, (float*)b, 1, result, 1, N); 
}

static inline void pl_VectorMulF_NN1 (float *result, const float* a, float b, PlankUL N)        
{ 
    vDSP_vsmul ((float*)a, 1, &b, result, 1, N); 
}

static inline void pl_VectorMulF_N1N (float *result, float a, const float* b, PlankUL N)        
{ 
    vDSP_vsmul ((float*)b, 1, &a, result, 1, N); 
}

static inline void pl_VectorDivF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vdiv ((float*)b, 1, (float*)a, 1, result, 1, N); // documented as a and b reversed
}

static inline void pl_VectorDivF_NN1 (float *result, const float* a, float b, PlankUL N)        
{ 
    vDSP_vsdiv ((float*)a, 1, &b, result, 1, N); 
}

static inline void pl_VectorDivF_N1N (float *result, float a, const float* b, PlankUL N)        
{ 
    float ra = 1.f / a;
    vDSP_vsmul ((float*)b, 1, &ra, result, 1, N); 
}

static inline void pl_VectorMinF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vmin ((float*)a, 1, (float*)b, 1, result, 1, N); 
}

static inline void pl_VectorMinF_NN1 (float *result, const float* a, float b, PlankUL N) 
{ 
    vDSP_vfill (&b, result, 1, N); 
    vDSP_vmin ((float*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMinF_N1N (float *result, float a, const float* b, PlankUL N) 
{ 
    vDSP_vfill (&a, result, 1, N); 
    vDSP_vmin ((float*)b, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMaxF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vmax ((float*)a, 1, (float*)b, 1, result, 1, N); 
}

static inline void pl_VectorMaxF_NN1 (float *result, const float* a, float b, PlankUL N) 
{ 
    vDSP_vfill (&b, result, 1, N); 
    vDSP_vmax ((float*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMaxF_N1N (float *result, float a, const float* b, PlankUL N) 
{ 
    vDSP_vfill (&a, result, 1, N); 
    vDSP_vmax ((float*)b, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorHypotF_NNN (float *result, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vdist ((float*)a, 1, (float*)b, 1, result, 1, N); 
}

static inline void pl_VectorHypotF_NN1 (float *result, const float* a, float b, PlankUL N) 
{ 
    vDSP_vfill (&b, result, 1, N); 
    vDSP_vdist ((float*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorHypotF_N1N (float *result, float a, const float* b, PlankUL N) 
{ 
    vDSP_vfill (&a, result, 1, N); 
    vDSP_vdist ((float*)b, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMulAddF_NNN (float *io, const float* a, const float* b, PlankUL N) 
{ 
    vDSP_vma ((float*)io, 1, (float*)a, 1, (float*)b, 1, (float*)io, 1, N); 
}

// works as documented but seems useless as it interpolates thr "wrong" two samples
//static inline void pl_VectorLookupF_NnN (float *result, const float* table, PlankUL n, const float* index, PlankUL N) 
//{ 
//    float mul = 1.f;
//    float add = 0.f;
//    vDSP_vtabi ((float*)index, 1, &mul, &add, (float*)table, n, result, 1, N);
//}
PLANK_VECTORLOOKUP_DEFINE(F)


//------------------------------- double ---------------------------------------


//PLANK_VECTOR_OPS(D)

static inline void pl_VectorFillD_N1 (double *result, double a, PlankUL N) 
{ 
    vDSP_vfillD (&a, result, 1, N); 
}

static inline void pl_VectorClearD_N (double *result, PlankUL N) 
{ 
    vDSP_vclrD (result, 1, N);
}

static inline void pl_VectorRampD_N1 (double *result, double a, double b, PlankUL N) 
{ 
    vDSP_vrampD (&a, &b, result, 1, N); 
}

static inline void pl_VectorLineD_N1 (double *result, double a, double b, PlankUL N) 
{ 
    vDSP_vgenD (&a, &b, result, 1, N); 
}

static inline void pl_VectorMoveD_NN (double *result, const double* a, PlankUL N) 
{ 
    vDSP_mmovD ((double*)a, result, N, 1, N, N); 
}

static inline void pl_VectorNegD_NN (double *result, const double* a, PlankUL N) 
{ 
    vDSP_vnegD ((double*)a, 1, result, 1, N); 
}

static inline void pl_VectorAbsD_NN (double *result, const double* a, PlankUL N) 
{ 
    vDSP_vabsD ((double*)a, 1, result, 1, N); 
}


#ifdef PLANK_IOS
// no vForce on iOS
static inline void pl_VectorReciprocalD_NN (double *result, const double* a, PlankUL N) 
{ 
    double one = 1.0;
    vDSP_vfillD (&one, result, 1, N); 
    vDSP_vdivD (result, 1, (double*)a, 1, result, 1, N); 
}

PLANK_VECTORUNARYOP_DEFINE(Log2,D)
PLANK_VECTORUNARYOP_DEFINE(Sin,D)
PLANK_VECTORUNARYOP_DEFINE(Cos,D)
PLANK_VECTORUNARYOP_DEFINE(Tan,D)
PLANK_VECTORUNARYOP_DEFINE(Asin,D)
PLANK_VECTORUNARYOP_DEFINE(Acos,D)
PLANK_VECTORUNARYOP_DEFINE(Atan,D)
PLANK_VECTORUNARYOP_DEFINE(Sinh,D)
PLANK_VECTORUNARYOP_DEFINE(Cosh,D)
PLANK_VECTORUNARYOP_DEFINE(Tanh,D)
PLANK_VECTORUNARYOP_DEFINE(Sqrt,D)
PLANK_VECTORUNARYOP_DEFINE(Log,D)
PLANK_VECTORUNARYOP_DEFINE(Log10,D)
PLANK_VECTORUNARYOP_DEFINE(Exp,D)
PLANK_VECTORUNARYOP_DEFINE(Ceil,D)
PLANK_VECTORUNARYOP_DEFINE(Floor,D)

PLANK_VECTORBINARYOP_DEFINE(Pow,D)
PLANK_VECTORBINARYOP_DEFINE(Atan2,D)
#else
// use vForce on MacOS
static inline void pl_VectorReciprocalD_NN (double *result, const double* a, PlankUL N)   { const int n = N; vvrec (result, a, &n); }
PLANK_VECTORUNARYOP_DEFINE(Log2,D)
static inline void pl_VectorSinD_NN (double *result, const double* a, PlankUL N)          { const int n = N; vvsin (result, a, &n); }
static inline void pl_VectorCosD_NN (double *result, const double* a, PlankUL N)          { const int n = N; vvcos (result, a, &n); }
static inline void pl_VectorTanD_NN (double *result, const double* a, PlankUL N)          { const int n = N; vvtan (result, a, &n); }
static inline void pl_VectorAsinD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvasin (result, a, &n); }
static inline void pl_VectorAcosD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvacos (result, a, &n); }
static inline void pl_VectorAtanD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvatan (result, a, &n); }
static inline void pl_VectorSinhD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvsinh (result, a, &n); }
static inline void pl_VectorCoshD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvcosh (result, a, &n); }
static inline void pl_VectorTanhD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvtanh (result, a, &n); }
static inline void pl_VectorSqrtD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvsqrt (result, a, &n); }
static inline void pl_VectorLogD_NN (double *result, const double* a, PlankUL N)          { const int n = N; vvlog (result, a, &n); }
static inline void pl_VectorLog10D_NN (double *result, const double* a, PlankUL N)        { const int n = N; vvlog10 (result, a, &n); }
static inline void pl_VectorExpD_NN (double *result, const double* a, PlankUL N)          { const int n = N; vvexp (result, a, &n); }
static inline void pl_VectorCeilD_NN (double *result, const double* a, PlankUL N)         { const int n = N; vvceil (result, a, &n); }
static inline void pl_VectorFloorD_NN (double *result, const double* a, PlankUL N)        { const int n = N; vvfloor (result, a, &n); }

static inline void pl_VectorPowD_NNN (double *result, const double* a, const double *b, PlankUL N)        
{ 
    const int n = N;
    vvpow (result, a, b, &n); 
}

static inline void pl_VectorPowD_NN1 (double *result, const double* a, double b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfillD (&b, result, 1, N); 
    vvpow (result, a, result, &n); 
}

static inline void pl_VectorAtan2D_NNN (double *result, const double* a, const double *b, PlankUL N)        
{ 
    const int n = N;
    vvatan2 (result, a, b, &n); 
}

static inline void pl_VectorAtan2D_NN1 (double *result, const double* a, double b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfillD (&b, result, 1, N); 
    vvatan2 (result, a, result, &n); 
}

static inline void pl_VectorAtan2D_N1N (double *result, double a, const double* b, PlankUL N)        
{ 
    const int n = N;
    vDSP_vfillD (&a, result, 1, N); 
    vvatan2 (result, result, b, &n); 
}
#endif

PLANK_VECTORBINARYOP_DEFINE(IsEqualTo,D)
PLANK_VECTORBINARYOP_DEFINE(IsNotEqualTo,D)
PLANK_VECTORBINARYOP_DEFINE(IsGreaterThan,D)
PLANK_VECTORBINARYOP_DEFINE(IsGreaterThanOrEqualTo,D)
PLANK_VECTORBINARYOP_DEFINE(IsLessThan,D)
PLANK_VECTORBINARYOP_DEFINE(IsLessThanOrEqualTo,D)
PLANK_VECTORBINARYOP_DEFINE(SumSqr,D)
PLANK_VECTORBINARYOP_DEFINE(DifSqr,D)
PLANK_VECTORBINARYOP_DEFINE(SqrSum,D)
PLANK_VECTORBINARYOP_DEFINE(SqrDif,D)
PLANK_VECTORBINARYOP_DEFINE(AbsDif,D)

PLANK_VECTORBINARYOPVECTOR_DEFINE(Thresh,D)

static inline void pl_VectorThreshD_NN1 (double *result, const double* a, double b, PlankUL N) 
{ 
    vDSP_vthresD ((double*)a, 1, &b, result, 1, N);
}

PLANK_SCALARBINARYOPVECTOR_DEFINE(Thresh,D)

static inline void pl_VectorSquaredD_NN (double *result, const double* a, PlankUL N) 
{ 
    vDSP_vsqD ((double*)a, 1, result, 1, N); 
}

static inline void pl_VectorCubedD_NN (double *result, const double* a, PlankUL N) 
{ 
    vDSP_vsqD ((double*)a, 1, result, 1, N); 
    vDSP_vmulD ((double*)a, 1, result, 1, result, 1, N);
}

PLANK_VECTORUNARYOP_DEFINE(Frac,D)
PLANK_VECTORUNARYOP_DEFINE(Sign,D)
PLANK_VECTORUNARYOP_DEFINE(M2F,D)
PLANK_VECTORUNARYOP_DEFINE(F2M,D)
PLANK_VECTORUNARYOP_DEFINE(A2dB,D)
PLANK_VECTORUNARYOP_DEFINE(dB2A,D)
PLANK_VECTORUNARYOP_DEFINE(D2R,D)
PLANK_VECTORUNARYOP_DEFINE(R2D,D)
PLANK_VECTORUNARYOP_DEFINE(Distort,D)
PLANK_VECTORUNARYOP_DEFINE(Zap,D)


static inline void pl_VectorAddD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vaddD ((double*)a, 1, (double*)b, 1, result, 1, N); 
}

static inline void pl_VectorAddD_NN1 (double *result, const double* a, double b, PlankUL N)        
{ 
    vDSP_vsaddD ((double*)a, 1, &b, result, 1, N); 
}

static inline void pl_VectorAddD_N1N (double *result, double a, const double* b, PlankUL N)        
{ 
    vDSP_vsaddD ((double*)b, 1, &a, result, 1, N); 
}

static inline void pl_VectorSubD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    // due to bug in vDSP_vsub
    double mone = -1.0;
    vDSP_vsmulD ((double*)b, 1, &mone, result, 1, N); 
    vDSP_vaddD ((double*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorSubD_NN1 (double *result, const double* a, double b, PlankUL N)        
{ 
    double nb = -b; 
    vDSP_vsaddD ((double*)a, 1, &nb, result, 1, N); 
}

static inline void pl_VectorSubD_N1N (double *result, double a, const double* b, PlankUL N)        
{ 
    double na = -a; 
    vDSP_vsaddD ((double*)b, 1, &na, result, 1, N); 
}

static inline void pl_VectorMulD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vmulD ((double*)a, 1, (double*)b, 1, result, 1, N); 
}

static inline void pl_VectorMulD_NN1 (double *result, const double* a, double b, PlankUL N)        
{ 
    vDSP_vsmulD ((double*)a, 1, &b, result, 1, N); 
}

static inline void pl_VectorMulD_N1N (double *result, double a, const double* b, PlankUL N)        
{ 
    vDSP_vsmulD ((double*)b, 1, &a, result, 1, N); 
}

static inline void pl_VectorDivD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vdivD ((double*)b, 1, (double*)a, 1, result, 1, N); // documented as a and b reversed
}

static inline void pl_VectorDivD_NN1 (double *result, const double* a, double b, PlankUL N)        
{ 
    vDSP_vsdivD ((double*)a, 1, &b, result, 1, N); 
}

static inline void pl_VectorDivD_N1N (double *result, double a, const double* b, PlankUL N)        
{ 
    double ra = 1.0 / a;
    vDSP_vsmulD ((double*)b, 1, &ra, result, 1, N); 
}

static inline void pl_VectorMinD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vminD ((double*)a, 1, (double*)b, 1, result, 1, N); 
}

static inline void pl_VectorMinD_NN1 (double *result, const double* a, double b, PlankUL N) 
{ 
    vDSP_vfillD (&b, result, 1, N); 
    vDSP_vminD ((double*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMinD_N1N (double *result, double a, const double* b, PlankUL N) 
{ 
    vDSP_vfillD (&a, result, 1, N); 
    vDSP_vminD ((double*)b, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMaxD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vmaxD ((double*)a, 1, (double*)b, 1, result, 1, N); 
}

static inline void pl_VectorMaxD_NN1 (double *result, const double* a, double b, PlankUL N) 
{ 
    vDSP_vfillD (&b, result, 1, N); 
    vDSP_vmaxD ((double*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMaxD_N1N (double *result, double a, const double* b, PlankUL N) 
{ 
    vDSP_vfillD (&a, result, 1, N); 
    vDSP_vmaxD ((double*)b, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorHypotD_NNN (double *result, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vdistD ((double*)a, 1, (double*)b, 1, result, 1, N); 
}

static inline void pl_VectorHypotD_NN1 (double *result, const double* a, double b, PlankUL N) 
{ 
    vDSP_vfillD (&b, result, 1, N); 
    vDSP_vdistD ((double*)a, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorHypotD_N1N (double *result, double a, const double* b, PlankUL N) 
{ 
    vDSP_vfillD (&a, result, 1, N); 
    vDSP_vdistD ((double*)b, 1, result, 1, result, 1, N); 
}

static inline void pl_VectorMulAddD_NNN (double *io, const double* a, const double* b, PlankUL N) 
{ 
    vDSP_vmaD ((double*)io, 1, (double*)a, 1, (double*)b, 1, (double*)io, 1, N); 
}

//static inline void pl_VectorLookupD_NnN (double *result, const double* table, PlankUL n, const double* index, PlankUL N) 
//{ 
//    double mul = 1.0;
//    double add = 0.0;
//    vDSP_vtabiD ((double*)index, 1, &mul, &add, (double*)table, n, result, 1, N);
//}
PLANK_VECTORLOOKUP_DEFINE(D)



#undef I
PLANK_VECTORCONVERTERS_DEFINE(F)
PLANK_VECTORCONVERTERS_DEFINE(D)
PLANK_VECTORCONVERTERS_DEFINE(C)
PLANK_VECTORCONVERTERS_DEFINE(I)
PLANK_VECTORCONVERTERS_DEFINE(S)
PLANK_VECTORCONVERTERS_DEFINE(L)


// vDSP_vspdp
// vDSP_vdpsp

#endif // !DOXYGEN
#endif // PLANK_VDSP_H
