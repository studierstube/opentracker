 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Header file for MathUtils class, a collection of mathematical functions.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/MathUtils.h,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _MATHUTILS_H
#define _MATHUTILS_H

#include "../dllinclude.h"

/**
 * This class implements some utility classes for matrix and quaternion
 * calculations. These are static methods, I just used a class to have
 * a common place for such things, like in Java. These do not handle any
 * memory issues, all arrays etc. have to be created and managed by the
 * calling code.
 * @todo think about using double in implementation to make it more accurate.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API MathUtils
{
public:
    /** converts an axis angle representation into a quaternion
     * @param axisa float[4] containing axis and angle in radiants
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* axisAngleToQuaternion(float* axisa, float* qResult);
    /** computes a quaternion from euler angles representing a rotation.
     * @param roll rotation around looking axis
     * @param pitch rotation around up axis
     * @param yaw rotation around side axis
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* eulerToQuaternion(float roll, float pitch, float yaw,
                                    float* qResult);
    /** inverts a quaternion.
     * @param q float[4] storing the quaternion
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* invertQuaternion(float* q, float* qResult);
    /** converts a rotational matrix to a quaternion.
     * @param matrix float[3][3] storing the rotational matrix
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* matrixToQuaternion(float matrix[3][3], float* qResult);
    /** multiplies two quaternions and stores result in a third.
     * @param q1 float[4] storing first quaternion
     * @param q2 float[4] storing second quaternion
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* multiplyQuaternion(float* q1, float* q2, float* qResult);
    /** normalizes quaternion to unit length. Here the computation is
     * done in place and the parameter is changed !
     * @param q float[4] storing quaternion
     * @return pointer to result array */
    static float* normalizeQuaternion(float* q);
    /** rotates a vector using a given unit quaternion. It does
     * not normalize the quaternion or check for unit length !
     * @param q float[4] storing quaternion
     * @param v float[3] storing vector
     * @param vResult float[3] where the result is stored
     * @return pointer to result array */
    static float* rotateVector(float* q, float* v, float* vResult);
};

#endif
