#pragma once

struct Keyframe
{
	float keyframeTime;
	Vec3<float> translation;
	Quat<float> quaternion;
};

class AnmImporter
{
public:
	int numBones;
	int numFrames;
	float frameDelay;
	vector<AnmBone> bones;

	void readFile(const wchar_t* path);
	AnmImporter(map<unsigned int, char*>& boneHashes) : boneHashes(boneHashes){};
	~AnmImporter();

private:
	const map<unsigned int, char*>& boneHashes;
	unsigned int StringToHash(string s);

	Quat<float> uncompressQuaternion(const unsigned short& flag, const unsigned short& sx, const unsigned short& sy, const unsigned short& sz);
	Vec3<float> uncompressVector(const Vec3<float>& min, const Vec3<float>& max, const unsigned short& sx, const unsigned short& sy, const unsigned short& sz);
	float uncompressTime(const unsigned short& ct, const float& animationLength);
};

