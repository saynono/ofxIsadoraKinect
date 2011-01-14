// ---------------------------------------------------------------------------
//	¥ class Vector2D
// ---------------------------------------------------------------------------
// An object to represent a 3D vector or a 3D point in space

#ifndef _H_Vector2D
#define _H_Vector2D

#include <math.h>

class Vector2D
{
public:
	float x;												// the x value of this Vector2D
	float y;												// the y value of this Vector2D

	inline Vector2D()										// Default Constructor to set x = y = z = 0
	{
		x = 0;
		y = 0;
	}
	
	inline Vector2D(float x, float y)						// Constructor that initializes this Vector2D to the intended values of x, y and z
	{
		this->x = x;
		this->y = y;
	}

	inline bool operator== (const Vector2D& v) const		// operator== returns true if the two vectors are equal
	{
		return (this->x == v.x && this->y == v.y);
	}

	inline bool operator!= (const Vector2D& v) const		// operator!= returns true if the two vectors are not equal
	{
		return (this->x != v.x || this->y != v.y);
	}

	inline Vector2D operator+ (const Vector2D& v) const		// operator+ is used to add two Vector3D's. operator+ returns a new Vector2D
	{
		return Vector2D(x + v.x, y + v.y);
	}

	inline Vector2D operator- (const Vector2D& v) const		// operator- is used to take difference of two Vector3D's. operator- returns a new Vector2D
	{
		return Vector2D(x - v.x, y - v.y);
	}

	inline Vector2D operator* (float value) const			// operator* is used to scale a Vector2D by a value. This value multiplies the Vector3D's x, y and z.
	{
		return Vector2D(x * value, y * value);
	}

	inline Vector2D operator/ (float value) const			// operator/ is used to scale a Vector2D by a value. This value divides the Vector3D's x, y and z.
	{
		return Vector2D(x / value, y / value);
	}

	inline Vector2D& operator+= (const Vector2D& v)			// operator+= is used to add another Vector2D to this Vector2D.
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	inline Vector2D& operator-= (const Vector2D& v)			// operator-= is used to subtract another Vector2D from this Vector2D.
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline Vector2D& operator*= (float value)				// operator*= is used to scale this Vector2D by a value.
	{
		x *= value;
		y *= value;
		return *this;
	}

	inline Vector2D& operator/= (float value)				// operator/= is used to scale this Vector2D by a value.
	{
		x /= value;
		y /= value;
		return *this;
	}


	inline Vector2D operator- () const						// operator- is used to set this Vector3D's x, y, and z to the negative of them.
	{
		return Vector2D(-x, -y);
	}

	inline float length() const								// length() returns the length of this Vector2D
	{
		return sqrtf(x*x + y*y);
	}	   	

	inline bool unitize()									// unitize() normalizes this Vector2D that its direction remains the same but its length is 1.
	{
		float length = this->length();

		if (length == 0)
			return false;

		x /= length;
		y /= length;
		
		return true;
	}

	inline Vector2D unit()									// unit() returns a new Vector2D. The returned value is a unitized version of this Vector2D.
	{
		float length = this->length();

		if (length == 0)
			return *this;
		
		return Vector2D(x / length, y / length);
	}
};

#endif
