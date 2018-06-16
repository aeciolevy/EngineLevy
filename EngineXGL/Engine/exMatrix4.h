#pragma once
class exMatrix4
{
public:
	exMatrix4() = default;

	float*				ToFloatPtr();
	const float*		ToFloatPtr() const;

public:
	float				m11, m12, m13, m14;
	float				m21, m22, m23, m24;
	float				m31, m32, m33, m34;
	float				m41, m42, m43, m44;
};