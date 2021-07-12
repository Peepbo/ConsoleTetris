#pragma once
#include "defineHeader.h"
#include <string>

class Draw
{
public:
	void SetWindowSize(const int& width,const int& height);

	void ClearScreen();
	void ShowConsoleCursor(const bool& showFlag);

	void NextAndSaveBlockDisplay(const BlockData& nextBlockData, const BlockData& saveBlockData);
	void Display(const vvb& matrix, const float& time, const int& score, const BlockData& nextBlockData, const BlockData& saveBlockData);
private:
	std::string space, margin;
};