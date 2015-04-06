#pragma once

class ColladaWriter
{
private:
	const wchar_t* filePath;
	FileLocation outputPath;
	const vector<short>& indices;
	const vector<SknVertex>& vertices;
	vector<SklBone, aligned_allocator<Matrix4f>>& bones;
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
	void writeScene();
	void writeAnimation();

public:
	bool includeSkl;
	bool includeAnm;

	void writeFile(const wchar_t* path);
	ColladaWriter(vector<short>& indices, vector<SknVertex>& vertices, vector<SklBone, aligned_allocator<Matrix4f>>& bones, vector<int>& boneIndices, AnmImporter& animation)
		: indices(indices), vertices(vertices), bones(bones), boneIndices(boneIndices), animation(animation){};
	~ColladaWriter();
};

