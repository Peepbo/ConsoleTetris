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

	//�Ʒ��� ����� ������ �� ���� ���� ���� (+1)
	//�Ʒ��� �ƹ��� ����� ������ �� ��ġ�� ���� ���� (+0)
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
		//��輱�̰ų� ���ʿ� ����� ���� ��
		for (const Point& ptr : leftCollisionBlock)
		{
			if (ptr.x + point.x == 0)
				return;

			if (matrix[ptr.y + point.y][ptr.x + point.x - 1].value == 1)
				return;
		}

		//������ �� ���� �� ������� ��

		//���� �ڸ��� �ִ� ����� �����
		RemoveBlock(matrix, point);

		//��ġ �̵�
		point.x--;

		//���� ���� ������Ʈ
		LandingPointUpdate(matrix, point);

		break;
	case RIGHT:
		//��輱�̰ų� �����ʿ� ����� ���� ��
		for (const Point& ptr : rightCollisionBlock)
		{
			if (ptr.x + point.x == X_SIZE - 1)
				return;

			if (matrix[ptr.y + point.y][ptr.x + point.x + 1].value == 1)
				return;
		}

		//������ �� ���� �� ������� ��

		//���� �ڸ��� �ִ� ����� �����
		RemoveBlock(matrix, point);

		//��ġ �̵�
		point.x++;

		//���� ���� ������Ʈ
		LandingPointUpdate(matrix, point);

		break;
	}
}

void BlockManager::Rotate(const char& command, vvb& matrix, const Point& point)
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
		//y������ �Ѿ�� ���
		if (tempPoint.y + point.y < 0 || tempPoint.y + point.y == Y_SIZE)return;
		//x������ �Ѿ�� ���
		if (tempPoint.x + point.x < 0 || tempPoint.x + point.x == X_SIZE)return;
		//��ġ�� ���
		if (matrix[tempPoint.y + point.y][tempPoint.x + point.x].value == 1)return;
	}

	//������� �Դٸ� ������ ��ȯ�� �ȵ� ����, ����� �����ָ� �ȴ�.

	//������ �׷��� ��ϵ��� �����ش�.
	RemoveBlock(matrix, point);

	//nowBlock change
	BlockUpdate(blockKind, afterIndex);

	//���� ���� ������Ʈ
	LandingPointUpdate(matrix, point);
}

void BlockManager::QuickDown(vvb& matrix, Point& point, int& score, float& time)
{
	time = 0;

	//�̹� �� ��ġ�� ���� �������� �˻�
	int sameCount = 0;
	for (const auto& it : landingMap)
	{
		if (matrix[it.first.y][it.first.x].value == 1)
			sameCount++;
	}

	//���� �����̶� �� ��� ��ġ�� ���� �������� ����
	if(sameCount != 4)
		RemoveBlock(matrix, point); //���� �ڸ��� �ִ� ����� �����

	//���� ��ġ�� ����� ����
	for (const auto& it : landingMap)
	{
		matrix[it.first.y][it.first.x] = { blockColor,1 };
	}
	
	//�� �˻� ��
	CheckMatrix(matrix, { point.x,landingCount }, score);
	
	//������ ��ġ�� �ʱ�ȭ ��Ű��
	ResetPoint(point);
	
	//���ο� ����� ����
	ChangeBlock();

	//���� ���� ������Ʈ
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

			//���� ���� ������Ʈ
			LandingPointUpdate(matrix, point);

			SurvivalCheck(matrix);
			return;
		}

		if (matrix[ptr.y + point.y + 1][ptr.x + point.x].value == 1)
		{
			CheckMatrix(matrix, point, score);

			ResetPoint(point);
			ChangeBlock();

			//���� ���� ������Ʈ
			LandingPointUpdate(matrix, point);

			SurvivalCheck(matrix);
			return;
		}
	}

	//������� �Դٸ� �ٴ����� �������� �Ǵ� ���

	//���� �ڸ��� �ִ� ����� �����
	RemoveBlock(matrix, point);

	//������ �̵�
	point.y++;
}

void BlockManager::Save(vvb& matrix, Point& point)
{
	RemoveBlock(matrix, point);

	//����ְų�, �Ⱥ���ְų� �� �� �ϳ�

	if (saveBlock.empty())
	{
		isSave = true;

		//����� �����ϰ� 
		saveBlock = nowBlock;
		saveBlockKind = blockKind;
		saveBlockIndex = blockIndex;
		saveBlockColor = blockColor;

		saveBlockData = { saveBlock ,saveBlockColor };

		//�� ��ġ�� �ű� ��
		ResetPoint(point);

		//���� ����� �޴´�.
		ChangeBlock();

		//���� ������ �ٽ� �޴´�.
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

		//���� ��ϰ� ����� ��� ������ �ٲ��ش�.
		auto temp = nowBlock;
		nowBlock = saveBlock;
		saveBlock = temp;

		saveBlockData = { saveBlock ,saveBlockColor };

		//�� ������� ������Ʈ ���ְ�
		BlockUpdate(blockKind, blockIndex);

		//�� ��ġ�� �ű��.
		ResetPoint(point);

		//���� ������ �ٽ� �޴´�.
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
* ����� ����� �ٲ��� ��(�ٸ� ������� �ٲ�ų� ȸ��) �����Ѵ�.
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

void BlockManager::SurvivalCheck(const vvb& matrix)
{
	//matrix�� ��ġ�� ����
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

	//�츮�� �ƴ°� ��� ���.
	//�ϴ� �����ġ���� ������� �˾ƾ� ��

	//�˻��ؾ��� ������ �����ϱ����� topY�� bottomY�� ����
	//matrix�� ũ�⸸ŭ �˻��ϸ� ������ ��������
	for (const Point& ptr : nowBlock)
	{
		topY = std::min(topY, ptr.y + point.y);
		bottomY = std::max(bottomY, ptr.y + point.y);
	}

	//�� �Ʒ��� ���� �˻�, ���� ���� �ö󰡸鼭 �˻�
	for (int y = bottomY; y >= topY; y--)
	{
		if (CheckLine(matrix, y, LineKind::FULL))//�ش� ���� FULL�̾�?
		{
			//������ �ø���
			score++;

			//�� �����
			for (int x = 0; x < X_SIZE; x++)
			{
				matrix[y][x].value = 0;
			}

			//�� ��ǥ ����
			int upY = y - 1;
			int nowY = y;

			//�Ⱥ���� ���� �� �ݺ�
			while (CheckLine(matrix, upY, LineKind::NOT_EMPTY))
			{
				//�Ⱥ�����ִٸ� ���� ����� �Ʒ��� �ű��.
				for (int x = 0; x < X_SIZE; x++)
				{
					matrix[nowY][x] = matrix[upY][x];
				}

				upY--;
				nowY--;
			}

			//����� ������?
			//���� ������ �����.
			for (int x = 0; x < X_SIZE; x++)
			{
				matrix[nowY][x].value = 0;
			}
		}

		//�ٽ� ���� �� �˻�
		if (CheckLine(matrix, y, LineKind::FULL))
			y++; //���������� �ٽ� �ش� �� �˻� ����
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