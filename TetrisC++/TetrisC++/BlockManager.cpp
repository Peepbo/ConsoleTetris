#include "BlockManager.h"

vvi BlockManager::GetBlock(BlockKind kind)
{
	switch (kind)
	{
	case BlockKind::I:
		return { {1,1,1,1},
				 {0,0,0,0} ,
				 {0,0,0,0} ,
				 {0,0,0,0} };
		break;
	case BlockKind::L:
		return { {1,0,0,0},
				 {1,0,0,0} ,
				 {1,1,0,0} ,
				 {0,0,0,0} };
		break;
	case BlockKind::J:
		return { {0,1,0,0},
				 {0,1,0,0} ,
				 {1,1,0,0} ,
				 {0,0,0,0} };
		break;
	case BlockKind::T:
		return { {1,1,1,0},
				 {0,1,0,0} ,
				 {0,0,0,0} ,
				 {0,0,0,0} };
		break;
	case BlockKind::O:
		return { {1,1,0,0},
				 {1,1,0,0} ,
				 {0,0,0,0} ,
				 {0,0,0,0} };
		break;
	case BlockKind::S:
		return { {0,1,1,0},
				 {1,1,0,0} ,
				 {0,0,0,0} ,
				 {0,0,0,0} };
		break;
	case BlockKind::Z:
		return { {1,1,0,0},
				 {0,1,1,0} ,
				 {0,0,0,0} ,
				 {0,0,0,0} };
		break;
	default:
		return { {0,0,0,0},
				 {0,0,0,0} ,
				 {0,0,0,0} ,
				 {0,0,0,0} };
		break;
	}
}

void BlockManager::ChangeMatrix(vvi& matrix, vvi kind, Point point)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[point.y + i][point.x + j] = kind[i][j];
		}
	}
}
