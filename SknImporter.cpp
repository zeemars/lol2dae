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

	float rawV;

	for (int i = 0; i < numVertices; ++i)
	{
		// Positions
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).positionX), 4);
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).positionY), 4);
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).positionZ), 4);

		// Bone IDs
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).boneId), 4);

		// Weights
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).boneWeight), 16);

		vertices.at(i).numWeights = 0;

		for (int j = 0; j < 4; ++j)
		{
			if (vertices[i].boneWeight[j] > 0.0f)
			{
				++vertices.at(i).numWeights;
			}
		}

		// Normals
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).normalX), 4);
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).normalY), 4);
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).normalZ), 4);

		// UV Coordinates
		inFile.read(reinterpret_cast<char*>(&vertices.at(i).textureU), 4);
		inFile.read(reinterpret_cast<char*>(&rawV), 4);

		// Invert V Coordinate
		if (invertTextureV)
		{
			vertices[i].textureV = 1 - rawV;
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
