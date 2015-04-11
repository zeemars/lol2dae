#pragma once

class EulerUtilities
{
public:
	static const float PI;
	static const float degree;

	static void radiansToDegrees(Euler<float>& r);
	EulerUtilities();
	~EulerUtilities();
};