#include "Tetris.h"

void Tetris::Start()
{
	draw.ShowConsoleCursor(false);
	InitMatrix();
}

void Tetris::Update()
{
	while (true)
	{
		draw.Display(matrix);
		draw.ClearScreen();
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
