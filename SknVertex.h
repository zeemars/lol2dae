#pragma once

struct SknVertex
{
	float positionX;
	float positionY;
	float positionZ;
	char boneId[4];
	float boneWeight[4];
	float normalX;
	float normalY;
	float normalZ;
	float textureU;
	float textureV;
};