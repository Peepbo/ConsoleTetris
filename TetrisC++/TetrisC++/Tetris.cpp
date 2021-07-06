#include "Tetris.h"
#include "conio.h"
#include "Key.h"

void Tetris::Start()
{
	draw.ShowConsoleCursor(false);
	InitMatrix();
}

void Tetris::Update()
{
	char key = 'N';
	while (true)
	{
		// input
		if (_kbhit()) key = _getch();

		printf("%c\n", key);

		Test(key);

		// output
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

void Tetris::Test(char ch)
{
	blockMng.ChangeMatrix(matrix, blockMng.GetBlock(BlockManager::BlockKind(ch - '1')), point);
}