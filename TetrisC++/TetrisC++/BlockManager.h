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

	//variable
	std::unordered_map < std::string, vvi> blockMap;
	std::unordered_map < BlockKind, int> maxIndexMap;

	char blockChar[7]{ 'I','L','J','T','O','S','Z' };

	pointVector nowBlock;
	pointVector leftCollisionBlock, rightCollisionBlock, downCollisionBlock;

	BlockKind blockKind;
	int blockIndex;

	//function
	void Init();
	void BlockMapInit();

	void AddBlockMap(BlockKind kind, std::vector<vvi> blocks);

	void ApplyToMatrix(vvi& matrix, const Point& point);
	void RemoveBlock(vvi& matrix, const Point& point);

	void Move(const char& arrow, vvi& matrix, Point& point);
	void Rotate(const char& command, vvi& matrix, const Point& point);

	pointVector InspectionPointWhenRotating(const vvi& before, const vvi& after);

	void BlockUpdate(BlockKind kind, int index);
	void CollisionBlockUpdate();
};

