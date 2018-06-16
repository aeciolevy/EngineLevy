#include "../exMatrix4.h"

float* exMatrix4::ToFloatPtr()
{
	return &m11;
}

const float* exMatrix4::ToFloatPtr() const
{
	return &m11;
}
