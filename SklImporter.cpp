#include "stdafx.h"
#include "SklImporter.h"

void SklImporter::readFile(const wchar_t* path)
{
	ifstream inFile(path, ios::in | ios::binary);
	if (!inFile.is_open())
	{
		throw lol2daeError("Unable to open skeleton file, please place it in the same directory as your mesh file.");
	}

	inFile.seekg(4, ios_base::cur);
	inFile.read(reinterpret_cast<char*>(&fileVersion), 4);

	if (fileVersion != 1953262451 && fileVersion != 587026371)
	{
		throw  lol2daeError("Skeleton file is invalid or unsupported.");
	}

	if (fileVersion == 1953262451)
	{
		inFile.seekg(8, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&numBones), 4);

		bones.resize(numBones);

		for (int i = 0; i < numBones; ++i)
		{
			inFile.read(reinterpret_cast<char*>(&bones.at(i).name), 32);
			inFile.read(reinterpret_cast<char*>(&bones.at(i).parent), 4);
			inFile.read(reinterpret_cast<char*>(&bones.at(i).scale), 4);
			inFile.read(reinterpret_cast<char*>(&bones.at(i).globalMatrix), 48);

			bones.at(i).globalMatrix(3, 0) = bones.at(i).globalMatrix(3, 1) = bones.at(i).globalMatrix(3, 2) = 0;
			bones.at(i).globalMatrix(3, 3) = 1;
		}

		for (auto& i : bones)
		{
			Matrix<float, 4, 4, RowMajor> relativeMatrix, inverseMatrix;

			if (i.parent != -1)
			{
				inverseMatrix = bones.at(i.parent).globalMatrix.inverse();
				i.localMatrix = inverseMatrix * i.globalMatrix;
			}

			else
			{
				i.localMatrix = i.globalMatrix;
			}
		}

		if (sknVersion == 0 || sknVersion == 1)
		{
			numBoneIndices = numBones;
			boneIndices.resize(numBoneIndices);
			for (int i = 0; i < numBoneIndices; ++i)
			{
				boneIndices.at(i) = i;
			}
		}

		else if (sknVersion == 2)
		{
			inFile.read(reinterpret_cast<char*>(&numBoneIndices), 4);

			boneIndices.resize(numBoneIndices);

			for (int i = 0; i < numBoneIndices; ++i)
			{
				inFile.read(reinterpret_cast<char*>(&boneIndices.at(i)), 4);
			}
		}
	}

	else if (fileVersion == 587026371)
	{
		inFile.seekg(6, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&numBones), 2);
		numBones = short(numBones);
		bones.resize(numBones);

		inFile.read(reinterpret_cast<char*>(&numBoneIndices), 4);
		boneIndices.resize(numBoneIndices);

		short dataOffset;
		inFile.read(reinterpret_cast<char*>(&dataOffset), 2);
		inFile.seekg(2, ios_base::cur);

		int boneIndicesOffset, boneNamesOffset;
		inFile.seekg(4, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&boneIndicesOffset), 4);
		inFile.seekg(8, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&boneNamesOffset), 4);

		inFile.seekg(dataOffset, ios_base::beg);

		for (int i = 0; i < numBones; ++i)
		{
			inFile.seekg(2, ios_base::cur);
			short boneId;
			inFile.read(reinterpret_cast<char*>(&boneId), 2);

			if (boneId != i)
			{
				throw  lol2daeError("Read error, unexpected identification value.");
			}

			inFile.read(reinterpret_cast<char*>(&bones[i].parent), 2);
			bones[i].parent = (short)bones[i].parent;

			inFile.seekg(2, ios_base::cur);

			inFile.read(reinterpret_cast<char*>(&bones.at(i).hash), 4);

			boneHashes[bones.at(i).hash] = bones.at(i).name;

			inFile.seekg(4, ios_base::cur);

			float tx, ty, tz;
			inFile.read(reinterpret_cast<char*>(&tx), 4);
			inFile.read(reinterpret_cast<char*>(&ty), 4);
			inFile.read(reinterpret_cast<char*>(&tz), 4);

			inFile.seekg(12, ios_base::cur);

			Quaternionf q;

			inFile.read(reinterpret_cast<char*>(&q), 16);

			Matrix<float, 3, 3, RowMajor> rotationMatrix;

			rotationMatrix = q.matrix();

			bones.at(i).localMatrix(0, 0) = rotationMatrix(0, 0);
			bones.at(i).localMatrix(0, 1) = rotationMatrix(0, 1);
			bones.at(i).localMatrix(0, 2) = rotationMatrix(0, 2);
			bones.at(i).localMatrix(1, 0) = rotationMatrix(1, 0);
			bones.at(i).localMatrix(1, 1) = rotationMatrix(1, 1);
			bones.at(i).localMatrix(1, 2) = rotationMatrix(1, 2);
			bones.at(i).localMatrix(2, 0) = rotationMatrix(2, 0);
			bones.at(i).localMatrix(2, 1) = rotationMatrix(2, 1);
			bones.at(i).localMatrix(2, 2) = rotationMatrix(2, 2);

			bones.at(i).localMatrix(0, 3) = tx;
			bones.at(i).localMatrix(1, 3) = ty;
			bones.at(i).localMatrix(2, 3) = tz;

			bones.at(i).localMatrix(3, 0) = bones.at(i).localMatrix(3, 1) = bones.at(i).localMatrix(3, 2) = 0;
			bones.at(i).localMatrix(3, 3) = 1;

			inFile.seekg(44, ios_base::cur);
		}

		for (auto& i : bones)
		{
			if (i.parent != -1)
			{
				i.globalMatrix = bones.at(i.parent).globalMatrix * i.localMatrix;
			}

			else
			{
				i.globalMatrix = i.localMatrix;
			}
		}

		inFile.seekg(boneIndicesOffset, ios_base::beg);

		for (int i = 0; i < numBoneIndices; ++i)
		{
			inFile.read(reinterpret_cast<char*>(&boneIndices.at(i)), 2);
			boneIndices.at(i) = short(boneIndices.at(i));
		}

		inFile.seekg(boneNamesOffset, ios_base::beg);

		for (int i = 0; i < numBones; ++i)
		{
			unsigned char curChar;
			streamoff filePos;

			do
			{
				filePos = inFile.tellg();
				inFile.read(reinterpret_cast<char*>(&curChar), 1);
			} while (filePos % 4 != 0);

			int curPos = 0;

			while (curChar)
			{
				bones.at(i).name[curPos] = curChar;
				++curPos;

				if (curPos >= 31)
				{
					break;
				}

				inFile.read(reinterpret_cast<char*>(&curChar), 1);
			}

			bones.at(i).name[curPos] = curChar;
		}
	}

	inFile.close();

	return;
}

SklImporter::~SklImporter()
{
}
