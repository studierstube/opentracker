

#include <assert.h>
#include <cstring> // memset(), memcpy()

#include "matrix.h"

template <class T>
matrix_t<T>::matrix_t()
 : values_(0)
{
	cout << "ERROR: create a matrix without specifying an initial size!" << endl;
	assert(0);
};

template <class T>
matrix_t<T>::matrix_t(unsigned int r, unsigned int c)
 : rows_(r), columns_(c), values_(0)
{
	new_mem();
};

template <class T>
matrix_t<T>::matrix_t(unsigned int r, unsigned int c, const T *values)
 : rows_(r), columns_(c), values_(0)
{
	new_mem();
	*this = values;
};

template <class T>
matrix_t<T>::matrix_t(const matrix_t &src)
 : values_(0)
{
	*this = src;
};

template <class T>
matrix_t<T>& matrix_t<T>::operator = (const matrix_t &src)
{
	if (this->values_ == &src) return *this;

	rows_ = src.rows_;
	columns_ = src.columns_;
	new_mem();
	memcpy(values_, src.values_, sizeof(T) * rows_ * columns_);

	return *this;
};

template <class T>
matrix_t<T>::~matrix_t()
{
	delete_mem();
};

template <class T>
void matrix_t<T>::new_mem(void)
{
	size_t count = rows_ * columns_;
	delete_mem();
	values_ = new T[count];
	assert(values_);
	memset(values_, 0, sizeof(T) * count);
};

template <class T>
void matrix_t<T>::delete_mem(void)
{
	if (values_) delete[] values_;
	values_ = 0;
};

template <class T>
matrix_t<T>& matrix_t<T>::operator = (const T *values)
{
	memcpy(values_, values, sizeof(T) * rows_ * columns_);
	return *this;
};

template <class T>
matrix_t<T> matrix_t<T>::operator + (const matrix_t &src) const
{
	matrix_t tmp(*this);
	tmp += src;
	return tmp;
};

template <class T>
matrix_t<T>& matrix_t<T>::operator += (const matrix_t &src)
{
	assert(rows_ == src.rows_ && columns_ == src.columns_);
	unsigned int num = rows_ * columns_;
	for (unsigned int i=0; i<num; ++i)
		values_[i] += src.values_[i];
	return *this;
};

template <class T>
matrix_t<T> matrix_t<T>::operator - (const matrix_t &src) const
{
	matrix_t tmp(*this);
	tmp -= src;
	return tmp;
};

template <class T>
matrix_t<T>& matrix_t<T>::operator -= (const matrix_t &src)
{
	if (values_ == &src)
		memset(values_, 0, sizeof(T) * rows_ * columns_);
	else
		{
		assert(rows_ == src.rows_ && columns_ == src.columns_);
		unsigned int num = rows_ * columns_;
		for (unsigned int i=0; i<num; ++i)
			values_[i] -= src.values_[i];
		}

	return *this;
};

template <class T>
matrix_t<T> matrix_t<T>::operator * (T factor) const
{
	matrix_t tmp(*this);
	tmp *= factor;
	return tmp;
};

template <class T>
matrix_t<T>& matrix_t<T>::operator *= (T factor)
{
	unsigned int num = rows_ * columns_;
	for (unsigned int i=0; i<num; ++i)
		values_[i] *= factor;
	return *this;
};

template <class T>
matrix_t<T> matrix_t<T>::operator * (const matrix_t &src) const
{
   assert(src.rows_ == columns_);
   matrix_t tmp(rows_, src.columns_);
   for(unsigned int i=0; i<rows_; ++i)
      for(unsigned int j=0; j<src.columns_; ++j)
         {
         unsigned int index = j*rows_ + i;
         tmp.values_[index] = (T)0;
         for(unsigned int k=0; k<columns_; ++k)
            tmp.values_[index] += values_[k*rows_ + i] * src.values_[j*src.rows_ + k];
         }
   return tmp;
};

template <class T>
matrix_t<T>& matrix_t<T>::operator *= (const matrix_t &src)
{
	matrix_t tmp(*this);
	*this = tmp * src;
	return *this;
};

template <class T>
matrix_t<T> matrix_t<T>::submatrix(unsigned int delete_row, unsigned int delete_column)const
{
	matrix_t subm(rows_-1, columns_-1);

	unsigned int dest_row, dest_column = 0;

	for(unsigned int i=0; i<columns_; ++i)
		if (i != delete_column)
			{
			const T *src_column_ptr = &values_[i*rows_];
			T *dest_column_ptr = subm[dest_column];

			dest_row = 0;
			for(unsigned int j=0; j<rows_; ++j)
				if (j != delete_row)
					dest_column_ptr[dest_row++] = src_column_ptr[j];

			++dest_column;
			}

	return subm;
};

template <class T>
void matrix_t<T>::print(std::ostream &stream, const char *name) const
{
	stream << columns_ << "x" << rows_ << " Matrix: " << (int)this;
	if (name)
		stream << ", " << name;
	stream << endl;

	for(unsigned int i=0; i<rows_; ++i)
		{
		stream << "    ";
		for(unsigned int j=0; j<columns_; ++j)
			stream << values_[rows_*j + i] << " ";
		stream << endl;
		}
};

