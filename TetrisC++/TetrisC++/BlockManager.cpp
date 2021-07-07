#include "BlockManager.h"
#include "Key.h"

#include <iostream>

void BlockManager::Init()
{
	BlockMapInit();

	blockIndex = 0;
	blockKind = BlockKind::I;
}

void BlockManager::BlockMapInit()
{
	AddBlockMap(BlockKind::I, { { {1,0,0,0},
						  {1,0,0,0},
						  {1,0,0,0},
						  {1,0,0,0} },

						{ {1,1,1,1},
						  {0,0,0,0},
						  {0,0,0,0},
						  {0,0,0,0} } });

	AddBlockMap(BlockKind::L, { { {1,0,0,0},
							  {1,0,0,0},
							  {1,1,0,0},
							  {0,0,0,0} },

							{ {1,1,1,0},
							  {1,0,0,0},
							  {0,0,0,0},
							  {0,0,0,0} },

							{ {1,1,0,0},
							  {0,1,0,0},
							  {0,1,0,0},
							  {0,0,0,0} },

							{ {0,0,1,0},
							  {1,1,1,0},
							  {0,0,0,0},
							  {0,0,0,0} } });

	AddBlockMap(BlockKind::J, { { {0,1,0,0},
							  {0,1,0,0},
							  {1,1,0,0},
							  {0,0,0,0} },

							{ {1,0,0,0},
							  {1,1,1,0},
							  {0,0,0,0},
							  {0,0,0,0} },

							{ {1,1,0,0},
							  {1,0,0,0},
							  {1,0,0,0},
							  {0,0,0,0} },

							{ {1,1,1,0},
							  {0,0,1,0},
							  {0,0,0,0},
							  {0,0,0,0} } });

	AddBlockMap(BlockKind::T, { { {0,0,0,0},
							  {1,1,1,0},
							  {0,1,0,0},
							  {0,0,0,0} },

							{ {0,1,0,0},
							  {1,1,0,0},
							  {0,1,0,0},
							  {0,0,0,0} },

							{ {0,1,0,0},
							  {1,1,1,0},
							  {0,0,0,0},
							  {0,0,0,0} },

							{ {0,1,0,0},
							  {0,1,1,0},
							  {0,1,0,0},
							  {0,0,0,0} } });

	AddBlockMap(BlockKind::O, { { {1,1,0,0},
							  {1,1,0,0},
							  {0,0,0,0},
							  {0,0,0,0} } });

	AddBlockMap(BlockKind::S, { { {0,1,1,0},
							  {1,1,0,0},
							  {0,0,0,0},
							  {0,0,0,0} },

							{ {1,0,0,0},
							  {1,1,0,0},
							  {0,1,0,0},
							  {0,0,0,0} } });

	AddBlockMap(BlockKind::Z, { { {1,1,0,0},
								  {0,1,1,0},
								  {0,0,0,0},
								  {0,0,0,0} },

							{ {0,1,0,0},
							  {1,1,0,0},
							  {1,0,0,0},
							  {0,0,0,0} } });
}

vvi BlockManager::GetBlock()
{
	blockIndex = 0;

	std::string key;
	key += blockChar[(int)blockKind];
	key += (char)blockIndex;

	return blockMap[key];
}

void BlockManager::AddBlockMap(BlockKind kind, std::vector<vvi> blocks)
{
	int blockIndex = 0;
	for (const vvi& block : blocks)
	{
		std::string key;
		key += blockChar[(int)kind];
		key += (char)blockIndex++;

		blockMap[key] = block;
	}

	maxIndexMap[kind] = blockIndex - 1;
}

void BlockManager::ChangeMatrix(vvi& matrix, const Point& point)
{
	std::string key;
	key += blockChar[(int)blockKind];
	key += (char)blockIndex;
	vvi kind = blockMap[key];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[point.y + i][point.x + j] = kind[i][j];
		}
	}
}

void BlockManager::Rotate(const char& arrow, vvi& matrix, const Point& point)
{
	std::string write;

	write += blockChar[(int)blockKind];

	switch (arrow)
	{
	case LEFT:
		if (blockIndex == 0)
			blockIndex = maxIndexMap[blockKind];
		else
			blockIndex--;
		break;
	case RIGHT:
		if (blockIndex == maxIndexMap[blockKind])
			blockIndex = 0;
		else
			blockIndex++;
		break;
	}

	write += (char)blockIndex;

	ChangeMatrix(matrix, point);
}
