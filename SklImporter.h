#pragma once

class SklImporter
{
private:
	int fileVersion;
	const short& sknVersion;
public:
	int numBones;
	vector<SklBone, aligned_allocator<Matrix4f>> bones;
	int numBoneIndices;
	vector<int> boneIndices;
	map<int, char*> boneHashes;

	void readFile(const wchar_t* path);
	SklImporter(short& sknVersion) : sknVersion(sknVersion){};
	~SklImporter();
};

