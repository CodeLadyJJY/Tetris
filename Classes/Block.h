#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "cocos2d.h"

USING_NS_CC;

const int BLOCK_SIZE = 16;

class Block : public Sprite
{
public:
	enum BlockType
	{
		blank,
		block
	};

	CREATE_FUNC(Block);
	virtual bool init();

	void setRow(int row) { this->row = row; }
	int getRow() const { return row; }
	void setCol(int col) { this->col = col; }
	int getCol() const { return col; }

	void setBlockType(int type);
	BlockType getBlockType() { return type; }

private:
	int row;							
	int col;
	BlockType type;
};

#endif