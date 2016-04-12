#ifndef __MATRIX__H_
#define __MATRIX__H_

#include <vector>
#include <assert.h>

using namespace std;

namespace ns_robotic_cleaner_simulator
{
	/*
	 * class Matrix
	 * class Matrix is used for saving generic 2d matrix, 
	 * with changing height and length
	 */
	template <typename T> class Matrix
	{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
		vector<T> _inner; // vector where the matrix store values
		unsigned int cols; //: the number of cols in the matrix
		unsigned int rows; //: the number of rows in the matrix
		T default_value; //the values of added rows/ cols
		
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		Matrix (unsigned int rows = 3, unsigned int cols = 3);
		Matrix (unsigned int rows , unsigned int cols , T newRowsColsValue);
		~Matrix(){ }

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		unsigned int getWidth() const { return cols; }
		unsigned int getHeight() const { return rows; }
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		bool IsInsideMatrix(unsigned int row, unsigned int col) const;
		bool IsInsideMatrix(const Point& location) const;
		T& operator()(const Point& location);
		const T& operator()(const Point& location) const;
		T& operator()(unsigned int row, unsigned int col);
		const T& operator()(unsigned int row, unsigned int col) const;
		void AddLastColumn();
		void AddFirstColumn();
		void AddFirstRow();
		void AddLastRow();
	private:
		void CopyRow(int srcRowIndex, int targetRowIndex);
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~
	template <typename T> inline Matrix<T>::Matrix (unsigned int rows /* =3 */, unsigned int cols /* =3 */)
		: cols (cols), rows (rows)
	{
		_inner = vector<T>(rows * cols);
	}

	template <typename T> inline Matrix<T>::Matrix (unsigned int rows , unsigned int cols , T newRowsColsValue)
		: cols (cols), rows (rows)
	{
		default_value = newRowsColsValue;
		_inner.assign(rows * cols, default_value);
	}

	template <typename T> inline bool Matrix<T>::IsInsideMatrix(unsigned int row, unsigned int col) const
	{
		return (col < cols && row < rows);
	}

	template <typename T> inline bool Matrix<T>::IsInsideMatrix(const Point& location) const
	{
		return IsInsideMatrix(location.GetRow(), location.GetCol());
	}

	template <typename T> inline T& Matrix<T>::operator()(const Point& location)
	{
		assert( IsInsideMatrix(location) );
		int col = location.GetCol();
		int row = location.GetRow();
		return _inner[row*cols+col];
	}

	template <typename T> inline const T& Matrix<T>::operator()(const Point& location) const
	{
		assert( IsInsideMatrix(location) );
		int col = location.GetCol();
		int row = location.GetRow();
		return _inner[row*cols+col];
	}

	template <typename T> inline T& Matrix<T>::operator()(unsigned int row, unsigned int col)
	{
		assert( IsInsideMatrix(row, col) );
		return _inner[row*cols+col];
	}

	template <typename T> inline const T& Matrix<T>::operator()(unsigned int row, unsigned int col) const
	{
		assert( IsInsideMatrix(row, col) );
		return _inner[row*cols+col];
	}

	template <typename T> inline void Matrix<T>::AddLastColumn()
	{
		int col, row, old_cols = cols;
		_inner.resize(rows * ++cols, default_value);
		int offsetRight = rows;
		for(row = rows - 1; row > 0; --row) //not moving first row
		{
			col = old_cols - 1;
			_inner[row*old_cols + col + offsetRight--] = default_value; // add default value to the last column in the row
			for(; col >= 0; --col)
			{
				int moveTo = row*old_cols + col + offsetRight;
				int moveFrom = row*old_cols+col;
				_inner[moveTo] = _inner[moveFrom];
			}
		}
		_inner[cols - 1] = default_value; // add default value to first row

	}

	template <typename T> inline void Matrix<T>::AddFirstColumn()
	{
		int old_cols = cols;
		cols++;
		_inner.resize(rows * cols, default_value);
		int offsetRight = rows;
		for(int row = rows - 1; row >= 0; --row)
		{
			for(int col = cols - 2; col >= 0; --col)
			{
				int moveTo = row*old_cols + col + offsetRight;
				int moveFrom = row*old_cols+col;
				_inner[moveTo] = _inner[moveFrom];
			}
			_inner[row*old_cols + --offsetRight] = default_value;
		}
	}

	template <typename T> inline void Matrix<T>::CopyRow(int srcRowIndex, int targetRowIndex)
	{
		for(unsigned int col = 0; col < cols; col ++)
		{
			_inner[targetRowIndex*cols+col] = _inner[srcRowIndex*cols+col];
		}
	}

	template <typename T> inline void Matrix<T>::AddFirstRow()
	{
		AddLastRow();
		for(int row = rows -2; row >=0 ; row--){
			CopyRow(row, row + 1);
		}
	}

	template <typename T> inline void Matrix<T>::AddLastRow()
	{
		rows++;
		_inner.resize(rows * cols, default_value);
	}



} // end of namespace ns_robotic_cleaner_simulator

#endif //__MATRIX__H_