#pragma once
#include "defineHeader.h"
#include "Draw.h"
#include "BlockManager.h"
#include "KeyManager.h"

class Tetris
{
public:
	void Start();
	void Update();

	void InputKey();
	
	void InitMatrix();
	void NextTime();
private:
	Draw draw;
	BlockManager blockMng;
	KeyManager keyMng;

	Point point{ 4,0 };

	vvb matrix;

	float checkTime = 0.0f, timeValue = 0.01f;
	int score = 0;
};

