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

	enum class LineKind
	{
		EMPTY, NOT_EMPTY, FULL
	};
public:
	//variable
	std::unordered_map < std::string, pointVector> blockMap;
	std::unordered_map < BlockKind, int> maxIndexMap;

	char blockChar[7]{ 'I','L','J','T','O','S','Z' };

	int blockOlder[7]{ 0,1,2,3,4,5,6 };
	int blockOlderIndex = 0;

	COLOR blockColor, saveBlockColor;

	pointVector nowBlock, nextBlock, saveBlock;
	pointVector leftCollisionBlock, rightCollisionBlock, downCollisionBlock;

	BlockKind blockKind, saveBlockKind;
	int blockIndex, saveBlockIndex;

	BlockData nextBlockData, saveBlockData;

	std::unordered_map <Point, int, std::hash<Point>> landingMap;
	int landingCount = 0;

	bool isSave;
public:
	//function
	void Init(const vvb& matrix, Point& point);
	void BlockMapInit();
	void Shuffle();

	void AddBlockMap(const BlockKind& kind, const std::vector<pointVector>& blocks);

	void ApplyToMatrix(vvb& matrix, const Point& point);
	void RemoveBlock(vvb& matrix, const Point& point);

	void ResetPoint(Point& point);
	void LandingPointUpdate(const vvb& matrix, const Point& point);

	void Move(const char& arrow, vvb& matrix, Point& point);
	void Rotate(const char& command, vvb& matrix, const Point& point);

	void QuickDown(vvb& matrix, Point& point, int& score, float& time);
	void FallDown(vvb& matrix, Point& point, int& score);
	void Save(vvb& matrix, Point& point);

	pointVector InspectionPointWhenRotating(const pointVector& before, const pointVector& after);

	void BlockUpdate(const BlockKind& kind, const int& index);
	void CollisionBlockUpdate();

	void ChangeBlock();
	void CheckMatrix(vvb& matrix, const Point& point, int& score);
	bool CheckLine(const vvb& matrix, const int& line, const LineKind& checkKind);
};

