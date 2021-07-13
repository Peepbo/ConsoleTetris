#pragma once
#include <vector>

struct Point
{
	int x, y;

	bool operator==(const Point& p)const
	{
		return x == p.x && y == p.y;
	}
};

template <>
struct std::hash<Point>
{
	std::size_t operator () (const Point &p)const
	{
		return p.x ^ p.y;
	}
};

enum class COLOR
{
	GRAY = 7,
	DARK_GRAY,
	BLUE,	//I
	GREEN,	//L
	MINT,	//J
	RED,	//T
	PURPLE,	//O
	YELLOW,	//S
	WHITE	//Z
};

struct Block
{
	COLOR color;
	int value;
};

#define vvb std::vector<std::vector<Block>>
#define pointVector std::vector<Point>

#define Y_SIZE 20
#define X_SIZE 10

struct BlockData
{
	pointVector pv;
	Block block;
};