// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       NewKFilt
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Class implementation of the Kalmann filter
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:09:43  rr      last modification
// ===========================================================================

/*****************************************************************************/
/*									     */
/*	Copyright Notice :						     */
/*									     */
/*	Copyright, 1990-1997 by the University of Alberta		     */
/*	All rights reserved by the University of Alberta       		     */
/*									     */
/*	This software is copyrighted, and is licenced to academic and        */
/*	researcher institutions. As per the signed licence agreement,        */
/*      the licencee expressly agrees to the following:			     */
/*									     */
/*	1. This software can only be used for educational and research       */
/*	   purposes. This software cannot be used for commercial purposes    */
/*	   or be a key component of any contract involving the licensee.     */
/*	2. Title and copyright of this software remains with the University  */
/*	   of Alberta. The licensee is free to make copies of this software  */
/*	   for the licensee's own purposes as long as this entire University */
/*	   of Alberta copyright notice is included in each copy.	     */
/*	3. The licensee will not transfer this software in any way or form   */
/*	   to a third party.						     */
/*	4. The use of this software and the University of Alberta must be    */
/*	   acknowledged in any publication or art work, written, video taped */
/*	   or any other form, that the licensee produces using this software */
/*	   or any part of it.						     */
/*	5. The licensee will inform the University of Alberta Computing      */
/*	   Science Department of any publication or production using all or  */
/*	   part of this software and will furnish the University of Alberta  */
/*	   Computing Science Department with a copy of the publication.      */
/*	6. The University of Alberta assumes no responsibility for damages   */
/*	   resulting from the use of this software however caused.	     */
/*									     */
/*****************************************************************************/


/* $Id: NewKFilt.cxx,v 1.1 2000/09/19 14:45:13 reitmayr Exp $
 * Predictive filter in quaternion space
 * By Jiandong Liang	January 19, 1992
 * leung@cs.ualberta.ca
 */

#include "NewKFilt.h"

/*********************************************************************
*
*	Library for 2x2 matrices and 2x1 vector operations.
*
*********************************************************************/

const float EPSILON = 1.0e-10;

/*
 * Set a 2x2 matrix:    
 *	/ a11 a12 \
 *	\ a21 a22 /
 */

void	KalmanFilter::Mset(float a11, float a12, float a21,float a22, float *m)
{
	*m++ = a11; *m++ = a12; *m++ = a21; *m   = a22;
}

/*
 * Set an identity matrix: m = I
 */

void	KalmanFilter::Midn(float *m)
{
	*m++ = 1.0; *m++ = 0.0; *m++ = 0.0; *m   = 1.0;
}

/*
 * Matrix copy: m2 = m1
 */

void	KalmanFilter::Mcpy(float *m1, float *m2)
{
	*m2++ = *m1++;
	*m2++ = *m1++;
	*m2++ = *m1++;
	*m2   = *m1  ;
}

/*
 * Print m on stdout
 */

void	KalmanFilter::Mprt(float *m)
{
	printf("%f \t %f \n",*m,*(m+1));
	printf("%f \t %f \n",*(m+2),*(m+3));
}

/*
 * Matrix transpose: m2 = (m1)T
 */

void	KalmanFilter::Mtrn(float *m1, float *m2)
{
	*m2++ = *m1;
	*m2++ = *(m1+2);
	*m2++ = *(m1+1);
	*m2   = *(m1+3);
}

/*
 * Matrix inversion: m2 = m1^(-1)
 */

void	KalmanFilter::Minv(float *m1, float *m2)
{
    float delta;

	delta = *m1 * (*(m1+3)) - *(m1+1) * (*(m1+2));
	if (delta == 0) {
		fprintf(stderr,"Matrix is singular!\n");
		exit(2);
	}
	*m2 = *(m1+3)/delta;
	*(m2+1) = -(*(m1+1))/delta;
	*(m2+2) = -(*(m1+2))/delta;
	*(m2+3) = *m1/delta;
}

/*
 * Matrix addtion: m3 = m1 + m2
 */

void	KalmanFilter::Madd(float *m1, float *m2, float *m3)
{
	*m3++ = *m1++ + *m2++;
	*m3++ = *m1++ + *m2++;
	*m3++ = *m1++ + *m2++;
	*m3   = *m1   + *m2  ;
}

