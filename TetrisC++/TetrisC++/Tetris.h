#pragma once
#include "defineHeader.h"
#include "Draw.h"
#include "BlockManager.h"

class Tetris
{
public:
	void Start();
	void Update();
	
	void InitMatrix();
	
	void Test(char ch);
private:
	Draw draw;
	Point point{ 4,0 };
	BlockManager blockMng;

	vvi matrix;
};

