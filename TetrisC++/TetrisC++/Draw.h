#pragma once
#include "defineHeader.h"

class Draw
{
public:
	void ClearScreen();
	void ShowConsoleCursor(const bool& showFlag);
	void Display(const vvi& matrix, const float& time);
};