/*
 * Matrix substraction: m3 = m1 - m2
 */

void	KalmanFilter::Msub(float *m1, float *m2, float *m3)
{
	*m3++ = *m1++ - *m2++;
	*m3++ = *m1++ - *m2++;
	*m3++ = *m1++ - *m2++;
	*m3   = *m1   - *m2  ;
}

/*
 * Matrix multipication: m3 = m1 x m2
 */

void	KalmanFilter::Mmul(float *m1, float *m2, float *m3)
{
	*m3++ = *m1 * (*m2) + *(m1+1) * (*(m2+2));
	*m3++ = *m1 * (*(m2+1)) + *(m1+1) * (*(m2+3));
	*m3++ = *(m1+2) * (*m2) + *(m1+3) * (*(m2+2));
	*m3   = *(m1+2) * (*(m2+1)) + *(m1+3) * (*(m2+3));
}

/*
 * Vector set: v = [ b0 b1 ]T (transpose)
 */

void	KalmanFilter::Vset(float b0, float b1, float *v)
{
	*v++ = b0;
	*v   = b1;
}

/*
 * Vecotor copy: v2 = v1
 */

void	KalmanFilter::Vcpy(float *v1, float *v2)
{
	*v2++ = *v1++;
	*v2   = *v1;
}

/*
 * Vector addtion: v3 = v1 + v2
 */

void	KalmanFilter::Vadd(float *v1, float *v2, float *v3)
{
	*v3++ = *v1++ + *v2++;
	*v3   = *v1   + *v2  ;
}

/*
 * Vector substraction: v3 = v1 - v2
 */

void	KalmanFilter::Vsub(float *v1, float *v2, float *v3)
{
	*v3++ = *v1++ - *v2++;
	*v3   = *v1   - *v2  ;
}

/*
 * vector multiply vector to form matrix: M = v1 x v2
 */

void	KalmanFilter::VxV(float *v1, float *v2, float *m)
{
	*m++ = *v1 * (*v2);
	*m++ = *v1 *(*(v2+1));
	*m++ = *(v1+1) * (*v2);
	*m   = *(v1+1) * (*(v2+1));
}

/*
 * dot product of vector: *R = v1 o v2
 */

void	KalmanFilter::VdV(float *v1, float *v2, float *R)
{
	*R = *v1 * (*v2) + *(v1+1) * (*(v2+1));
}

/*
 * vector multiply matrix: v2 = v1 x M
 */

void	KalmanFilter::VxM(float *v1, float *m, float *v2)
{
	*v2++ = *v1 * (*m) + *(v1+1) * (*(m+2));
	*v2   = *v1 * (*(m+1)) + *(v1+1) * (*(m+3));
}

/*
 * scale a vector: v2 = s o v1
 */

void	KalmanFilter::Vsca(float *v1, float s, float *v2)
{
	*v2++ = *v1++ * s;
	*v2   = *v1   * s;
}

/*
 * Matrix multiply vector: w = M x V
 */

void	KalmanFilter::MxV(float m[4], float v[2], float w[2])
{
	w[0] = m[0] * v[0] + m[1] * v[1];
	w[1] = m[2] * v[0] + m[3] * v[1];
}

/*
 * Set a identity quaternion
 */

void KalmanFilter::Qidn(float q[4])
{
	q[0] = 1.0;
	q[1] = q[2] = q[3] = 0.0;
}

/*
 * copy quaternion
 */

void	KalmanFilter::Qcp(float q1[4],float q2[4])
{
int	k;
	for (k=0; k<4; k++) q2[k] = q1[k];
}

/*
 * Great arc distance between quaternions
 */

float	KalmanFilter::Qarc(float q[4],float r[4])
{
float	len = q[0]*r[0]+q[1]*r[1]+q[2]*r[2]+q[3]*r[3];
	if (len > 1.0)	return(0.0);
	return ((float)acos((double)len));
}

/*
 * Spherical Linear Combination of quaternions
 */

void	KalmanFilter::Slic(float q1[4], float q2[4], float t, float Q[4])
{
float	a, b, c, d;
int	i;

	a = Qarc(q1, q2);
	if (a < EPSILON) {
		Qcp(q2, Q);
	} else {
		b = t * a;
		c = cos(b)+sin(b)/tan(a);
		d = sin(b)/sin(a);
		for (i=0; i<4; i++)
			Q[i] = c * q2[i] - d * q1[i];
	}
}

