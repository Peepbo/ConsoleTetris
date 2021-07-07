#include "Tetris.h"
#include "conio.h"
#include "Key.h"
#include <iostream>

void Tetris::Start()
{
	draw.ShowConsoleCursor(false);
	InitMatrix();
	blockMng.Init();
}

void Tetris::Update()
{
	char key = 'N';
	while (true)
	{
		// input
		if (_kbhit()) key = _getch();
		else key = ' ';

		switch (key)
		{
		case LEFT:
		case RIGHT:
			blockMng.Move(key, matrix, point);
			break;
		case ROTATE_L_SMALL:
		case ROTATE_L_BIG:
		case ROTATE_R_SMALL:
		case ROTATE_R_BIG:
			blockMng.Rotate(key, matrix, point);
			break;
		default:
			blockMng.ChangeMatrix(matrix, point);
			break;
		}

		// only test
		if (key == '+')
			timeValue *= 2;
		else if (key == '-')
			timeValue *= 0.5f;

		//if (key == ROTATE_R || key == ROTATE_L)
		//	blockMng.Rotate(key, matrix, point);
		//else if (key == LEFT || key == RIGHT)
		//	blockMng.Move(key, matrix, point);
		//else if (key - '1' >= 0 && key - '1' <= 6) // only test
		//{
		//	blockMng.blockKind = BlockManager::BlockKind(key - '1');
		//	blockMng.blockIndex = 0;
		//	blockMng.ChangeMatrix(matrix, point);
		//}
		//else
		//	blockMng.ChangeMatrix(matrix, point);

		// output
		draw.Display(matrix, time);
		draw.ClearScreen();

		NextTime();
	}
}

void Tetris::InitMatrix()
{
	for (int i = 0; i < Y_SIZE; i++)
	{
		matrix.emplace_back();

		for (int j = 0; j < X_SIZE; j++)
		{
			matrix[i].emplace_back(0);
		}
	}
}

void Tetris::NextTime()
{
	time += timeValue;

	if (time > 1.0f)
	{
		time = 0.f;
		FallDown();
	}
}

void Tetris::FallDown()
{
	std::string key;
	key += blockMng.blockChar[(int)blockMng.blockKind];
	key += blockMng.blockIndex;

	//가장 밑쪽에 있는 조각 지점을 아래를 확인한다.
	vvi temp = blockMng.blockMap[key];

	std::vector<Point> save;

	int arr[4]{ -1,-1,-1,-1 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (temp[i][j] == 0)continue;

			arr[j] = i;
		}
	}

	//체크해야되는 부분만 추출 후 save에 저장
	for (int i = 0; i < 4; i++)
	{
		if (arr[i] == -1)continue;

		save.push_back({ i,arr[i] });
	}

	bool isEnd = false;

	//충돌부분 아래쪽을 검사하여 해당 블록을 마무리 지을지 계속 밑으로 떨굴지 선택함
	for (const auto it : save)
	{
		if ((point.y + it.y == Y_SIZE - 1))
		{
			isEnd = true;
			break;
		}

		else if (matrix[point.y + it.y + 1][point.x + it.x] == 1)
		{
			isEnd = true;
			break;
		}
	}

	if (isEnd)
	{
		point = { 4,0 };
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if ((point.y + i) == Y_SIZE)break;

			vvi temp = blockMng.blockMap[key];
			for (int j = 0; j < 4; j++)
			{
				if (temp[i][j] == 1)
					matrix[point.y + i][point.x + j] = 0;
			}
		}

		point.y++;
	}
}
