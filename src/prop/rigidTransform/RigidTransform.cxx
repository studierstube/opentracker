
/** Source file for RigidTransform class.
 *
 * @author Stefan Mooslechner
 */

#include <OpenTracker/prop/rigidTransform/RigidTransform.h>



#include <assert.h>


//---------------------------------------------------------------
RigidTransform::RigidTransform(const TNT::Matrix<double> &d_, const TNT::Matrix<double> &m_)
{
   assert (d_ != 0 || m_ != 0);
   assert (d_.num_cols() == m_.num_cols());
   assert (d_.num_rows() == m_.num_rows());
   assert (d_.num_rows() != 3 || d_.num_rows() != 4);
   num_cols = d_.num_cols();
   d = d_;
   m = m_;
   R = TNT::Matrix<double>(3,3);
   T = TNT::Matrix<double>(3,1);
   calculateRigidTransform();
}

//---------------------------------------------------------------
void RigidTransform::calculateRigidTransform()
{
   TNT::Matrix<double> d_average(3,1);
   TNT::Matrix<double> m_average(3,1);

   // calculate the average vector of the pointsets
   for (int i=1; i<=num_cols; ++i)
   {
      d_average = d_average + getColumn(d, i);
      m_average = m_average + getColumn(m, i);
   }
   for(int i=1; i<=3; ++i)
   {
      d_average(i,1) *= 1.0/num_cols;
      m_average(i,1) *= 1.0/num_cols;
   } 
   
   // calculate matrices of the pointsets moved to the center
   TNT::Matrix<double> d_centered(3,num_cols);
   TNT::Matrix<double> m_centered(3,num_cols);

   for (int i=1; i<=num_cols; ++i)
   {
      for(int j=1; j<=3; ++j)
      {
         d_centered(j,i) = d(j,i) - d_average(j,1);
         m_centered(j,i) = m(j,i) - m_average(j,1);
      }
   }

   // calculate correlation matrix H
   TNT::Matrix<double> H(3,3);

   for (int i=1; i<=num_cols; ++i)
      H = H + ( getColumn(m_centered,i) * transpose( getColumn(d_centered,i) ) );

   // move H into an Array2d (SVD just takes Array2d) 
   TNT::Array2D<double> H_array(3,3);
   for (int i=0; i<3; ++i)
      for (int j=0; j<3; ++j)
         H_array[j][i] = H[j][i];

   // executing SVD
   JAMA::SVD<double> svd(H_array);
   TNT::Array2D<double> U_array,V_array;
   svd.getU(U_array);
   svd.getV(V_array);

   // move U,V back into Matrices
   TNT::Matrix<double> U(3,3);
   TNT::Matrix<double> V(3,3);

   for (int i=0; i<3; ++i)
   {
      for (int j=0; j<3; ++j)
      {
         U[j][i] = U_array[j][i]*-1;
         V[j][i] = V_array[j][i]*-1;
      }
   }

   //calculate R and T
   R = V * transpose(U);
   T = d_average - (R * m_average);

   // check special case (2 planar pointsets)
   if (getDeterminant(R) == -1)
   {
      TNT::Matrix<double> Trans(3,3);
      Trans(1,1) = 1.0;
      Trans(2,2) = 1.0;
      Trans(3,3) = getDeterminant( U * transpose(T) );
      R = U * Trans * transpose(V);
   }
}

//---------------------------------------------------------------
TNT::Matrix<double> RigidTransform::getColumn(const TNT::Matrix<double> &M, int col)
{
   TNT::Matrix<double> tmp(3,1);
   for (int i=1; i<=3; ++i)
      tmp(i,1) = M(i,col);
   return tmp;
}

//---------------------------------------------------------------
double RigidTransform::getDeterminant(const TNT::Matrix<double> &M)
{
   assert ( M.num_cols() == M.num_rows() );
   assert ( M.num_cols() == 3 );

   double result = 0.0;
   result += M(1,1)*M(2,2)*M(3,3);
   result += M(1,2)*M(2,3)*M(3,1);
   result += M(1,3)*M(2,1)*M(3,2);
   result -= M(1,2)*M(2,2)*M(3,1);
   result -= M(1,1)*M(2,3)*M(3,2);
   result -= M(1,2)*M(2,1)*M(3,3);
   return result;
}

//---------------------------------------------------------------
TNT::Matrix<double> RigidTransform::getR()
{   
   return R;
}

//---------------------------------------------------------------
TNT::Matrix<double> RigidTransform::getT()
{
   return T;
}