/*
 * Set a quaternion 
 */

void	KalmanFilter::Qset(float q[4], float a, float b, float c, float d)
{
	q[0] = a;
	q[1] = b;
	q[2] = c;
	q[3] = d;
}

/*
 * Normalize a quaternion
 */

void	KalmanFilter::Qnormalize(float q[4])
{
float	len = (float)sqrt((double)(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]));
        if (fabs(len) < 0.01) {
	    q[0] = 1.0;
	    q[1] = q[2] = q[3] = 0.0;
	}
        else {
	    q[0] /= len;
	    q[1] /= len;
	    q[2] /= len;
	    q[3] /= len;
	}
}


/***********************************************************************/

float	KalmanFilter::Kalman(int k, float s[2], float m)
//int	k;	/* KF index */
//float	s[2];	/* state vector */
//float	m;	/* observed measurement: 1x1 */
{
float	est[2];		/* estimation */
float	vect[2];	/* temperary vector */
float	pred[2];	/* prediction */

	Vsca(K[k],(m - s[0]),vect);
	Vadd(s,vect,est);
	MxV(F[k],est,s);		/* one step */
	MxV(F_[k],est,pred);	/* multiple steps */
	return (pred[0]);
}

void KalmanFilter::New_K_filt(float inq[4], float ouq[4])
{
        int i;

	Qcp(inq, new_inq);
#ifdef K_FILT_DEBUG
        if (index_big < BIG_SIZE)
	    Qcp(inq, INQ[index_big]);
#endif /* K_FILT_DEBUG */
	Qnormalize(new_inq);

	arc = Qarc(new_inq, old_inq);

	if (arc > M_PI/2.0) { /* flipped input */
		new_inq[0] *= -1.0;
		new_inq[1] *= -1.0;
		new_inq[2] *= -1.0;
		new_inq[3] *= -1.0;
		arc = Qarc(new_inq, old_inq);
	}
#ifdef K_FILT_DEBUG
        if (index_big < BIG_SIZE)
	    Qcp(new_inq, NEWINQ[index_big]);
#endif /* K_FILT_DEBUG */

	if (state == SMOOTH) {
		if (scnt) {
			scnt--;
			alpha = ALPHA-1.0;
			Qcp(old_ouq, q__);
		} else if (arc > MAXNOISE) {
			state = PREDICT;
			Qcp(old_inq, R0);
			sum = arc;
			s[SLOW][0] = s[SLOW][1] = 0.0;
			new_kp[SLOW] = Kalman(SLOW, s[SLOW], -arc);
			new_kp[SLOW] = Kalman(SLOW, s[SLOW], 0.0);
			new_kp[SLOW] = Kalman(SLOW, s[SLOW], arc);
			s[FAST][0] = s[FAST][1] = 0.0;
			new_kp[FAST] = Kalman(FAST, s[FAST], -arc);
			new_kp[FAST] = Kalman(FAST, s[FAST], 0.0);
			new_kp[FAST] = Kalman(FAST, s[FAST], arc);
			mcnt = MLENGTH;
			alpha = new_kp[FAST]/arc-1.0;
			Qcp(R0, q__);
		} else {
			alpha = (arc/MAXNOISE)-1.0;
			Qcp(old_ouq, q__);
		}
	} else if (state == PREDICT) {
		sum = Qarc(R0, new_inq);
		if (mcnt) mcnt--;
		new_kp[SLOW] = Kalman(SLOW, s[SLOW], sum);
		new_kp[FAST] = Kalman(FAST, s[FAST], sum);
		mix = (mcnt*0.4/MLENGTH)+0.6;
		comb = mix*new_kp[FAST]+(1.0-mix)*new_kp[SLOW];
		if (arc > 0.0)
			alpha = comb/sum -1.0;
		else
			alpha = 0.0;
		Qcp(R0, q__);
		if (arc < MAXNOISE/2.0) {
			state = SMOOTH;
			scnt = SLENGTH;
		}
	}

	Slic(q__, new_inq, alpha, new_ouq);

	Qcp(new_ouq, ouq);

#ifdef K_FILT_DEBUG
        if (index_big < BIG_SIZE)
	    Qcp(ouq, OUQ[index_big]);

        if (index_big < BIG_SIZE)
	    index_big++;


        if (index_big == BIG_SIZE) {
	    for (i=0; i<BIG_SIZE; i++) {
#if 0
		fprintf(dbgfile, "I %f %f %f %f\n", INQ[i][0], INQ[i][1], INQ[i][2], INQ[i][3]);
		fprintf(dbgfile, "%f %f %f %f\t", NEWINQ[i][0], NEWINQ[i][1], NEWINQ[i][2], NEWINQ[i][3]);
		fprintf(dbgfile, "%f %f %f %f\n", OUQ[i][0], OUQ[i][1], OUQ[i][2], OUQ[i][3]);
#endif
	    }

	    index_big++;
	}


#if 0
fprintf(dbgfile, "O %f %f %f %f\n", ouq[0], ouq[1], ouq[2], ouq[3]);
#endif
#endif /* K_FILT_DEBUG */

#if 0
	fprintf(dbgfile, " %f ", arc);
	fprintf(dbgfile, " %f ", sum);
	fprintf(dbgfile, " %f ", new_kp[SLOW]);
	fprintf(dbgfile, " %f ", new_kp[FAST]);
	fprintf(dbgfile, " %f ", comb);
	fprintf(dbgfile, " %f ", (float)state);
	fprintf(dbgfile, "\n");
#endif

	old_kp[SLOW] = new_kp[SLOW];
	old_kp[FAST] = new_kp[FAST];
	Qcp(new_ouq,old_ouq);
	Qcp(new_inq,old_inq);
}


