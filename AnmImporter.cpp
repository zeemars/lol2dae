#include "stdafx.h"
#include "AnmImporter.h"

void AnmImporter::readFile(const wchar_t* path)
{
	ifstream inFile(path, ios::in | ios::binary);
	if (!inFile.is_open())
	{
		throw lol2daeError("Unable to open animation file.");
	}

	char magicNumber[8];
	inFile.read(magicNumber, 8);

	if (memcmp(magicNumber, "r3d2anmd", 8) != 0)
	{
		throw  lol2daeError("Animation file is invalid or unsupported.");
	}

	int version;
	inFile.read(reinterpret_cast<char*>(&version), 4);

	if (version > 5)
	{
		throw  lol2daeError("Animation version is currently unsupported.");
	}

	if (version == 3)
	{
		inFile.seekg(4, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&numBones), 4);
		inFile.read(reinterpret_cast<char*>(&numFrames), 4);
		int framesPerSecond;
		inFile.read(reinterpret_cast<char*>(&framesPerSecond), 4);
		frameDelay = 1.0f / framesPerSecond;

		bones.resize(numBones);

		for (int i = 0; i < numBones; ++i)
		{
			inFile.read(reinterpret_cast<char*>(&bones.at(i).name), 32);
			inFile.seekg(4, ios_base::cur);

			bones.at(i).translation.resize(numFrames);
			bones.at(i).quaternion.resize(numFrames);

			for (int j = 0; j < numFrames; ++j)
			{
				inFile.read(reinterpret_cast<char*>(&bones.at(i).quaternion.at(j)), 16);
				inFile.read(reinterpret_cast<char*>(&bones.at(i).translation.at(j)), 12);
			}
		}

	}

	else if (version == 4)
	{
		inFile.seekg(16, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&numBones), 4);
		inFile.read(reinterpret_cast<char*>(&numFrames), 4);
		inFile.read(reinterpret_cast<char*>(&frameDelay), 4);
		inFile.seekg(12, ios_base::cur);

		int translationsOffset, quaternionsOffset, framesOffset;
		inFile.read(reinterpret_cast<char*>(&translationsOffset), 4);
		translationsOffset += 12;
		inFile.read(reinterpret_cast<char*>(&quaternionsOffset), 4);
		quaternionsOffset += 12;
		inFile.read(reinterpret_cast<char*>(&framesOffset), 4);
		framesOffset += 12;

		const int positionBlockSize = 12;
		vector<Vector3f> translationEntries;

		int numTranslationEntries = (int)(quaternionsOffset - translationsOffset) / positionBlockSize;

		inFile.seekg(translationsOffset, ios_base::beg);

		for (int i = 0; i < numTranslationEntries; ++i)
		{
			Vector3f translationEntry;
			inFile.read(reinterpret_cast<char*>(&translationEntry), 12);
			translationEntries.push_back(translationEntry);
		}

		const int quaternionBlockSize = 16;
		vector<Quaternionf, aligned_allocator<Quaternionf>> quaternionEntries;

		int numQuaternionEntries = (int)(framesOffset - quaternionsOffset) / quaternionBlockSize;

		inFile.seekg(quaternionsOffset, ios_base::beg);

		for (int i = 0; i < numQuaternionEntries; ++i)
		{
			Quaternionf quaternionEntry;
			inFile.read(reinterpret_cast<char*>(&quaternionEntry), 16);
			quaternionEntries.push_back(quaternionEntry);
		}

		map<int, vector<pair<short, short>>> boneMap;

		inFile.seekg(framesOffset, ios_base::beg);

		for (int i = 0; i < numBones; ++i)
		{
			for (int j = 0; j < numFrames; ++j)
			{
				int boneHash; 
				short translationOffset, quaternionOffset;

				inFile.read(reinterpret_cast<char*>(&boneHash), 4);
				inFile.read(reinterpret_cast<char*>(&translationOffset), 2);
				inFile.seekg(2, ios_base::cur);
				inFile.read(reinterpret_cast<char*>(&quaternionOffset), 2);
				inFile.seekg(2, ios_base::cur);

				boneMap[boneHash].push_back(pair<short, short>(translationOffset, quaternionOffset));
			}
		}

		for (auto& i : boneMap)
		{
			if (boneHashes.find(i.first) == boneHashes.end())
			{
				continue;
			}

			AnmBone boneEntry;
			strcpy_s(boneEntry.name, boneHashes.at(i.first));

			for (auto& f : i.second)
			{
				boneEntry.translation.push_back(translationEntries.at(f.first));
				boneEntry.quaternion.push_back(quaternionEntries.at(f.second));
			}

			bones.push_back(boneEntry);
		}
	}

	else if (version == 5)
	{
		int fileSize;
		inFile.read(reinterpret_cast<char*>(&fileSize), 4);
		fileSize += 12;

		inFile.seekg(12, ios_base::cur);

		inFile.read(reinterpret_cast<char*>(&numBones), 4);
		inFile.read(reinterpret_cast<char*>(&numFrames), 4);
		inFile.read(reinterpret_cast<char*>(&frameDelay), 4);

		int translationsOffset, quaternionsOffset, framesOffset, hashesOffset;

		inFile.read(reinterpret_cast<char*>(&hashesOffset), 4);
		inFile.seekg(8, ios_base::cur);
		inFile.read(reinterpret_cast<char*>(&translationsOffset), 4);
		inFile.read(reinterpret_cast<char*>(&quaternionsOffset), 4);
		inFile.read(reinterpret_cast<char*>(&framesOffset), 4);

		translationsOffset += 12;
		quaternionsOffset += 12;
		framesOffset += 12;
		hashesOffset += 12;

		const int translationBlock = 12;
		vector<Vector3f> translationEntries;

		int numTranslationEntries = (int)(quaternionsOffset - translationsOffset) / translationBlock;

		inFile.seekg(translationsOffset, ios_base::beg);

		for (int i = 0; i < numTranslationEntries; ++i)
		{
			Vector3f translationEntry;
			inFile.read(reinterpret_cast<char*>(&translationEntry), 12);
			translationEntries.push_back(translationEntry);
		}

		const int quaternionBlock = 6;
		vector<bitset<48>> quaternionEntries;

		int numQuaternionEntries = (int)(hashesOffset - quaternionsOffset) / quaternionBlock;

		inFile.seekg(quaternionsOffset, ios_base::beg);

		for (int i = 0; i < numQuaternionEntries; ++i)
		{
			bitset<48> quaternionEntry;
			inFile.read(reinterpret_cast<char*>(&quaternionEntry), 6);
			quaternionEntries.push_back(quaternionEntry);
		}

		const int hashBlock = 4;
		vector<int> hashEntries;

		int numHashEntries = (int)(framesOffset - hashesOffset) / hashBlock;

		inFile.seekg(hashesOffset, ios_base::beg);

		for (int i = 0; i < numHashEntries; ++i)
		{
			int hashEntry;
			inFile.read(reinterpret_cast<char*>(&hashEntry), 4);
			hashEntries.push_back(hashEntry);
		}
		
		bones.resize(numBones);

		inFile.seekg(framesOffset, ios_base::beg);

		for (int i = 0; i < numFrames; ++i)
		{
			for (int j = 0; j < numBones; ++j)
			{
				short translationIndex, quaternionIndex;
				inFile.read(reinterpret_cast<char*>(&translationIndex), 2);
				inFile.seekg(2, ios_base::cur);
				inFile.read(reinterpret_cast<char*>(&quaternionIndex), 2);

				bones.at(j).translation.push_back(translationEntries.at(translationIndex));
				
				bitset<48> mask = 0x7FFF;
				unsigned short flag = static_cast<unsigned short>((quaternionEntries.at(quaternionIndex) >> 45).to_ulong());
				unsigned short sx = static_cast<unsigned short>((quaternionEntries.at(quaternionIndex) >> 30 & mask).to_ulong());
				unsigned short sy = static_cast<unsigned short>((quaternionEntries.at(quaternionIndex) >> 15 & mask).to_ulong());
				unsigned short sz = static_cast<unsigned short>((quaternionEntries.at(quaternionIndex) & mask).to_ulong());

				Quaternionf quaterionEntry = uncompressQuaternion(flag, sx, sy, sz);

				bones.at(j).quaternion.push_back(quaterionEntry);
			}
		}

		for (int i = 0; i < numBones; ++i)
		{
			strcpy_s(bones.at(i).name, boneHashes.at(hashEntries.at(i)));
		}
	}
}

Quaternionf AnmImporter::uncompressQuaternion(const unsigned short& flag, const unsigned short& sx, const unsigned short& sy, const unsigned short& sz)
{
	float fx = sqrt(2.0f) * ((int)sx - 16384) / 32768.0f;
	float fy = sqrt(2.0f) * ((int)sy - 16384) / 32768.0f;
	float fz = sqrt(2.0f) * ((int)sz - 16384) / 32768.0f;
	float fw = sqrt(1.0f - fx * fx - fy * fy - fz * fz);

	Quaternionf uq;

	switch (flag)
	{
	case 0:
		uq.x() = fw;
		uq.y() = fx;
		uq.z() = fy;
		uq.w() = fz;
		break;

	case 1:
		uq.x() = fx;
		uq.y() = fw;
		uq.z() = fy;
		uq.w() = fz;
		break;

	case 2:
		uq.x() = -fx;
		uq.y() = -fy;
		uq.z() = -fw;
		uq.w() = -fz;
		break;

	case 3:
		uq.x() = fx;
		uq.y() = fy;
		uq.z() = fz;
		uq.w() = fw;
		break;
	}

	return uq;
}

AnmImporter::~AnmImporter()
{
}
