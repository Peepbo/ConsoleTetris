#include <Windows.h>
#include <iostream>
#include "Draw.h"

void Draw::SetWindowSize(const int& width, const int& height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);

	space = "    ";
	margin = "  ";
}

void Draw::ClearScreen()
{
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void Draw::ShowConsoleCursor(const bool& showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Draw::NextAndSaveBlockDisplay(const BlockData& nextBlockData,const BlockData& saveBlockData)
{
	char nextBlockMatrix[4][4]{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0} };

	char saveBlockMatrix[4][4]{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0} };

	for (const Point& ptr : nextBlockData.pv)
	{
		nextBlockMatrix[ptr.y][ptr.x] = 1;
	}

	for (const Point& ptr : saveBlockData.pv)
	{
		saveBlockMatrix[ptr.y][ptr.x] = 1;
	}

	std::cout << margin << "NEXT" << '\t' << space << margin << "SAVE" << '\n';

	for (int i = 0; i < 4; i++)
	{
		std::cout << margin;
		for (int j = 0; j < 4; j++)
		{
			if (nextBlockMatrix[i][j] == 0)
				std::cout << "□";
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)nextBlockData.block.color);
				std::cout << "■";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)COLOR::GRAY);
			}
		}

		std::cout << space;

		for (int j = 0; j < 4; j++)
		{
			if (saveBlockMatrix[i][j] == 0)
				std::cout << "□";
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)saveBlockData.block.color);
				std::cout << "■";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)COLOR::GRAY);
			}
		}

		std::cout << '\n';
	}
}

void Draw::Display(const vvb& matrix, const float& time, const int& score,
	const BlockData& nextBlockData, const BlockData& saveBlockData,
	const std::unordered_map <Point, int, std::hash<Point>>& landingMap)
{
	//next save
	NextAndSaveBlockDisplay(nextBlockData, saveBlockData);

	std::cout << margin << "SCORE : " << score << '\n';

	//matrix
	for (int y = 0; y < Y_SIZE; y++)
	{
		std::cout << margin;
		for (int x = 0; x < X_SIZE; x++)
		{
			if (matrix[y][x].value == 1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)matrix[y][x].color);
				std::cout << "■";
				//▣▤▥▨▧▦▩
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)COLOR::GRAY);
			}

			else if (landingMap.find({ x,y }) != landingMap.end())
				std::cout << "■";

			else if (matrix[y][x].value == 0)
				std::cout << "□";
		}

		std::cout << '\n';
	}
}

void Draw::InputName(std::string& nickName)
{
	ClearScreen();
	system("cls");
	printf("\033[%d;%dH", 0, 0);
	while (nickName.empty())
	{
		std::cout << "\t기록할 닉네임\t\n";
		std::cin >> nickName;
	}
}

void Draw::RankDisplay(const std::vector<std::string>& data)
{
	system("cls");
	data.size();

	for (const std::string& rank : data)
	{
		std::cout << rank << '\n';
	}
	std::string write;
	std::cin >> write;
}
