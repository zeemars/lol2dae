#pragma once

#include "stdafx.h"

#include <Eigen/Dense>
using namespace Eigen;

struct AnmBone
{
	char name[32];
	vector<Vector3f> translation;
	vector<Quaternionf, aligned_allocator<Quaternionf>> quaternion;
};