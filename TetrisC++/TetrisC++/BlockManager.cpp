#include <random>
#include "BlockManager.h"
#include "Key.h"

void BlockManager::Init()
{
	BlockMapInit();

	Shuffle();

	blockColor = COLOR(blockOlder[blockOlderIndex] + (int)COLOR::BLUE);
	BlockUpdate(BlockKind(blockOlder[blockOlderIndex]), 0);
}

void BlockManager::BlockMapInit()
{
	/*
	* I Block
	* {1,0,0,0}	{1,1,1,1}
	* {1,0,0,0}	{0,0,0,0}
	* {1,0,0,0}	{0,0,0,0}
	* {1,0,0,0} {0,0,0,0}
	*/

	AddBlockMap(BlockKind::I, { {
		{0,0},{0,1},{0,2},{0,3}},
		{{0,0},{1,0},{2,0},{3,0}} });

	/*
	* L Block
	* {1,0,0,0} {1,1,1,0} {1,1,0,0} {0,0,1,0}
	* {1,0,0,0} {1,0,0,0} {0,1,0,0} {1,1,1,0}
	* {1,1,0,0} {0,0,0,0} {0,1,0,0} {0,0,0,0}
	* {0,0,0,0} {0,0,0,0} {0,0,0,0} {0,0,0,0}
	*/

	AddBlockMap(BlockKind::L, { {
		{{0,0},{0,1},{0,2},{1,2}},
		{{0,0},{1,0},{2,0},{0,1}},
		{{0,0},{1,0},{1,1},{1,2}},
		{{2,0},{0,1},{1,1},{2,1}}} });

	/*
	* J Block
	* {0,1,0,0} {1,0,0,0} {1,1,0,0} {1,1,1,0}
	* {0,1,0,0} {1,1,1,0} {1,0,0,0} {0,0,1,0}
	* {1,1,0,0} {0,0,0,0} {1,0,0,0} {0,0,0,0}
	* {0,0,0,0} {0,0,0,0} {0,0,0,0} {0,0,0,0}
	*/

	AddBlockMap(BlockKind::J, { {
		{{1,0},{1,1},{0,2},{1,2}},
		{{0,0},{0,1},{1,1},{2,1}},
		{{0,0},{1,0},{0,1},{0,2}},
		{{0,0},{1,0},{2,0},{2,1}}} });

	/*
	* T Block
	* {0,0,0,0} {0,1,0,0} {0,1,0,0} {0,1,0,0}
	* {1,1,1,0} {1,1,0,0} {1,1,1,0} {0,1,1,0}
	* {0,1,0,0} {0,1,0,0} {0,0,0,0} {0,1,0,0}
	* {0,0,0,0} {0,0,0,0} {0,0,0,0} {0,0,0,0}
	*/

	AddBlockMap(BlockKind::T, { {
		{{0,1},{1,1},{2,1},{1,2}},
		{{1,0},{0,1},{1,1},{1,2}},
		{{1,0},{0,1},{1,1},{2,1}},
		{{1,0},{1,1},{2,1},{1,2}}} });

	/*
	* O Block
	* {1,1,0,0}
	* {1,1,0,0}
	* {0,0,0,0}
	* {0,0,0,0}
	*/

	AddBlockMap(BlockKind::O, { {
		{{0,0},{1,0},{0,1},{1,1}}} });
	
	/*
	* S Block
	* {0,1,1,0} {1,0,0,0}
	* {1,1,0,0} {1,1,0,0}
	* {0,0,0,0} {0,1,0,0}
	* {0,0,0,0} {0,0,0,0}
	*/

	AddBlockMap(BlockKind::S, { {
		{{1,0},{2,0},{0,1},{1,1}},
		{{0,0},{0,1},{1,1},{1,2}}} });

	/*
	* Z block
	* {1,1,0,0} {0,1,0,0}
	* {0,1,1,0} {1,1,0,0}
	* {0,0,0,0} {1,0,0,0}
	* {0,0,0,0} {0,0,0,0}
	*/

	AddBlockMap(BlockKind::Z, { {
		{{0,0},{1,0},{1,1},{2,1}},
		{{1,0},{0,1},{1,1},{0,2}}} });
}

