#pragma once
#include "defineHeader.h"

class Draw
{
public:
	void ClearScreen();
	void ShowConsoleCursor(bool showFlag);
	void Display(const vvi& matrix);
public:
	float time = 0;
};