#include "stdafx.h"
#include "SknImporter.h"

bool SknImporter::checkMagicNumber()
{
	if (magicNumber != 1122867)
	{
		return false;
	}

	return true;
}

void SknImporter::readFile(const wchar_t* path)
{
	ifstream inFile(path, ios::in | ios::binary);
	if (!inFile.is_open())
	{
		throw lol2daeError("Unable to open input file.");
	}
	
	// Magic Number
	inFile.read(reinterpret_cast<char*>(&magicNumber), 4);

	if (!checkMagicNumber())
	{
		throw lol2daeError("Corrupted or invalid header in file.");
	}
	
	// Header Block
	inFile.read(reinterpret_cast<char*>(&fileVersion), 2);
	inFile.read(reinterpret_cast<char*>(&matHeader), 2);

	if (fileVersion > 4)
	{
		throw lol2daeError("File version is not supported.");
	}

	if (!matHeader == 1)
	{
		throw lol2daeError("The file could not be parsed.");
	}

	if (fileVersion > 0)
	{
		inFile.read(reinterpret_cast<char*>(&numMaterials), 4);

		// Material Block
		for (int i = 0; i < numMaterials; ++i)
		{
			inFile.seekg(80, ios_base::cur);
		}
	}

	if (fileVersion == 4)
	{
		inFile.seekg(4, ios_base::cur);
	}

	// Count Block
	inFile.read(reinterpret_cast<char*>(&numIndices), 4);
	inFile.read(reinterpret_cast<char*>(&numVertices), 4);

	if (fileVersion == 4)
	{
		inFile.seekg(48, ios_base::cur);
	}

	// Index Block
	indices.resize(numIndices);
	for (int i = 0; i < numIndices; ++i)
	{
		inFile.read(reinterpret_cast<char*>(&indices.at(i)), 2);
	}

	// Vertex Block
	vertices.resize(numVertices);

	for (int i = 0; i < numVertices; ++i)
	{
		// Positions
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).position), 12);

		// Bone IDs
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).boneIndices), 4);

		// Weights
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).boneWeights), 16);

		// Normals
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).normal), 12);

		// UV Coordinates
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).u), 4);
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).v), 4);

		vertices.at(i).v = 1.0f - vertices.at(i).v;
	}

	for (auto& i : vertices)
	{
		const float totalWeight = i.boneWeights[0] + i.boneWeights[1] + i.boneWeights[2] + i.boneWeights[3];
		const float weightError = 1.0f - totalWeight;

		if (weightError != 0.0f)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i.boneWeights[j] > 0.0f)
				{
					i.boneWeights[j] += (i.boneWeights[j] / totalWeight) * weightError;
				}
			}	
		}
	}

	inFile.close();

	return;
}

SknImporter::SknImporter()
{
}

SknImporter::~SknImporter()
{
}
