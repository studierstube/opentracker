

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>
#include <assert.h>

// the class matrix_t<T> organizes its values in a single array with
// the complete columns stored sequentially (column-major order)

template <class T>
class matrix_t
	{
   	public:
		// have to specify an initial size of matrix at creation time
		// matrix will be initialized with zero values:
      	matrix_t(unsigned int rows, unsigned int columns); 

		// matrix will be initialized with given array (no range check!)::
      	matrix_t(unsigned int rows, unsigned int columns, const T *values); 

		// copy stuff:
      	matrix_t(const matrix_t&);
      	matrix_t& operator = (const matrix_t&);

		// set the whole matrix at once (no range check!):
      	matrix_t& operator = (const T*);

		// destructor:
      	virtual ~matrix_t();

      	inline unsigned int num_rows(void) const;
      	inline unsigned int num_columns(void) const;

		// to get the pointer to the whole matrix:
		inline const T* operator & (void) const;
		inline       T* operator & (void);

		// elementar access to the columns of a matrix (without range check):
     	inline const T* operator [] (unsigned int column) const;
     	inline T* operator [] (unsigned int column);

		// elementar access to the elements of a matrix with range check:
      	inline T  get_element(unsigned int row, unsigned int column) const;
      	inline T& get_element(unsigned int row, unsigned int column);

		// operations:
      	matrix_t operator + (const matrix_t&)const;
      	matrix_t operator - (const matrix_t&)const;
		matrix_t operator * (T f) const;

		// returns this * m:
      	matrix_t operator * (const matrix_t &m) const;

		matrix_t& operator += (const matrix_t&);
      	matrix_t& operator -= (const matrix_t&);
      	matrix_t& operator *= (T);

		// applies m from the right:
      	matrix_t& operator *= (const matrix_t &m);

		// generates the sub-matrix through removing one row and one column:
		matrix_t submatrix(unsigned int delete_row, unsigned int delete_column) const;

		// prints out:
		void print(std::ostream &stream = std::cout, const char *name=0) const;

   	protected:
      	unsigned int rows_, columns_;
      	T *values_;

		void delete_mem(void);
		void new_mem(void);

	private:
		// have to specify an initial size of matrix at creation time:
		matrix_t();
	};

//***********************************************************************************
// inline functions of template class matrix_t:

template <class T>
inline unsigned int matrix_t<T>::num_rows(void) const
{
	return rows_;
};

template <class T>
inline unsigned int matrix_t<T>::num_columns(void) const
{
	return columns_;
};

template <class T>
inline const T* matrix_t<T>::operator & (void) const
{
	return values_;
};

template <class T>
inline T* matrix_t<T>::operator & (void)
{
	return values_;
};

template <class T>
inline const T* matrix_t<T>::operator [] (unsigned int column) const
{
	return &values_[column * rows_];
};

template <class T>
inline T* matrix_t<T>::operator [] (unsigned int column)
{
	return &values_[column * rows_];
};

template <class T>
inline T matrix_t<T>::get_element(unsigned int row, unsigned int column) const
{
	assert(row < rows_ && column < columns_);
	return values_[column * rows_ + row];
};

template <class T>
inline T& matrix_t<T>::get_element(unsigned int row, unsigned int column)
{
	assert(row < rows_ && column < columns_);
	return values_[column * rows_ + row];
};

// have to include the rest of the implementation, because compilers make all
// member functions of a class template inline:
#ifdef ALL_INLINE
#include "matrix.cpp"
#endif

#endif // MATRIX_H_INCLUDED

