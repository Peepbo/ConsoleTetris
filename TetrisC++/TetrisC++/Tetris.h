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
	void NextTime() { time += timeValue; }
private:
	Draw draw;
	BlockManager blockMng;

	Point point{ 4,0 };

	vvi matrix;

	float time = 0.0f, timeValue = 0.01f;
};

