#include "BlockManager.h"
#include "Key.h"

#include <iostream>

void BlockManager::Init()
{
	BlockMapInit();

	blockIndex = 0;
	blockKind = BlockKind::I;
	
	nowBlock.push_back({ 0,0 });
	nowBlock.push_back({ 0,1 });
	nowBlock.push_back({ 0,2 });
	nowBlock.push_back({ 0,3 });
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
		if ((point.y + i) == Y_SIZE)break;

		for (int j = 0; j < 4; j++)
		{
			if ((point.x + j) == X_SIZE)break;

			if (kind[i][j] == 1)
				matrix[point.y + i][point.x + j] = 1;
		}
	}
}

void BlockManager::Move(const char& arrow, vvi& matrix, Point& point)
{
	std::string write;

	write += blockChar[(int)blockKind];
	write += (char)blockIndex;

	for (const auto& it : nowBlock)
	{
		matrix[it.y + point.y][it.x + point.x] = 0;
	}

	switch (arrow)
	{
	case LEFT:
		if (point.x > 0)
		{
			bool can = true;

			for (const auto& it : nowBlock)
			{
				if (matrix[it.y + point.y][it.x + point.x - 1] == 1)
				{
					can = false;
					break;
				}
			}

			if(can) 
				point.x--;
		}
		break;
	case RIGHT:
		if (point.x < X_SIZE - 1)
		{
			bool can = true;

			for (const auto& it : nowBlock)
			{
				if (matrix[it.y + point.y][it.x + point.x + 1] == 1)
				{
					can = false;
					break;
				}
			}

			if(can) 
				point.x++;
		}
		break;
	}

	ChangeMatrix(matrix, point);
}

void BlockManager::Rotate(const char& command, vvi& matrix, const Point& point)
{
	std::string write;

	write += blockChar[(int)blockKind];

	switch (command)
	{
	case ROTATE_R:
		if (blockIndex == 0)
			blockIndex = maxIndexMap[blockKind];
		else
			blockIndex--;
		break;
	case ROTATE_L:
		if (blockIndex == maxIndexMap[blockKind])
			blockIndex = 0;
		else
			blockIndex++;
		break;
	}

	write += (char)blockIndex;

	ChangeMatrix(matrix, point);
}
