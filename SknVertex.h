#pragma once

struct SknVertex
{
	Vec3<float> position;
	char boneIndices[4];
	float boneWeights[4];
	Vec3<float> normal;
	float u;
	float v;
};