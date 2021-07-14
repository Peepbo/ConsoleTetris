#include <random>
#include "BlockManager.h"
#include "Key.h"

void BlockManager::Init(const vvb& matrix,Point& point)
{
	BlockMapInit();

	Shuffle();

	blockColor = COLOR(blockOlder[blockOlderIndex] + (int)COLOR::BLUE);
	BlockUpdate(BlockKind(blockOlder[blockOlderIndex]), 0);

	ResetPoint(point);

	LandingPointUpdate(matrix, point);

	saveBlockData = {};
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

void BlockManager::ResetPoint(Point& point)
{
	point = { 4,0 };
}

void BlockManager::LandingPointUpdate(const vvb& matrix, const Point& point)
{
	landingMap.clear();

	int landingY = point.y;

	bool isLand = false, isBlock = false;

	while (!isLand && !isBlock)
	{
		if (landingY + 1 > Y_SIZE - 1)
		{
			for (const Point& ptr : nowBlock)
			{
				landingMap[{ptr.x + point.x, ptr.y + landingY}] = 1;
		 	} 

			return;
		}

		landingY++;
		
		for (const Point& ptr : downCollisionBlock)
		{
			if (ptr.y + landingY > Y_SIZE - 1)return;

			if (matrix[ptr.y + landingY][ptr.x + point.x].value == 1)
			{
				isBlock = true;
			}

			else if (ptr.y + landingY == Y_SIZE - 1)
			{
				isLand = true;
			}
		}
	}

	landingCount = landingY;

	//아래에 블록이 있으면 그 위가 착지 지점 (+1)
	//아래에 아무런 블록이 없으면 그 위치가 착지 지점 (+0)
	for (const Point& ptr : nowBlock)
	{
		landingMap[{ptr.x + point.x, ptr.y + landingY - (isBlock ? 1 : 0)}] = 1;
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

		//착지 지점 업데이트
		LandingPointUpdate(matrix, point);

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

		//착지 지점 업데이트
		LandingPointUpdate(matrix, point);

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

	//착지 지점 업데이트
	LandingPointUpdate(matrix, point);
}

void BlockManager::QuickDown(vvb& matrix, Point& point, int& score, float& time)
{
	time = 0;

	//이미 내 위치가 착지 지점인지 검사
	int sameCount = 0;
	for (const auto& it : landingMap)
	{
		if (matrix[it.first.y][it.first.x].value == 1)
			sameCount++;
	}

	//착지 지점이랑 내 블록 위치랑 같지 않을때만 삭제
	if(sameCount != 4)
		RemoveBlock(matrix, point); //원래 자리에 있던 블록을 지우고

	//착지 위치에 블록을 생성
	for (const auto& it : landingMap)
	{
		matrix[it.first.y][it.first.x] = { blockColor,1 };
	}
	
	//줄 검사 후
	CheckMatrix(matrix, { point.x,landingCount }, score);
	
	//포인터 위치를 초기화 시키고
	ResetPoint(point);
	
	//새로운 블록을 받음
	ChangeBlock();

	//착지 지점 업데이트
	LandingPointUpdate(matrix, point);

	SurvivalCheck(matrix);
}

void BlockManager::FallDown(vvb& matrix, Point& point, int& score)
{
	for (const Point& ptr : downCollisionBlock)
	{
		if (ptr.y + point.y == Y_SIZE - 1)
		{
			CheckMatrix(matrix, point, score);

			ResetPoint(point);
			ChangeBlock();

			//착지 지점 업데이트
			LandingPointUpdate(matrix, point);

			SurvivalCheck(matrix);
			return;
		}

		if (matrix[ptr.y + point.y + 1][ptr.x + point.x].value == 1)
		{
			CheckMatrix(matrix, point, score);

			ResetPoint(point);
			ChangeBlock();

			//착지 지점 업데이트
			LandingPointUpdate(matrix, point);

			SurvivalCheck(matrix);
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
		isSave = true;

		//블록을 저장하고 
		saveBlock = nowBlock;
		saveBlockKind = blockKind;
		saveBlockIndex = blockIndex;
		saveBlockColor = blockColor;

		saveBlockData = { saveBlock ,saveBlockColor };

		//원 위치로 옮긴 뒤
		ResetPoint(point);

		//다음 블록을 받는다.
		ChangeBlock();

		//착지 지점을 다시 받는다.
		LandingPointUpdate(matrix, point);

		SurvivalCheck(matrix);
	}

	else
	{
		if (isSave)return;

		isSave = true;

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
		ResetPoint(point);

		//착지 지점을 다시 받는다.
		LandingPointUpdate(matrix, point);

		SurvivalCheck(matrix);
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

void BlockManager::SurvivalCheck(const vvb& matrix)
{
	//matrix와 겹치면 종료
	for (const Point& ptr : nowBlock)
	{
		//{4,0}
		if (matrix[ptr.y + 0][ptr.x + 4].value == 1)
		{
			*isSurvival = false;
			break;
		}
	}
}

void BlockManager::ChangeBlock()
{
	isSave = false;

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

	//맨 아랫줄 부터 검사, 점점 위로 올라가면서 검사
	for (int y = bottomY; y >= topY; y--)
	{
		if (CheckLine(matrix, y, LineKind::FULL))//해당 줄이 FULL이야?
		{
			//점수를 올리고
			score++;

			//다 지우고
			for (int x = 0; x < X_SIZE; x++)
			{
				matrix[y][x].value = 0;
			}

			//위 좌표 저장
			int upY = y - 1;
			int nowY = y;

			//안비워져 있을 때 반복
			while (CheckLine(matrix, upY, LineKind::NOT_EMPTY))
			{
				//안비워져있다면 위에 블록을 아래로 옮긴다.
				for (int x = 0; x < X_SIZE; x++)
				{
					matrix[nowY][x] = matrix[upY][x];
				}

				upY--;
				nowY--;
			}

			//비워져 있으면?
			//가장 윗줄을 지운다.
			for (int x = 0; x < X_SIZE; x++)
			{
				matrix[nowY][x].value = 0;
			}
		}

		//다시 현재 줄 검사
		if (CheckLine(matrix, y, LineKind::FULL))
			y++; //꽉차있으면 다시 해당 줄 검사 시작
	}
}

bool BlockManager::CheckLine(const vvb& matrix, const int& line, const LineKind& checkKind)
{
	for (int x = 0; x < X_SIZE; x++)
	{
		if (matrix[line][x].value == 1)
		{
			if (checkKind == LineKind::EMPTY)
				return false;
			if (checkKind == LineKind::NOT_EMPTY)
				return true;
		}
		else
		{
			if (checkKind == LineKind::FULL)
				return false;
		}
	}

	switch (checkKind)
	{
	case LineKind::EMPTY:
		return true;
	case LineKind::NOT_EMPTY:
		return false;
	case LineKind::FULL:
		return true;
	default:
		return true;
	}
}