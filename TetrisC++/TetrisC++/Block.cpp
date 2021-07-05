#include <vector>
#include "defineHeader.h"

enum class BlockKind
{
	I,L,J,T,O,S,Z
};

vvi GetBlock(BlockKind kind)
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
	}
	return { {} };
}