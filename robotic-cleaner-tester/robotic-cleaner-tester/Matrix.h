#ifndef __MATRIX__H_
#define __MATRIX__H_

#include <vector>
#include <assert.h>

using namespace std;

namespace ns_robotic_cleaner_simulator
{

	template <typename T> class Matrix
	{
	/*
	 * class Matrix
	 * class Matrix is used for saving generic 2d matrix, 
	 * with changing height and length
	 */
	private:
		
		vector<T> inner_;
		unsigned int cols, rows;

	public:

		Matrix (unsigned int rows = 0, unsigned int cols = 0)
			: cols (cols), rows (rows)
		{
			inner_ = vector<char>(cols*rows);
		}

		~Matrix()
		{
		}

		bool IsInsideMatrix(unsigned int row, unsigned int col) const
		{
			return (col < cols && row < rows);
		}

		bool IsInsideMatrix(const Point& location) const
		{
			return IsInsideMatrix(location.GetRow(), location.GetCol());
		}

		T& operator()(const Point& location)
		{
			assert( IsInsideMatrix(location) );
			int col = location.GetCol();
			int row = location.GetRow();
			return inner_[row*cols+col];
		}

		const T& operator()(const Point& location) const
		{
			assert( IsInsideMatrix(location) );
			int col = location.GetCol();
			int row = location.GetRow();
			return inner_[row*cols+col];
		}

		T& operator()(unsigned int row, unsigned int col)
		{
			assert( IsInsideMatrix(row, col) );
			return inner_[row*cols+col];
		}

		const T& operator()(unsigned int row, unsigned int col) const
		{
			assert( IsInsideMatrix(row, col) );
			return inner_[row*cols+col];
		}

		unsigned int getWidth() const
		{
			return cols;
		}

		unsigned int getHeight() const
		{
			return rows;
		}

	};

} // end of namespace ns_robotic_cleaner_simulator

#endif //__MATRIX__H_