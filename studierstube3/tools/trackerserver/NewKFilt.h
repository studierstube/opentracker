// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       NewKFilt
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of the Kalmann filter
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:09:43  rr      last modification
// ===========================================================================


#ifndef _NEW_K_FILT_H_
#define _NEW_K_FILT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define K_FILT_DEBUG		// ONLY for debugging... 

const float MAXNOISE = 0.02;
const float KEPSILON = 1.0e-5;
const int SLENGTH = 3;

const int MLENGTH = 15;
const float ALPHA = 0.2;

const int SLOW = 0;
const int FAST = 1;

const int SMOOTH = 0;
const int PREDICT = 1;

class KalmanFilter
{
    public:
        KalmanFilter();
        void New_K_filt(float* inq, float* ouq);

    private:
        static void Mset(float a11, float a12, float a21,float a22, float *m);
        static void Midn(float *m);
        static void Mcpy(float *m1, float *m2);
        static void Mprt(float *m);
        static void Mtrn(float *m1, float *m2);
        static void Minv(float *m1, float *m2);
        static void Madd(float *m1, float *m2, float *m3);
        static void Msub(float *m1, float *m2, float *m3);
        static void Mmul(float *m1, float *m2, float *m3);
        static void Vset(float b0, float b1, float *v);
        static void Vcpy(float *v1, float *v2);
        static void Vadd(float *v1, float *v2, float *v3);
        static void Vsub(float *v1, float *v2, float *v3);
        static void VxV(float *v1, float *v2, float *m);
        static void VdV(float *v1, float *v2, float *R);
        static void VxM(float *v1, float *m, float *v2);
        static void Vsca(float *v1, float s, float *v2);
        static void MxV(float m[4], float v[2], float w[2]);
        static void Qidn(float q[4]);
        static void Qcp(float q1[4],float q2[4]);
        static float Qarc(float q[4],float r[4]);
        static void Slic(float q1[4], float q2[4], float t, float Q[4]);
        static void Qset(float q[4], float a, float b, float c, float d);
        static void Qnormalize(float q[4]);
        void gain(float beta, float sig2, float err, float F[4], float K[2]);
        float Kalman(int k, float s[2], float m);

        float H[2];

        int state;
        int scnt;
        int mcnt;
        float alpha, sum, arc, comb, mix;
        float q__[4];

        float beta[2];  // time constant
        float sig2[2];  // variance factor
        float err[2];   // observation error
        float K[2][2];  // Kalman gain
        float F[2][4];  // 1-step transition matrices
        float F_[2][4]; // n-step transition matrices
        float s[2][2];  // state vector
        float new_kp[2];// Kalman prediction
        float old_kp[2];// last prediction

        float new_inq[4], old_inq[4], new_ouq[4], old_ouq[4];
        float R0[4];

        int index_big;

#ifdef K_FILT_DEBUG
#define  BIG_SIZE  400
        float INQ[BIG_SIZE][4];
        float OUQ[BIG_SIZE][4];
        float NEWINQ[BIG_SIZE][4];
#endif

};

#endif
