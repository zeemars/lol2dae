#pragma once
#include "SknVertex.h"

class SknImporter
{
private:
	int magicNumber;
	short matHeader;
	int numMaterials;

	bool checkMagicNumber();

public:
	int numIndices;
	int numVertices;
	vector<short> indices;
	vector<SknVertex> vertices;
	short fileVersion;

	void readFile(const wchar_t* path);
	SknImporter();
	~SknImporter();

};

