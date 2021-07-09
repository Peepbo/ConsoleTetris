#include "BlockManager.h"
#include "Key.h"

#include <iostream>

void BlockManager::Init()
{
	BlockMapInit();

	blockIndex = 0;
	blockKind = BlockKind::I;

	BlockUpdate(blockKind, blockIndex);
}

void BlockManager::BlockMapInit()
{
	AddBlockMap(BlockKind::I, { {
						{1,0,0,0},
						{1,0,0,0},
						{1,0,0,0},
						{1,0,0,0} },
						{
						{1,1,1,1},
						{0,0,0,0},
						{0,0,0,0},
						{0,0,0,0} } });

	AddBlockMap(BlockKind::L, { {
						{1,0,0,0},
						{1,0,0,0},
						{1,1,0,0},
						{0,0,0,0} },
						{
						{1,1,1,0},
						{1,0,0,0},
						{0,0,0,0},
						{0,0,0,0} },
						{
						{1,1,0,0},
						{0,1,0,0},
						{0,1,0,0},
						{0,0,0,0} },
						{
						{0,0,1,0},
						{1,1,1,0},
						{0,0,0,0},
						{0,0,0,0} } });

	AddBlockMap(BlockKind::J, { {
						{0,1,0,0},
						{0,1,0,0},
						{1,1,0,0},
						{0,0,0,0} },
						{ 
						{1,0,0,0},
						{1,1,1,0},
						{0,0,0,0},
						{0,0,0,0} },
						{ 
						{1,1,0,0},
						{1,0,0,0},
						{1,0,0,0},
						{0,0,0,0} },
						{ 
						{1,1,1,0},
						{0,0,1,0},
						{0,0,0,0},
						{0,0,0,0} } });

	AddBlockMap(BlockKind::T, { { 
						{0,0,0,0},
						{1,1,1,0},
						{0,1,0,0},
						{0,0,0,0} },
						{ 
						{0,1,0,0},
						{1,1,0,0},
						{0,1,0,0},
						{0,0,0,0} },
						{ 
						{0,1,0,0},
						{1,1,1,0},
						{0,0,0,0},
						{0,0,0,0} },
						{ 
						{0,1,0,0},
						{0,1,1,0},
						{0,1,0,0},
						{0,0,0,0} } });

	AddBlockMap(BlockKind::O, { {
						{1,1,0,0},
						{1,1,0,0},
						{0,0,0,0},
						{0,0,0,0} } });

	AddBlockMap(BlockKind::S, { {
						{0,1,1,0},
						{1,1,0,0},
						{0,0,0,0},
						{0,0,0,0} },
						{ 
						{1,0,0,0},
						{1,1,0,0},
						{0,1,0,0},
						{0,0,0,0} } });

	AddBlockMap(BlockKind::Z, { {
						{1,1,0,0},
						{0,1,1,0},
						{0,0,0,0},
						{0,0,0,0} },
						{ 
						{0,1,0,0},
						{1,1,0,0},
						{1,0,0,0},
						{0,0,0,0} } });
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

void BlockManager::ApplyToMatrix(vvi& matrix, const Point& point)
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

void BlockManager::RemoveBlock(vvi& matrix, const Point& point)
{
	for (const Point& ptr : nowBlock)
	{
		matrix[ptr.y + point.y][ptr.x + point.x] = 0;
	}
}

void BlockManager::Move(const char& arrow, vvi& matrix, Point& point)
{
	switch (arrow)
	{
	case LEFT:
		//��輱 �� ��
		if (point.x == 0)
			return;

		//���ʿ� ����� ���� ��
		for (const Point& ptr : leftCollisionBlock)
		{
			if (matrix[ptr.y + point.y][ptr.x + point.x - 1] == 1)
				return;
		}

		//������ �� ���� �� ������� ��

		//���� �ڸ��� �ִ� ����� �����
		RemoveBlock(matrix, point);

		//��ġ �̵�
		point.x--;

		break;
	case RIGHT:
		//��輱�̰ų� �����ʿ� ����� ���� ��
		for (const Point& ptr : rightCollisionBlock)
		{
			if (ptr.x + point.x == X_SIZE - 1)
				return;

			if (matrix[ptr.y + point.y][ptr.x + point.x + 1] == 1)
				return;
		}

		//������ �� ���� �� ������� ��
			
		//���� �ڸ��� �ִ� ����� �����
		RemoveBlock(matrix, point);

		//��ġ �̵�
		point.x++;

		break;
	}
}

void BlockManager::Rotate(const char& command, vvi& matrix, const Point& point)
{
	/*
	* ȸ���� �ȵǴ� ���
	* 
	* 1. ����� ȭ������� �Ѿ ��
	* 2. ����� �ٸ� ��ϰ� ��ġ�� �κ��� ���� ��
	*/
	std::string before;
	before += blockChar[(int)blockKind];
	before += (char)blockIndex;

	std::string after;
	after += blockChar[(int)blockKind];
	int afterIndex = blockIndex;
	switch (command)
	{
	case ROTATE_L_SMALL:
	case ROTATE_L_BIG:
		if (afterIndex == 0)
			afterIndex = maxIndexMap[blockKind];
		else
			afterIndex--;
		break;

	case ROTATE_R_SMALL:
	case ROTATE_R_BIG:
		if (afterIndex == maxIndexMap[blockKind])
			afterIndex = 0;
		else
			afterIndex++;
		break;
	}

	after += (char)afterIndex;

	const pointVector inspectionPoints = InspectionPointWhenRotating(blockMap[before], blockMap[after]);

	for (const Point& tempPoint : inspectionPoints)
	{
		//y������ �Ѿ�� ���
		if (tempPoint.y + point.y < 0 || tempPoint.y + point.y == Y_SIZE)return;
		//x������ �Ѿ�� ���
		if (tempPoint.x + point.x < 0 || tempPoint.x + point.x == X_SIZE)return;
		//��ġ�� ���
		if (matrix[tempPoint.y + point.y][tempPoint.x + point.x] == 1)return;
	}

	//������� �Դٸ� ������ ��ȯ�� �ȵ� ����, ����� �����ָ� �ȴ�.

	//������ �׷��� ��ϵ��� �����ش�.
	RemoveBlock(matrix, point);

	//nowBlock change
	BlockUpdate(blockKind, afterIndex);

	ApplyToMatrix(matrix, point);
}

std::vector<Point> BlockManager::InspectionPointWhenRotating(const vvi& before, const vvi& after)
{
	std::vector<Point> InspectionPoint;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			//a�� ��ġ�� �ʴ� b�� �κ�
			if (before[y][x] == 0 && after[y][x] == 1)
				InspectionPoint.push_back({ x,y });
		}
	}

	return InspectionPoint;
}