void BlockManager::Shuffle()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 6);

	for (int i = 0; i < 7; i++)
	{
		std::swap(blockOlder[i], blockOlder[dis(gen)]);
	}
}

void BlockManager::AddBlockMap(const BlockKind& kind, const std::vector<pointVector>& blocks)
{
	int blockIndex = 0;
	std::string key;

	for (const pointVector& block : blocks)
	{
		key.clear();

		key += blockChar[(int)kind];
		key += (char)blockIndex++;

		blockMap[key] = block;
	}

	maxIndexMap[kind] = blockIndex - 1;
}

void BlockManager::ApplyToMatrix(vvb& matrix, const Point& point)
{
	std::string key;
	key += blockChar[(int)blockKind];
	key += (char)blockIndex;
	pointVector blocks = blockMap[key];

	for (const Point& block : blocks)
	{
		matrix[block.y + point.y][block.x + point.x] = { blockColor,1 };
	}
}

void BlockManager::RemoveBlock(vvb& matrix, const Point& point)
{
	for (const Point& ptr : nowBlock)
	{
		matrix[ptr.y + point.y][ptr.x + point.x].value = 0;
	}
}

void BlockManager::Move(const char& arrow, vvb& matrix, Point& point)
{
	switch (arrow)
	{
	case LEFT:
		//경계선이거나 왼쪽에 블록이 있을 때
		for (const Point& ptr : leftCollisionBlock)
		{
			if (ptr.x + point.x == 0)
				return;

			if (matrix[ptr.y + point.y][ptr.x + point.x - 1].value == 1)
				return;
		}

		//움직일 수 있을 때 여기까지 옴

		//원래 자리에 있던 블록을 지우고
		RemoveBlock(matrix, point);

		//위치 이동
		point.x--;

		break;
	case RIGHT:
		//경계선이거나 오른쪽에 블록이 있을 때
		for (const Point& ptr : rightCollisionBlock)
		{
			if (ptr.x + point.x == X_SIZE - 1)
				return;

			if (matrix[ptr.y + point.y][ptr.x + point.x + 1].value == 1)
				return;
		}

		//움직일 수 있을 때 여기까지 옴

		//원래 자리에 있던 블록을 지우고
		RemoveBlock(matrix, point);

		//위치 이동
		point.x++;

		break;
	}
}

void BlockManager::Rotate(const char& command, vvb& matrix, const Point& point)
{
	/*
	* 회전이 안되는 경우
	*
	* 1. 블록이 화면밖으로 넘어갈 때
	* 2. 블록이 다른 블록과 겹치는 부분이 생길 때
	*/
	std::string before;
	before += blockChar[(int)blockKind];
	before += (char)blockIndex;

	std::string after;
	after += blockChar[(int)blockKind];
	int afterIndex = blockIndex;
	switch (command)
	{
	case ROTATE_L:
		if (afterIndex == 0)
			afterIndex = maxIndexMap[blockKind];
		else
			afterIndex--;
		break;

	case ROTATE_R:
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
		//y범위를 넘어가는 경우
		if (tempPoint.y + point.y < 0 || tempPoint.y + point.y == Y_SIZE)return;
		//x범위를 넘어가는 경우
		if (tempPoint.x + point.x < 0 || tempPoint.x + point.x == X_SIZE)return;
		//겹치는 경우
		if (matrix[tempPoint.y + point.y][tempPoint.x + point.x].value == 1)return;
	}

	//여기까지 왔다면 위에서 반환이 안된 상태, 블록을 돌려주면 된다.

	//이전에 그려진 블록들을 지워준다.
	RemoveBlock(matrix, point);

	//nowBlock change
	BlockUpdate(blockKind, afterIndex);
}

