// ---------------------------------------------------------------------------
//	¥ class Vector3D
// ---------------------------------------------------------------------------
// An object to represent a 3D vector or a 3D point in space

#ifndef _H_Vector3D
#define _H_Vector3D

#include <math.h>

#define	v_num	float

class Vector3D
{
public:
	v_num x;										// the x value of this Vector3D
	v_num y;										// the y value of this Vector3D
	v_num z;										// the z value of this Vector3D

	inline Vector3D()								// Constructor to set x = y = z = 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	inline Vector3D(v_num x, v_num y, v_num z)		// Constructor that initializes this Vector3D to the intended values of x, y and z
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline Vector3D(
		const Vector3D&	a)
	{
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
	}

	inline bool operator== (const Vector3D& v) const		// operator== returns true if the two vectors are equal
	{
		return (this->x == v.x && this->y == v.y && this->z == v.z);
	}

	inline bool operator!= (const Vector3D& v) const		// operator!= returns true if the two vectors are not equal
	{
		return (this->x != v.x || this->y != v.y || this->z != v.z);
	}

	inline Vector3D& operator= (const Vector3D& v)			// operator= sets values of v to this Vector3D. example: v1 = v2 means that values of v2 are set onto v1
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vector3D operator+ (const Vector3D& v) const		// operator+ is used to add two Vector3D's. operator+ returns a new Vector3D
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3D operator- (const Vector3D& v) const		// operator- is used to take difference of two Vector3D's. operator- returns a new Vector3D
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3D operator* (v_num value) const			// operator* is used to scale a Vector3D by a value. This value multiplies the Vector3D's x, y and z.
	{
		return Vector3D(x * value, y * value, z * value);
	}

	inline Vector3D operator* (const Vector3D& value) const			// operator* is used to scale a Vector3D by a value. This value multiplies the Vector3D's x, y and z.
	{
		return Vector3D(x * value.x, y * value.y, z * value.z);
	}

	inline Vector3D operator/ (v_num value) const			// operator/ is used to scale a Vector3D by a value. This value divides the Vector3D's x, y and z.
	{
		return Vector3D(x / value, y / value, z / value);
	}

	inline Vector3D& operator+= (const Vector3D v)			// operator+= is used to add another Vector3D to this Vector3D.
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector3D& operator-= (const Vector3D v)			// operator-= is used to subtract another Vector3D from this Vector3D.
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector3D& operator*= (v_num value)				// operator*= is used to scale this Vector3D by a value.
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	inline Vector3D& operator/= (v_num value)				// operator/= is used to scale this Vector3D by a value.
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}


	inline Vector3D operator- () const						// operator- is used to set this Vector3D's x, y, and z to the negative of them.
	{
		return Vector3D(-x, -y, -z);
	}

	inline v_num length() const								// length() returns the length of this Vector3D
	{
		return sqrtf(x*x + y*y + z*z);
	};			   		

	inline bool unitize()									// unitize() normalizes this Vector3D that its direction remains the same but its length is 1.
	{
		v_num length = this->length();

		if (length == 0)
			return false;

		x /= length;
		y /= length;
		z /= length;
		
		return true;
	}

	inline Vector3D unit()									// unit() returns a new Vector3D. The returned value is a unitized version of this Vector3D.
	{
		v_num length = this->length();

		if (length == 0)
			return *this;
		
		return Vector3D(x / length, y / length, z / length);
	}
	
	static inline void CrossProduct(
							Vector3D&		p,
							const Vector3D&	a,
							const Vector3D&	b)
						{
						   p.x = a.y * b.z - a.z * b.y;
						   p.y = a.z * b.x - a.x * b.z;
						   p.z = a.x * b.y - a.y * b.x;
						}
};

#endif
