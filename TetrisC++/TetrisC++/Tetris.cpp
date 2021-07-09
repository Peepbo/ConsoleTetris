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
			blockMng.ApplyToMatrix(matrix, point);
			break;
		}

		// only test
		if (key == '+')
			timeValue *= 2;
		else if (key == '-')
			timeValue *= 0.5f;

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
	for (const Point& ptr : blockMng.downCollisionBlock)
	{
		if (ptr.y + point.y == Y_SIZE - 1)
		{
			point = { 4,0 };
			return;
		}

		if (matrix[ptr.y + point.y + 1][ptr.x + point.x] == 1)
		{
			point = { 4,0 };
			return;
		}
	}

	//여기까지 왔다면 바닥으로 내려가도 되는 경우
	for (const Point& ptr : blockMng.nowBlock)
	{
		matrix[ptr.y + point.y][ptr.x + point.x] = 0;
	}

	point.y++;
}