void BlockManager::QuickDown(vvb& matrix, Point& point, int& score, float& time)
{
	time = 0;

	Point tempPoint = point;
	bool isDown = true;

	while (isDown)
	{
		for (const Point& ptr : downCollisionBlock)
		{
			//맨 아래에 위치하면
			if (ptr.y + tempPoint.y == Y_SIZE - 1)
			{
				isDown = false;
				break;
			}

			//바로 아래가 블록이면
			if (matrix[ptr.y + tempPoint.y + 1][ptr.x + tempPoint.x].value == 1)
			{
				isDown = false;
				break;
			}
		}

		if (isDown)
			tempPoint.y++;
	}

	//원래 자리에 있던 블록을 지우고
	RemoveBlock(matrix, point);

	//최종 위치로 옮긴 뒤

	for (const Point& block : nowBlock)
	{
		matrix[block.y + tempPoint.y][block.x + tempPoint.x] = 
		{ blockColor,1 };
	}
	
	//줄 검사 후
	CheckMatrix(matrix, tempPoint, score);
	
	//포인터 위치를 초기화 시키고
	point = { 4,0 };
	
	//새로운 블록을 받음
	ChangeBlock();
}

void BlockManager::FallDown(vvb& matrix, Point& point, int& score)
{
	for (const Point& ptr : downCollisionBlock)
	{
		if (ptr.y + point.y == Y_SIZE - 1)
		{
			CheckMatrix(matrix, point, score);

			point = { 4,0 };
			ChangeBlock();
			return;
		}

		if (matrix[ptr.y + point.y + 1][ptr.x + point.x].value == 1)
		{
			CheckMatrix(matrix, point, score);

			point = { 4,0 };
			ChangeBlock();
			return;
		}
	}

	//여기까지 왔다면 바닥으로 내려가도 되는 경우

	//원래 자리에 있던 블록을 지우고
	RemoveBlock(matrix, point);

	//밑으로 이동
	point.y++;
}

void BlockManager::Save(vvb& matrix, Point& point)
{
	RemoveBlock(matrix, point);

	//비어있거나, 안비어있거나 둘 중 하나

	if (saveBlock.empty())
	{
		//블록을 저장하고 
		saveBlock = nowBlock;
		saveBlockKind = blockKind;
		saveBlockIndex = blockIndex;
		saveBlockColor = COLOR(blockOlder[blockOlderIndex] + (int)COLOR::BLUE);

		saveBlockData = { saveBlock ,saveBlockColor };

		//원 위치로 옮긴 뒤
		point = { 4,0 };

		//다음 블록을 받는다.
		ChangeBlock();
	}

	else
	{
		//swap()
		std::swap(saveBlockKind, blockKind);
		std::swap(saveBlockIndex, blockIndex);
		std::swap(saveBlockColor, blockColor);

		//현재 블록과 저장된 블록 정보를 바꿔준다.
		auto temp = nowBlock;
		nowBlock = saveBlock;
		saveBlock = temp;

		saveBlockData = { saveBlock ,saveBlockColor };

		//새 블록으로 업데이트 해주고
		BlockUpdate(blockKind, blockIndex);

		//원 위치로 옮긴다.
		point = { 4,0 };
	}
}

pointVector BlockManager::InspectionPointWhenRotating(const pointVector& before, const pointVector& after)
{
	pointVector InspectionPoint;

	int tempMatrix[4][4]{ 
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0} };

	for (int i = 0; i < 4; i++)
	{
		tempMatrix[before[i].y][before[i].x]--;
		tempMatrix[after[i].y][after[i].x]++;
	}

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (tempMatrix[y][x] != 1)continue;

			InspectionPoint.push_back({ x,y });
		}
	}

	return InspectionPoint;
}