/*
* ����� ����� �ٲ��� ��(�ٸ� ������� �ٲ�ų� ȸ��) �����Ѵ�.
*/
void BlockManager::BlockUpdate(BlockKind kind, int index)
{
	blockKind = kind;
	blockIndex = index;

	//nowBlock
	nowBlock.clear();

	std::string key;
	key += blockChar[(int)blockKind];
	key += (char)blockIndex;

	vvi block = blockMap[key];

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (block[y][x] == 0)continue;

			nowBlock.push_back({ x,y });
		}
	}

	CollisionBlockUpdate();
}

void BlockManager::CollisionBlockUpdate()
{
	//leftCollisionBlock, rightCollisionBlock, downCollisionBlock
	leftCollisionBlock.clear();
	rightCollisionBlock.clear();
	downCollisionBlock.clear();

	//variable
	std::pair<int,int> x_arr[4]{ {4,-1},{4,-1},{4,-1},{4,-1} };// {min,max}
	int y_arr[4]{ -1,-1,-1,-1 };
	/*
	* left, right
	* y�ٿ��� ���� ū x��, ���� ���� x���� �����ϸ� ��
	* down
	* x�ٿ��� ���� ū y���� �����ϸ� ��
	*/

	//first = min, second = max
	for (const Point& block : nowBlock)
	{
		int x = block.x, y = block.y;

		//min x
		x_arr[y].first = std::min(x_arr[y].first, x);
		//max x
		x_arr[y].second = std::max(x_arr[y].second, x);

		//max y
		y_arr[x] = std::max(y_arr[x], y);
	}

	for (int i = 0; i < 4; i++)
	{
		//left
		if (x_arr[i].first != 4)
			leftCollisionBlock.push_back({ x_arr[i].first,i });

		//right
		if (x_arr[i].second != -1)
			rightCollisionBlock.push_back({ x_arr[i].second,i });

		//down
		if (y_arr[i] != -1)
			downCollisionBlock.push_back({ i,y_arr[i] });
	}
}
