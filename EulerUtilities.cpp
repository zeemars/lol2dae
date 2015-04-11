#pragma once
#include "stdafx.h"
#include "EulerUtilities.h"

const float EulerUtilities::PI = 3.14159265358979323846f;
const float EulerUtilities::degree = 180.0f / EulerUtilities::PI;

void EulerUtilities::radiansToDegrees(Euler<float>& r)
{
	r.x *= degree;
	r.y *= degree;
	r.z *= degree;

	return;
}

EulerUtilities::EulerUtilities()
{
}

EulerUtilities::~EulerUtilities()
{
}