/*
* 블록의 모양이 바꼈을 때(다른 블록으로 바뀌거나 회전) 실행한다.
*/
void BlockManager::BlockUpdate(const BlockKind& kind, const int& index)
{
	blockKind = kind;
	blockIndex = index;

	std::string key;
	pointVector block;

	//nowBlock
	nowBlock.clear();
	key += blockChar[(int)blockKind];
	key += (char)blockIndex;

	block = blockMap[key];

	for (const Point& ptr : block)
	{
		nowBlock.emplace_back(ptr);
	}

	//nextBlock
	nextBlock.clear();
	key.clear();
	block.clear();

	int tempIndex = blockOlderIndex + 1;
	if (tempIndex > 6)
		tempIndex = 0;

	key += blockChar[blockOlder[tempIndex]];
	key += (char)0;

	block = blockMap[key];

	for (const Point& ptr : block)
	{
		nextBlock.emplace_back(ptr);
	}

	nextBlockData = { nextBlock, COLOR(blockOlder[tempIndex] + (int)COLOR::BLUE) };

	CollisionBlockUpdate();
}

void BlockManager::CollisionBlockUpdate()
{
	//leftCollisionBlock, rightCollisionBlock, downCollisionBlock
	leftCollisionBlock.clear();
	rightCollisionBlock.clear();
	downCollisionBlock.clear();

	//variable
	std::pair<int, int> x_arr[4]{ {4,-1},{4,-1},{4,-1},{4,-1} };// {min,max}
	int y_arr[4]{ -1,-1,-1,-1 };
	/*
	* left, right
	* y줄에서 가장 큰 x값, 가장 작은 x값을 저장하면 됨
	* down
	* x줄에서 가장 큰 y값을 저장하면 됨
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

void BlockManager::ChangeBlock()
{
	if (blockOlderIndex != 6)
		blockOlderIndex++;
	else
		blockOlderIndex = 0;

	BlockUpdate(BlockKind(blockOlder[blockOlderIndex]), 0);

	blockColor = COLOR(blockOlder[blockOlderIndex] + (int)COLOR::BLUE);
}

void BlockManager::CheckMatrix(vvb& matrix, const Point& point, int& score)
{
	int topY = Y_SIZE, bottomY = -1;

	//우리가 아는건 블록 모양.
	//일단 어느위치에서 멈췄는지 알아야 함

	//검사해야할 범위를 제한하기위해 topY와 bottomY를 구함
	//matrix의 크기만큼 검사하면 성능이 좋지않음
	for (const Point& ptr : nowBlock)
	{
		topY = std::min(topY, ptr.y + point.y);
		bottomY = std::max(bottomY, ptr.y + point.y);
	}

	for (int y = topY; y <= bottomY; y++)
	{
		bool isClear = true;
		for (int x = 0; x < X_SIZE; x++)
		{
			if (matrix[y][x].value == 0)
			{
				isClear = false;
				break;
			}
		}

		if (isClear)
		{
			score++;

			//위 matrix에 블록이 있으면 한칸씩 다 내려와야 한다.
			for (int x = 0; x < X_SIZE; x++)
			{
				matrix[y][x].value = 0;
			}

			//일단 위가 비었는지 안비었는지 확인해야 한다.
			bool isEmpty = false;
			int tempY = y, targetY = y - 1; //지워진 줄 바로 위

			while (!isEmpty)
			{
				for (; targetY >= 0; targetY--,tempY--)
				{
					isEmpty = true;

					for (int x = 0; x < X_SIZE; x++)
					{
						if (matrix[targetY][x].value == 1)
						{
							isEmpty = false;
							break;
						}
					}

					if (isEmpty)
					{
						//계속 위에있는 블록들을 아래로 복제했기 때문에 위가 비워졌을 경우
						//맨위의 블록과 그 아래 줄 블록이 똑같은 모양일것이다.(맨위를 지워야함)
						//고로 맨 위를 지워서 한칸씩 내려간 효과를 준다.

						for (int x = 0; x < X_SIZE; x++)
						{
							matrix[tempY][x].value = 0;
						}

						break;
					}

					for (int x = 0; x < X_SIZE; x++)
					{
						matrix[tempY][x] = matrix[targetY][x];
					}
				}
			}
		}
	}
}
