#pragma once
#include "defineHeader.h"

class Draw
{
public:
	void ClearScreen();
	void ShowConsoleCursor(const bool& showFlag);
	void Display(const vvb& matrix, const float& time);
};