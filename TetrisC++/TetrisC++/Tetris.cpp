#include "Tetris.h"
#include "conio.h"
#include "Key.h"

#include <vector>

void Tetris::Start()
{
	draw.SetWindowSize(235, 475);
	draw.ShowConsoleCursor(false);
	InitMatrix();
	blockMng.Init(matrix, point);
	blockMng.isSurvival = &isSurvival;
	keyMng.Init();
}

void Tetris::Update()
{
	while (true)
	{
		if (isSurvival)
		{
			keyMng.Update();
			InputKey();

			NextTime();

			blockMng.ApplyToMatrix(matrix, point);

			draw.Display(matrix, checkTime, score, blockMng.nextBlockData, blockMng.saveBlockData, blockMng.landingMap);
			draw.ClearScreen();
		}
		else
		{
			nickName.clear();
			draw.InputName(nickName);
			
			if (rankMng.CheckScore(score))
			{
				rankMng.RecordScore("ScoreData.txt", nickName, score);
				score = 0;
			}

  			draw.ClearScreen();
			
			draw.RankDisplay(rankMng.GetRankData("ScoreData.txt"));

			//restart
			InitMatrix();
			isSurvival = true;
			blockMng.Init(matrix, point);

			draw.ClearScreen();
		}
	}
}

void Tetris::InputKey()
{
	if (keyMng.KeyDown(VK_LEFT))
		blockMng.Move(LEFT, matrix, point);
	if(keyMng.KeyDown(VK_RIGHT))
		blockMng.Move(RIGHT, matrix, point);
	if(keyMng.KeyDown(VK_DOWN))
		blockMng.FallDown(matrix, point, score);

	if(keyMng.KeyDown(VK_UP) || keyMng.KeyDown(0x58))
		blockMng.Rotate(ROTATE_R, matrix, point);
	else if(keyMng.KeyDown(0x5A))
		blockMng.Rotate(ROTATE_L, matrix, point);

	if(keyMng.KeyDown(VK_ADD))
		timeValue *= 2;
	else if(keyMng.KeyDown(VK_SUBTRACT))
		timeValue *= 0.5f;

	if(keyMng.KeyDown(VK_SPACE))
		blockMng.QuickDown(matrix, point, score, checkTime);

	if (keyMng.KeyDown(VK_SHIFT))
		blockMng.Save(matrix, point);
}

void Tetris::InitMatrix()
{
	if (matrix.empty())
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

	else
	{
		for (int y = 0; y < Y_SIZE; y++)
		{
			for (int x = 0; x < X_SIZE; x++)
			{
				matrix[y][x] = { COLOR::GRAY,0 };
			}
		}
	}
}

void Tetris::NextTime()
{
	checkTime += timeValue;

	if (checkTime > 1.0f)
	{
		checkTime = 0.f;

		blockMng.FallDown(matrix, point, score);
	}
}