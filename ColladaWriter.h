#pragma once

class ColladaWriter
{
public:
	enum Mode
	{
		Mesh = 1,
		Skeleton,
		Animation
	};

private:
	const wchar_t* filePath;
	const vector<short>& indices;
	const vector<SknVertex>& vertices;
	vector<SklBone>& bones;
	const vector<int>& boneIndices;
	const AnmImporter& animation;
	list<int> nodes;
	ostringstream nodeStream;

	bool emplaceNode(int boneId);
	void createNodeStringStream();
	void writeHeader();
	void writeMaterial();
	void writeMesh();
	void writeSkin();
	void writeScene(Mode& mode);
	void writeAnimation();

public:
	void writeFile(const wchar_t* path, Mode& mode);
	ColladaWriter(vector<short>& indices, vector<SknVertex>& vertices, vector<SklBone>& bones, vector<int>& boneIndices, AnmImporter& animation)
		: indices(indices), vertices(vertices), bones(bones), boneIndices(boneIndices), animation(animation){};
	~ColladaWriter();
};

