#pragma once
class CVector
{
public:
	float x, y, z;

	CVector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	CVector(const CVector& vec)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// Basic math operations
	CVector operator=(const CVector& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;

		return *this;
	}

	CVector operator+(const CVector& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;

		return *this;
	}

	CVector operator-(const CVector& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;

		return *this;
	}

	CVector operator*(const CVector& vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;

		return *this;
	}

	CVector operator/(const CVector& vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;

		return *this;
	}

	bool operator==(const CVector& vec)
	{
		return (this->x == vec.x && this->y == vec.y && this->z == vec.z);
	}
};

