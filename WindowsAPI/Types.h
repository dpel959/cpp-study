#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct Stat
{
	int32 hp = 0;
	int32 maxHp = 0;
	float speed = 0;
};

struct Vector
{
public:

	Vector() {};
	Vector(float x, float y) : x(x), y(y) {}
	Vector(POINT pt) : x(static_cast<float>(pt.x)), y(static_cast<float>(pt.y)) {}

	Vector operator+ (const Vector& other)
	{
		Vector ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	Vector operator- (const Vector& other)
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vector operator* (const float value)
	{
		Vector ret;
		ret.x = value * x;
		ret.y = value * y;
		return ret;
	}

	Vector& operator+= (const Vector& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector& operator-= (const Vector& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector operator*= (const float value)
	{
		x *= value;
		y *= value;
		return *this;
	}

	float LengthSquared()
	{
		return x * x + y * y;
	}

	float Length()
	{
		return ::sqrt(LengthSquared());
	}

	void Normalize()
	{
		float length = Length();
		// float 값이니까... 오차가 있을수도 있으니 < 0으로는 하지 않는다.
		if (length < 0.0000000001f)
		{
			return;
		}
		else
		{
			x = x / length;
			y = y / length;
		}
	}

	float Dot(Vector other)
	{
		return (x * other.x + y * other.y);
	}

	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	float x = 0;
	float y = 0;
};

using Pos = Vector;