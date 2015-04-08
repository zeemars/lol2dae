#pragma once

#include "stdafx.h"

#include <Eigen/Dense>
using namespace Eigen;

struct AnmBone
{
	char name[32];
	vector<pair<float, Vector3f>> translation;
	vector<pair<float, Quaternionf>, aligned_allocator<Quaternionf>> quaternion;
	vector<pair<float, Vector3f>> scale;
};