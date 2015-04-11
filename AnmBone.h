#pragma once

#include "stdafx.h"

struct AnmBone
{
	char name[32];
	vector<pair<float, Vec3<float>>> translation;
	vector<pair<float, Quat<float>>> quaternion;
	vector<pair<float, Vec3<float>>> scale;
};