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

		if (key == LEFT || key == RIGHT)
			blockMng.Rotate(key, matrix, point);
		else if (key - '1' >= 0 && key - '1' <= 6) // only test
		{
			blockMng.blockKind = BlockManager::BlockKind(key - '1');
			blockMng.blockIndex = 0;
			blockMng.ChangeMatrix(matrix, point);
		}
		else
			blockMng.ChangeMatrix(matrix, point);

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