#pragma once
#include "defineHeader.h"

class Draw
{
public:
	void SetWindowSize(const int& width,const int& height);

	void ClearScreen();
	void ShowConsoleCursor(const bool& showFlag);
	void Display(const vvb& matrix, const float& time, const int& score, const pointVector& nextBlock, const COLOR& nextBlockColor);
};