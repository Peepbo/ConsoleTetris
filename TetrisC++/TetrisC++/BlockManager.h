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

	int blockOlder[7]{ 0,1,2,3,4,5,6 };
	int blockOlderIndex = 0;

	pointVector nowBlock, nextBlock;
	pointVector leftCollisionBlock, rightCollisionBlock, downCollisionBlock;

	BlockKind blockKind;
	int blockIndex;

	COLOR nextBlockColor;

	//function
	void Init();
	void BlockMapInit();
	void Shuffle();

	void AddBlockMap(const BlockKind& kind, const std::vector<pointVector>& blocks);

	void ApplyToMatrix(vvb& matrix, const Point& point);
	void RemoveBlock(vvb& matrix, const Point& point);

	void Move(const char& arrow, vvb& matrix, Point& point);
	void Rotate(const char& command, vvb& matrix, const Point& point);

	pointVector InspectionPointWhenRotating(const pointVector& before, const pointVector& after);

	void BlockUpdate(const BlockKind& kind, const int& index);
	void CollisionBlockUpdate();

	void ChangeBlock();
	void CheckMatrix(vvb& matrix, const Point& point, int& score);
};

