#pragma once
#include "defineHeader.h"
#include <unordered_map>

class BlockManager
{
public:
	enum class BlockKind
	{
		I, L, J, T, O, S, Z
	};

	std::unordered_map < std::string, vvi> blockMap;
	std::unordered_map < BlockKind, int> maxIndexMap;

	char blockChar[7]{ 'I','L','J','T','O','S','Z' };

	BlockKind blockKind;
	int blockIndex;

	void Init();
	void BlockMapInit();

	void AddBlockMap(BlockKind kind, std::vector<vvi> blocks);

	vvi GetBlock();
	void ChangeMatrix(vvi& matrix, const Point& point);
	void Rotate(const char& arrow, vvi& matrix, const Point& point);
};

