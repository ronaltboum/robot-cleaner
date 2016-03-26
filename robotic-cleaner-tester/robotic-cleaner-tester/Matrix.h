#ifndef __MATRIX__H_
#define __MATRIX__H_

#include "stdafx.h"
#include "Point.h"
#include <vector>
#include <assert.h>

using namespace std;

namespace ns_robotic_cleaner
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
		unsigned int dimx_, dimy_;

	public:

		Matrix (unsigned int dimx = 0, unsigned int dimy = 0)
			: dimx_ (dimx), dimy_ (dimy)
		{
			inner_ = vector<char>(dimx_*dimy_);
		}

		~Matrix()
		{
		}

		bool IsInsideMatrix(unsigned int x, unsigned int y) const
		{
			return (x < dimx_ && y < dimy_);
		}

		bool IsInsideMatrix(const Point& location) const
		{
			return (location.GetX() < dimx_ && location.GetY() < dimy_);
		}

		T& operator()(const Point& location)
		{
			assert( IsInsideMatrix(location) );
			return inner_[dimx_*location.GetY() + location.GetX()];
		}

		const T& operator()(const Point& location) const
		{
			assert( IsInsideMatrix(location) );
			return inner_[dimx_*location.GetY() + location.GetX()];
		}

		const T& operator()(unsigned int x, unsigned int y) const
		{
			assert( IsInsideMatrix(x,y) );
			return inner_[dimx_*y + x];
		}

		unsigned int getWidth() const
		{
			return dimx_;
		}

		unsigned int getHeight() const
		{
			return dimy_;
		}

	};

} // end of namespace ns_robotic_cleaner

#endif //__MATRIX__H_