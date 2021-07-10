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
	std::unordered_map < std::string, pointVector> blockMap;
	std::unordered_map < BlockKind, int> maxIndexMap;

	char blockChar[7]{ 'I','L','J','T','O','S','Z' };

	pointVector nowBlock;
	pointVector leftCollisionBlock, rightCollisionBlock, downCollisionBlock;

	BlockKind blockKind;
	int blockIndex;

	//function
	void Init();
	void BlockMapInit();

	void AddBlockMap(const BlockKind& kind, const std::vector<pointVector>& blocks);

	void ApplyToMatrix(vvi& matrix, const Point& point);
	void RemoveBlock(vvi& matrix, const Point& point);

	void Move(const char& arrow, vvi& matrix, Point& point);
	void Rotate(const char& command, vvi& matrix, const Point& point);

	pointVector InspectionPointWhenRotating(const pointVector& before, const pointVector& after);

	void BlockUpdate(const BlockKind& kind, const int& index);
	void CollisionBlockUpdate();
};

