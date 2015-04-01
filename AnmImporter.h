#pragma once

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
	AnmImporter(map<int, char*>& boneHashes) : boneHashes(boneHashes){};
	~AnmImporter();
};

