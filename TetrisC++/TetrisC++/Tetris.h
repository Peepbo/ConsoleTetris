#pragma once
#include "defineHeader.h"
#include "RankManager.h"
#include "BlockManager.h"
#include "KeyManager.h"
#include "Draw.h"

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
	RankManager rankMng;

	Point point{ 4,0 };

	vvb matrix;

	float checkTime = 0.0f, timeValue = 0.01f;
	int score = 0;
	bool isSurvival = true;

	std::string nickName;
};

