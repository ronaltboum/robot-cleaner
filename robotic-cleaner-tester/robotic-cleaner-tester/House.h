#ifndef __HOUSE__H_
#define __HOUSE__H_

#include <stdio.h>
template <typename T>
class Matrix
{
        std::vector<T> inner_;
        unsigned int dimx_, dimy_;

public:
        Matrix (unsigned int dimx, unsigned int dimy)
                : dimx_ (dimx), dimy_ (dimy)
        {
                inner_.resize (dimx_*dimy_);
        }

        T& operator()(unsigned int x, unsigned int y)
        {
                if (x >= dimx_ || y>= dimy_)
                        throw 0; // ouch
                return inner_[dimx_*y + x];
        }
};


class House 
{ 

private: 
	int					width;		//: the house witdh
	int					height;		//: the house height
	Matrix<char>	floor;		//: the floor of the house


public:
	virtual ~House() {}
    void initialize();

	//~~~~~~~~~~~~~~~~~`getters~~~~~~~~~~~~~~~~`
	int GetHeight() const
	{
		return height;
	}
	int GetWidth() const
	{
		return width;
	}
	
	//~~~~~~~~~~~~~~~~~`functions~~~~~~~~~~~~~~~~`
	bool IsWall(int w, int h) const; 
	int GetDirtLevel(unsigned int w, unsigned int h) const
	{
		char a = floor(w,h);
		return int(floor(w,h)-'0');
	}
};

#endif //__ABSTRACT_HOUSE__H_