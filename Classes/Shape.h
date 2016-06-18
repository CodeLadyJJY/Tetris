#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "cocos2d.h"
#include "Block.h"

USING_NS_CC;

const int BOARD_WIDTH = 13;
const int BOARD_HEIGHT = 28;

const int shape_array[7][4] = {
	{0x0F00, 0x2222, 0x00F0, 0x4444},
	{0x44C0, 0x8E00, 0x6440, 0x0E20},
	{0x4460, 0x0E80, 0xC440, 0x2E00},
	{0xCC00, 0xCC00, 0xCC00, 0xCC00},
	{0x06C0, 0x8C40, 0x6C00, 0x4620},
	{0x0E40, 0x4C40, 0x4E00, 0x4640},
	{0x0C60, 0x4C80, 0xC600, 0x2640}
};

class Shape : public Node
{
public:
	static Shape* create(int type);
	virtual bool init(int type);

	void setRow(int row) { this->row = row; }
	int getRow() { return row; }
	void setCol(int col) { this->col = col; }
	int getCol() { return col; }

	void setBlocks();
	Vector<Block*> getBlocks() { return this->blocks; }

	void dropDown();
	void moveLeft();
	void moveRight();

	bool canRoate();
	void roateShape();

private:
	int row;
	int col;
	int type;
	int dir;
	Vector<Block*> blocks;
};

#endif