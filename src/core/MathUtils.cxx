 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for MathUtils class, a collection of mathematical functions.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/MathUtils.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "MathUtils.h"
#include <math.h>

// converts an axis angle representation into a quaternion

float* MathUtils::axisAngleToQuaternion(float* axisa, float* qResult)
{
    float s = sin( axisa[3]/2 );
    qResult[3] = cos( axisa[3]/2 );
    qResult[0] = axisa[0]*s;
    qResult[1] = axisa[1]*s;
    qResult[2] = axisa[2]*s;
    return qResult;
}

// computes a quaternion from euler angles representing a rotation.

float* MathUtils::eulerToQuaternion(float roll, float pitch, float yaw,
                                    float* qResult)
{
    float cr, cp, cy, sr, sp, sy, cpcy, spsy;

    // calculate trig identities
    cr = cos(roll/2);
    cp = cos(pitch/2);
    cy = cos(yaw/2);

    sr = sin(roll/2);
    sp = sin(pitch/2);
    sy = sin(yaw/2);

    cpcy = cp * cy;
    spsy = sp * sy;

    qResult[0] = sr * cpcy - cr * spsy;
    qResult[1] = cr * sp * cy + sr * cp * sy;
    qResult[2] = cr * cp * sy - sr * sp * cy;
    qResult[3] = cr * cpcy + sr * spsy;
    return qResult;
}

// Inverts a Quaternion. Returns result in the second parameter.

float* MathUtils::invertQuaternion(float* q, float* qResult)
{
    float mod = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
    qResult[0] = -q[0] / mod;
    qResult[1] = -q[1] / mod;
    qResult[2] = -q[2] / mod;
    qResult[3] = q[3] / mod;
    return qResult;
}

// computes a quaternion from a passed rotation matrix.

float* MathUtils::matrixToQuaternion(float matrix[3][3], float* qResult)
{
    float tr, s;
    int i, j, k;
    int nxt[3] = {1, 2, 0};

    tr = matrix[0][0] + matrix[1][1] + matrix[2][2];

    // check the diagonal
    if (tr > 0.0)
    {
        s = sqrt (tr + 1.0);
        qResult[3] = s / 2.0;
        s = 0.5 / s;
        qResult[0] = (matrix[2][1] - matrix[1][2]) * s;
        qResult[1] = (matrix[0][2] - matrix[2][0]) * s;
        qResult[2] = (matrix[1][0] - matrix[0][1]) * s;
    }
    else
    {
        // diagonal is negative
        i = 0;
        if (matrix[1][1] > matrix[0][0]) i = 1;
        if (matrix[2][2] > matrix[i][i]) i = 2;
        j = nxt[i];
        k = nxt[j];

        s = sqrt((matrix[i][i] - (matrix[j][j] + matrix[k][k])) + 1.0);

        qResult[i] = s * 0.5;

        if (s != 0.0) s = 0.5 / s;

        qResult[3] = (matrix[k][j] - matrix[j][k]) * s;
        qResult[j] = (matrix[j][i] + matrix[i][j]) * s;
        qResult[k] = (matrix[k][i] + matrix[i][k]) * s;
    }
    return qResult;
}

// multiplies two quaternions and returns result in a third.

float* MathUtils::multiplyQuaternion(float* q1, float* q2, float* qResult)
{
    qResult[0] = q1[3]*q2[0]+q1[0]*q2[3]+q1[1]*q2[2]-q1[2]*q2[1];
    qResult[1] = q1[3]*q2[1]-q1[0]*q2[2]+q1[1]*q2[3]+q1[2]*q2[0];
    qResult[2] = q1[3]*q2[2]+q1[0]*q2[1]-q1[1]*q2[0]+q1[2]*q2[3];
    qResult[3] = q1[3]*q2[3]-q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2];
    return qResult;
}

// normalizes a quaternion to a unit quaternion.

float* MathUtils::normalizeQuaternion(float* q)
{
    float mod = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
    q[0] /= mod;
    q[1] /= mod;
    q[2] /= mod;
    q[3] /= mod;
    return q;
}

// rotates a vector by a given quaternion.

float* MathUtils::rotateVector(float* q, float* v, float* vResult)
{
    float t1[4], t2[4], t3[4];
    // vector in t2
    t2[0] = v[0];
    t2[1] = v[1];
    t2[2] = v[2];
    t2[3] = 0;
    // inverse in t1
    invertQuaternion( q, t1 );
    // v * t1 -> t3
    multiplyQuaternion( t2, t1, t3 );
    // q * t3 -> t1
    multiplyQuaternion( q, t3, t1 );
    // t1 result -> vResult
    vResult[0] = t1[0];
    vResult[1] = t1[1];
    vResult[2] = t1[2];
    return vResult;
}
