 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** Header file for MathUtils class, a collection of mathematical functions.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/MathUtils.h,v 1.16 2003/07/02 07:28:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _MATHUTILS_H
#define _MATHUTILS_H

#include "../dllinclude.h"

/* for accessing the elements of Quaternion and Vector3 */
#define Q_X    0
#define Q_Y    1
#define Q_Z    2
#define Q_W    3

/**
 * This class implements some utility classes for matrix and quaternion
 * calculations. These are static methods, I just used a class to have
 * a common place for such things, like in Java. These do not handle any
 * memory issues, all arrays etc. have to be created and managed by the
 * calling code.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API MathUtils
{
public:
    /// 3x3 double matrix type
    typedef double Matrix3x3[3][3];
    /// 4x4 double matrix type
    typedef double Matrix4x4[4][4];
    /// 3 double vector
    typedef double Vector3[3];
    /// 4 double vector used as quaternion
    typedef double Quaternion[4];

    /** converts an axis angle representation into a quaternion. This method
     * operates directly on the arguments. Therefore using the same pointers
     * for several arguments will produce wrong results !
     * @param axisa float[4] containing axis and angle in radiants
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* axisAngleToQuaternion(const float* axisa, float* qResult);
    /** computes a quaternion from euler angles representing a rotation.
     * @param roll rotation around looking axis
     * @param pitch rotation around up axis
     * @param yaw rotation around side axis
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* eulerToQuaternion(const float roll, const float pitch, const float yaw,
                                    float* qResult);
    /** inverts a quaternion. This method
     * operates directly on the arguments. Therefore using the same pointers
     * for several arguments will produce wrong results !
     * @param q float[4] storing the quaternion
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* invertQuaternion(const float* q, float* qResult);
    /** converts a rotational matrix to a quaternion.
     * @param matrix float[3][3] storing the rotational matrix
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* matrixToQuaternion(const float matrix[3][3], float* qResult);
    /** multiplies two quaternions and stores result in a third. This method
     * operates directly on the arguments. Therefore using the same pointers
     * for several arguments will produce wrong results !
     * @param q1 float[4] storing first quaternion
     * @param q2 float[4] storing second quaternion
     * @param qResult float[4] where the result is stored
     * @return pointer to result array */
    static float* multiplyQuaternion(const float* q1, const float* q2, float* qResult);
    /** normalizes quaternion to unit length. Here the computation is
     * done in place and the parameter is changed !
     * @param q float[4] storing quaternion
     * @return pointer to result array */
    static float* normalizeQuaternion(float* q);
    /** rotates a vector using a given unit quaternion. It does
     * not normalize the quaternion or check for unit length ! This method
     * operates directly on the arguments. Therefore using the same pointers
     * for several arguments will produce wrong results !
     * @param q float[4] storing quaternion
     * @param v float[3] storing vector
     * @param vResult float[3] where the result is stored
     * @return pointer to result array */
    static float* rotateVector(const float* q, const float* v, float* vResult);
	/** computes the determinant of a 3x3 matrix.
	 * @param matrix the 3x3 matrix to use
	 * @return determinant of the matrix */
	static float determinant( const float matrix[3][3] );

    /** computes the vector and angle representation of a quaternion. This method
     * operates directly on the arguments. Therefore using the same pointers
     * for several arguments will produce wrong results !
	 * @param q float[4] storing the quaternion
	 * @param axisa float[4] storing the vector and angle of the given quaternion
	 * @return pointer to result array (axisa) */
	static float* quaternionToAxisAngle(const float *q, float *axisa);
    
    /**
     * computes the angle between two n-dimensional vectors.
     * @param v1 float[n] first vector
     * @param v2 float[n] second vector
     * @param dim dimension n of the two vectors
     * @return angle between the two vectors
     */    
    static double angle( const float * v1, const float * v2, const int dim );

    /**
     * computes the dot product between two n-dimensional vectors.
     * @param v1 float[n] first vector
     * @param v2 float[n] second vector
     * @param dim dimension n of the two vectors
     * @return dot product between the two vectors
     */ 
    static double dot( const float * v1, const float * v2, const int dim );
    
    /**
     * computes the spherical linear interpolation between two quaternions. The
     * formulas used are probably only stable for t in [0,1], but it can be 
     * used to calculate extrapolations as well. Moreover the slerp is always
     * computed on the shorter path between the quaternions. This method
     * operates directly on the arguments. Therefore using the same pointers
     * for several arguments will produce wrong results !
     * @param q1 float[4] first quaternion 
     * @param q2 float[4] second quaternion
     * @param t interpolation parameter
     * @param qResult float[4] stores result
     * @return pointer to result = qResult
     */
    static float * slerp( const float * q1, const float *q2, const float t, float * qResult );

	static void matrixMultiply(const Matrix4x4 m1, const Matrix4x4 m2, Matrix4x4 &m);
	static void matrixMultiply(const Matrix3x3 m1, const Matrix3x3 m2, Matrix3x3 &m);
	
    /// mirror at the x-z-plane
    static const Matrix4x4 matrix4x4_flipY;
    /// identity matrix
	static const Matrix4x4 matrix4x4_identity;

    /*****************************************************************************
     * MatrixToEuler - convert a column matrix to euler angles    
     *
     *  input:
     *	- vector to hold euler angles
	 *  - src column matrix
     *  Vector3  	angles    :      Holds outgoing roll, pitch, yaw
     *  Matrix4x4 	colMatrix :      Holds incoming rotation 
     *
     *  output:
     *  - euler angles in radians in the range -pi to pi;
	 *  vec[0] = yaw, vec[1] = pitch, vec[2] = roll
	 *  yaw is rotation about Z axis, pitch is about Y, roll -> X rot.
     *
     * notes:
     *	- originally written by Gary Bishop
     *
     *****************************************************************************/
	static void MatrixToEuler(Vector3 angles, const Matrix4x4 colMatrix);

    /// the nice constant Pi
    static const double Pi;

    /// the also nice constant e
    static const double E;

	/// another nice constant to transform grad to radiants
	static const double GradToRad;
};

#endif
