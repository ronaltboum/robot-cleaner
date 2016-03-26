#ifndef __MATRIX__H_
#define __MATRIX__H_

#include <stdio.h>
#include "Point.h"

namespace (ns_robotic_cleaner) 
{

	template <typename T> class Matrix
	{
	private:
		std::vector<T> inner_;
		unsigned int dimx_, dimy_;

	public:

		Matrix (unsigned int dimx = 0, unsigned int dimy = 0)
			: dimx_ (dimx), dimy_ (dimy)
		{
			inner_ = new std::vector(dimx_*dimy_);
		}

		~Matrix()
		{
		}

		bool IsInsideMatrix(int x, int y) const
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

		const T& operator()(int x, int y) const
		{
			assert( IsInsideMatrix(x,y) );
			return inner_[dimx_*y + x];
		}

		int getWidth() const
		{
			return dimx_;
		}

		int getHeight() const
		{
			return dimy_;
		}

	};

} // end of namespace ns_robotic_cleaner

#endif //__MATRIX__H_