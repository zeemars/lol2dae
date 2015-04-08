#pragma once

struct Keyframe
{
	float keyframeTime;
	Vector3f translation;
	Quaternionf quaternion;
};

class AnmImporter
{
public:
	int numBones;
	int numFrames;
	float frameDelay;
	vector<AnmBone> bones;
	map<int, char*>& boneHashes;
	
	void readFile(const wchar_t* path);
	Quaternionf uncompressQuaternion(const unsigned short& flag, const unsigned short& sx, const unsigned short& sy, const unsigned short& sz);
	Vector3f uncompressVector(const Vector3f& min, const Vector3f& max, const unsigned short& sx, const unsigned short& sy, const unsigned short& sz);
	float uncompressTime(const unsigned short& ct, const float& animationLength);
	AnmImporter(map<int, char*>& boneHashes) : boneHashes(boneHashes){};
	~AnmImporter();
};

