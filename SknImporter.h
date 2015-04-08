#pragma once
#include "SknVertex.h"

using namespace std;

class SknImporter
{
private:
	int magicNumber;
	short matHeader;
	int numMaterials;

	bool checkMagicNumber();

public:
	bool applyOffset;
	bool invertTextureV;
	int numIndices;
	int numVertices;
	vector<short> indices;
	vector<SknVertex> vertices;
	short fileVersion;

	void readFile(const wchar_t* path);
	SknImporter();
	~SknImporter();

};

