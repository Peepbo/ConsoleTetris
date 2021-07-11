#pragma once
#include "defineHeader.h"
#include "Draw.h"
#include "BlockManager.h"

class Tetris
{
public:
	void Start();
	void Update();

	void InputKey(char& key);
	
	void InitMatrix();
	void NextTime();
private:
	Draw draw;
	BlockManager blockMng;

	Point point{ 4,0 };

	vvb matrix;

	float time = 0.0f, timeValue = 0.01f;
	int score = 0;
};

