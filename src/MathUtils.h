// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      MathUtils.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'MathUtils'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Update comment header
// ===========================================================================
#ifndef _MATHUTILS_H
#define _MATHUTILS_H


//@START_USER1
//@END_USER1


/*@NOTE_4568
implements some utility classes for matrix and quaternion calculations.
*/
class MathUtils
{

//@START_USER2
//@END_USER2

// Members
private:

protected:

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    virtual ~MathUtils();
    static float* axisAngleToQuaternion(float* axisa, float* qResult);
    static float* eulerToQuaternion(float roll, float pitch, float yaw,
                                    float* qResult);
    static float* invertQuaternion(float* q, float* qResult);
    static float* matrixToQuaternion(float matrix[3][3], float* qResult);
    static float* multiplyQuaternion(float* q1, float* q2, float* qResult);
    static float* normalizeQuaternion(float* q);
    static float* rotateVector(float* q, float* v, float* vResult);
};

#endif


#ifdef CB_INLINES
#ifndef _MATHUTILS_H_INLINES
#define _MATHUTILS_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
