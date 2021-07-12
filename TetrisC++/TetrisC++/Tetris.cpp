#include "Tetris.h"
#include "conio.h"
#include "Key.h"

#include <windows.h>

void Tetris::Start()
{
	draw.SetWindowSize(235, 475);
	draw.ShowConsoleCursor(false);
	InitMatrix();
	blockMng.Init();
}

void Tetris::Update()
{
	char key = 'N';
	while (true)
	{
		InputKey(key);

		draw.Display(matrix, time, score, blockMng.nextBlockData, {});
		draw.ClearScreen();

		NextTime();
	}
}

void Tetris::InputKey(char& key)
{
	if (_kbhit()) 
		key = _getch();
	else 
		key = '*';

	switch (key)
	{
	case LEFT:
	case RIGHT:
		blockMng.Move(key, matrix, point);
		break;
	case DOWN:
		blockMng.FallDown(matrix, point, score);
		break;
	case ROTATE_L_SMALL:
	case ROTATE_L_BIG:
	case ROTATE_R_SMALL:
	case ROTATE_R_BIG:
		blockMng.Rotate(key, matrix, point);
		break;
	case SPEED_UP:
		timeValue *= 2;
		break;
	case SPEED_DOWN:
		timeValue *= 0.5f;
		break;
	case QUICK_DOWN:
		blockMng.QuickDown(matrix, point, score, time);
		break;
	}

	blockMng.ApplyToMatrix(matrix, point);
}

void Tetris::InitMatrix()
{
	matrix.assign(Y_SIZE, {});

	for (int i = 0; i < Y_SIZE; i++)
	{
		for (int j = 0; j < X_SIZE; j++)
		{
			matrix[i].push_back({ COLOR::GRAY, 0 });
		}
	}
}

void Tetris::NextTime()
{
	time += timeValue;

	if (time > 1.0f)
	{
		time = 0.f;

		blockMng.FallDown(matrix, point, score);
	}
}