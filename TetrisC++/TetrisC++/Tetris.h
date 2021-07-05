#pragma once
#include "defineHeader.h"
#include "Draw.h"

class Tetris
{
public:
	void Start();
	void Update();
	
	void InitMatrix();
private:
	Draw draw;
	vvi matrix;
};

