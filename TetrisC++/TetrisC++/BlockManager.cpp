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
				if (it.x + point.x + 1 == X_SIZE)
				{
					can = false;
					break;
				}

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
	//ȸ���� �ȵǴ� ����ó��
	//1. ��ġ�ų� ȭ������� �Ѿ�� ���� ����
	std::string before;
	before += blockChar[(int)blockKind];
	before += (char)blockIndex;

	std::string after;
	after += blockChar[(int)blockKind];
	int tempIndex = blockIndex;
	switch (command)
	{
	case ROTATE_L_SMALL:
	case ROTATE_L_BIG:
		if (tempIndex == 0)
			tempIndex = maxIndexMap[blockKind];
		else
			tempIndex--;
		break;

	case ROTATE_R_SMALL:
	case ROTATE_R_BIG:
		if (tempIndex == maxIndexMap[blockKind])
			tempIndex = 0;
		else
			tempIndex++;
		break;
	}

	after += (char)tempIndex;

	const std::vector<Point> vp = RotateCheck(blockMap[before], blockMap[after]);

	for (const Point& tempPoint : vp)
	{
		//y������ �Ѿ�� ���
		if (tempPoint.y + point.y < 0 || tempPoint.y + point.y == Y_SIZE)return;
		//x������ �Ѿ�� ���
		if (tempPoint.x + point.x < 0 || tempPoint.x + point.x == X_SIZE)return;
		//��ġ�� ���
		if (matrix[tempPoint.y + point.y][tempPoint.x + point.x] == 1)return;
	}

	//������� �Դٸ� ������ ��ȯ�� �ȵ� ����, ����� �����ָ� �ȴ�.

	//
	for (const auto& it : nowBlock)
	{
		matrix[it.y + point.y][it.x + point.x] = 0;
	}

	switch (command)
	{
	case ROTATE_L_SMALL:
	case ROTATE_L_BIG:
		if (blockIndex == 0)
			blockIndex = maxIndexMap[blockKind];
		else
			blockIndex--;
		break;

	case ROTATE_R_SMALL:
	case ROTATE_R_BIG:
		if (blockIndex == maxIndexMap[blockKind])
			blockIndex = 0;
		else
			blockIndex++;
		break;
	}

	ChangeMatrix(matrix, point);

	//nowBlock change
	nowBlock.clear();
	std::string write;

	write += blockChar[(int)blockKind];
	write += (char)blockIndex;

	const vvi temp = blockMap[write];

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (temp[y][x] == 1)
				nowBlock.push_back({ x,y }); // x,y ����
		}
	}
}

std::vector<Point> BlockManager::RotateCheck(const vvi& before, const vvi& after)
{
	std::vector<Point> output;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			//a�� ��ġ�� �ʴ� b�� �κ�
			if (before[y][x] == 0 && after[y][x] == 1)
				output.push_back({ x,y });
		}
	}

	return output;
}
