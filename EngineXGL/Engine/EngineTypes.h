//
// * ENGINE-X
//
// + Types.h
// representations of: 2D vector with floats, 2D vector with integers and RBGA color as four bytes
//

#pragma once
#include <math.h>
//-----------------------------------------------------------------
//-----------------------------------------------------------------

const float kViewPortWidth = 800.0f;
const float kViewPortWidthMin = 0.0f;
const float kViewPortHeightMin = 0.0f;
const float kViewPortHeight = 600.0f;
const float PaddleHeight = 150.0f;

struct exVector2
{
	float x, y;
	exVector2 Normalize()
	{
		float sumSquare = powf(x, 2) + powf(y,2);
		float magnitude = sqrtf(sumSquare);
		exVector2 uVector = { x / magnitude, y / magnitude };
		return uVector;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}
		return value;
	}

	exVector2 Abs()
	{
		return { fabsf(x), fabsf(y) };
	}

	float Magnitude()
	{
		float sumSquare = powf(x, 2) + powf(y, 2);
		return sqrtf(sumSquare);
	}

	exVector2 GetVector(exVector2 finalPosition)
	{
		return { finalPosition.x - x, finalPosition.y - y };
	}

	float Dot(exVector2 vec1)
	{
		return vec1.x * x + vec1.y * y;
	}

	exVector2 Add(exVector2 vec)
	{
		return { x + vec.x, y + vec.y };
	}

	exVector2 Sub(exVector2 vec)
	{
		return { x - vec.x, y - vec.y };
	}

	exVector2 VectorDirection()
	{
		float xDirection = x >= 0 ? 1.0f : -1.0f;
		float yDirection = y >= 0 ? 1.0f : -1.0f;
		return { xDirection, yDirection };
	}

	exVector2 Mult(exVector2 vec)
	{
		return { x * vec.x, y * vec.y };
	}

	exVector2 MultVectorByFactor(float factor)
	{
		return { x * factor, y * factor };
	}
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct exIntegerVector2
{
	int x, y;
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------

struct exColor
{
	unsigned char mColor[4];
	void SetColor(int r, int g, int b, int a)
	{
		mColor[0] = (char) r;
		mColor[1] = (char) g;
		mColor[2] = (char) b;
		mColor[3] = (char) a;
	}
};

struct exColorF
{
	float mColor[4];
	void SetColor(int r, int g, int b, int a)
	{
		mColor[0] = (char)r;
		mColor[1] = (char)g;
		mColor[2] = (char)b;
		mColor[3] = (char)a;
	}
};