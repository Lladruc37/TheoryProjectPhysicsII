// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __P2POINT_H__
#define __P2POINT_H__

#include <math.h>

template<class TYPE>
class p2Point
{
public:
	TYPE x, y;

	p2Point()
	{}

	p2Point(const p2Point& p)
	{
		x = p.x;
		y = p.y;
	}

	p2Point(const TYPE& newX, const TYPE& newY)
	{
		x = newX;
		y = newY;
	}

	// Math ------------------------------------------------
	p2Point operator -(const p2Point& v) const
	{
		p2Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	p2Point operator +(const p2Point& v) const
	{
		p2Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const p2Point& operator -=(const p2Point& v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const p2Point& operator +=(const p2Point& v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const p2Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const p2Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	p2Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	p2Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	p2Point& Sqrtf(float x)
	{
		float number;
		float sqrt;
		sqrt = x / 2;
		number = 0;
		if (x <= 0)
		{
			return number;
		}

		while (sqrt != number)
		{
			number = sqrt;
			sqrt = (x / number + number) / 2;
		}
		return sqrt;
	}

	// Distances ---------------------------------------------
	
	// Returns the distance between two points (always returns float)
	float DistanceTo(const p2Point& v) const
	{
		float fx = x - v.x;
		float fy = y - v.y;

		return sqrtf((fx * fx) + (fy * fy));
	}
};

typedef p2Point<int> iPoint;
typedef p2Point<float> fPoint;

#endif // __P2POINT_H__