#pragma once
#include "defineHeader.h"

class BlockManager
{
public:
	enum class BlockKind
	{
		I, L, J, T, O, S, Z
	};

	vvi GetBlock(BlockKind kind);
	void ChangeMatrix(vvi& matrix, vvi kind, Point point);
};

