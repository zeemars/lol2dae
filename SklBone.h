#pragma once

struct SklBone
{
	char name[32];
	unsigned int hash;
	int parent;
	Vec3<float> scale;
	Matrix44<float> localMatrix;
	Matrix44<float> globalMatrix;
};