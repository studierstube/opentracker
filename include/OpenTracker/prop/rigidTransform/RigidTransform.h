
/** Header file for RigidTransform class.
 *
 * @author Stefan Mooslechner
 */

/**
 * @page module_ref Module Reference
 *
 * This class calculates an 3x3 rotation matrix \f$ R \f$ and an 3x1 translation vector \f$ T \f$ to
 * describe the transformation between two given corresponding pointsets \f$ d \f$ and \f$ m \f$ .\n 
 * The point sets may have a noise but they must not be scaled or sheared.
 *
 * The two corresponded point sets \f$ \{ m_i \} \f$ and \f$ \{ d_i \} , i = 1 .. N \f$
 * are related by: \n \f$ d_i = R \, m_i + T + V_i \f$ \n where \n
 * \f$ R \f$ is a 3x3 rotation matix, \n
 * \f$ T \f$ is a 3x1 translation vector and \n 
 * \f$ V_i \f$ is a noise vector. \n
 *
 * <p>
 * The algorithm based on the first algorithm from <em><b>Eggert, D.W., Lorusso, A., Fisher, R.B.</b>: 
 *          "Estimating 3-D rigid body transformations: a comparison of four major algorithms", 
 *          Machine Vision and Applications, Volume 9, Numbers 5-6 / March 1997; 
 *          Springer-Verlag Berlin / Heidelberg (1997) </em> </p>
 */


#ifndef RIGID_TRANSFORM_H___
#define RIGID_TRANSFORM_H___

#include "../tnt/tnt_cmat.h"
#include "../tnt/tnt_array2d.h"
#include "../tnt/jama_svd.h"

/** This class calculates an 3x3 rotation matrix R and an 3x1 translation vector T to
 *  describe the transformation between two given corresponding pointsets d and m.
 *  \see <A HREF="module_ref.html"> Module Reference </A> for further information. */
class RigidTransform{

private:
   /** Input matrix d (3xN or 4xN) */
   TNT::Matrix<double> d;

   /** Input matrix m (3xN or 4xN) */
   TNT::Matrix<double> m;

   /** Result matrix R (3x3)*/
   TNT::Matrix<double> R;

   /** Result matix T (3x1)*/
   TNT::Matrix<double> T;

   /** Number of columns i.e. size of the pointset */
   int num_cols;

   /** Copyconstructor method */
   RigidTransform(const RigidTransform &src){}
   
   /** Main method. Calculates the 3x3 rotation matrix R and the
    *  3x1 translation matrix T and stores them in the members R and T. */
   void calculateRigidTransform();

   /** Calculates the determinant of a 3x3 matrix.
    *  @param M reference to an 3x3 matrix 
    *  @return determinant of the given matrix */
   double getDeterminant(const TNT::Matrix<double> &M);
   
   /** Takes a 3xN or a 4xN Matrix and returns the Nth column.
    *  @note It returns allways a 3x1 matrix independent of number of rows
    *    of the given matrix.
    *  @param M const reference to the matrix (3xN or 4xN)
    *  @param col number of the column to return (range: 1..N)
    *  @return 3x1 matrix with the values of the Nth column of M */
   TNT::Matrix<double> getColumn(const TNT::Matrix<double> &M, int col);

public:

   /** Constructor method. 
    *  Takes as argument two 3xN or 4xN matrices to calculate a 3x3 rotation matrix
    *  and a 3x1 translation matrix.
    *  @param d_ const reference to the matrix were the first pointset is stored
    *  @param m_ const reference to the matrix were the second pointset is stored 
    *  @note The matrices d_ and m_ must have the same dimension.  */
   RigidTransform(const TNT::Matrix<double> &d_, const TNT::Matrix<double> &m_);
   
   /** Destructor method */
   virtual ~RigidTransform(){}

   /** @return the calculated 3x3 rotationmatrix R */
   TNT::Matrix<double> getR();

   /** @return the calculated 3x1 translationmatrix T */
   TNT::Matrix<double> getT();
};

#endif //RIGID_TRANSFORM_H___