KalmanFilter::KalmanFilter()
{
int	j, plen;

    state = SMOOTH;
//    H[2] = {1.0, 0.0};
    H[0] = 1.0;
    H[1] = 0.0;
    index_big = 0;

	plen = 3;
	beta[SLOW] = 5.0;	sig2[SLOW] = 0.2;	err[SLOW] = 0.1;
	beta[FAST] = 0.1;	sig2[FAST] = 0.4;	err[FAST] = 0.05;

	for (j=SLOW; j<=FAST; j++) {
		F[j][0] = 1.0;
		F[j][1] = (1.0-exp(-beta[j]))/beta[j];
		F[j][2] = 0.0;
		F[j][3] = exp(-beta[j]);
		F_[j][0] = 1.0;
		F_[j][1] = (1.0-exp((float)plen*(-beta[j])))/beta[j];
		F_[j][2] = 0.0;
		F_[j][3] = exp((float)plen*(-beta[j]));
		gain(beta[j],sig2[j],err[j],F[j],K[j]);
	}
}

/* Compute stable state Kalman gain */

void KalmanFilter::gain(float beta, float sig2, float err, float F[4], float K[2])
{
float	d, e_1, e_2, S[2], P[4], Q[4];
int	n = 0;
float	T[4];			/* updated error covariance matrix */
float	Ft[4];			/* transposed transition matrix */
float	V1[2], V2[2];		/* temperary vectors */
float	r1, r2;			/* temperary veriables */
float	M1[4], M2[4], M3[4];	/* temperary matrices */


	Midn(P);
	e_1 = (1.0 - exp(-beta))/beta;
	e_2 = (1.0 - exp(2.0*(-beta)))/beta/2.0;
	Q[0] = 2.0*sig2*(1.0 - 2.0*e_1 + e_2)/beta;
	Q[1] = 2.0*sig2*(e_1 - e_2);
	Q[2] = 2.0*sig2*(e_1 - e_2);
	Q[3] = 2.0*sig2*e_2*beta;
	Vset(0.0, 0.0, S);
	Vset(1.0, 1.0, K);
	while (1) {
		if (++n % 100 == 0) fprintf(stderr,"n= %d\n", n);
		d=fabs(K[0]-S[0])+fabs(K[1]-S[1]);
		if (d < KEPSILON) break;
		Vcpy(K, S);
		VxM(H,P,V1);
		VdV(V1,H,&r1);
		r2 = 1.0/(r1 + err);
		MxV(P,H,V2);
		Vsca(V2,r2,K);
		Midn(M1);
		VxV(K,H,M2);
		Msub(M1,M2,M3);
		Mmul(M3,P,T);
		Mmul(F,T,M1);
		Mtrn(F,Ft);
		Mmul(M1,Ft,M2);
		Madd(M2,Q,P);
	}
}


