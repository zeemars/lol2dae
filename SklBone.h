#pragma once

#include <Eigen/Dense>
using namespace Eigen;

struct SklBone
{
	char name[32];
	int hash;
	int parent;
	float scale;
	Matrix<float, 4, 4, RowMajor> localMatrix;
	Matrix<float, 4, 4, RowMajor> globalMatrix;
};