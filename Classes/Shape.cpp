#include "Shape.h"

Shape* Shape::create(int type)
{
	Shape* pRet = new Shape();
	if (pRet && pRet->init(type))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Shape::init(int type)
{
	if (!Node::init())
		return false;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			Block* b = Block::create();
			this->addChild(b);
			blocks.pushBack(b);
		}
	}

	dir = Dir::up;
	this->setBlocks(type);

	return true;
}

void Shape::setBlocks(int type)
{
	int bit = 0x8000;
	int block = shape_array[type][dir];
	int row = 0, col = 0;

	for (; bit > 0; bit = bit >> 1)
	{
		if (bit & block)
		{
			Block* b = blocks.at(row * 4 + col);
			b->setBlockType(type);
			b->setPosition(BLOCK_SIZE * Vec2(col, 3 - row));
			b->setRow(6 + col);
			b->setCol(22 + 3 - row);
		}
		/*else
		{
			Block* b = blocks.at(row * 4 + col);
			b->setBlockType(7);
			b->setPosition(BLOCK_SIZE * Vec2(col, 3 - row));
		}*/
		if (++col == 4)
		{
			col = 0;
			row++;
		}
	}
}

void Shape::moveDown()
{
	this->setPosition(BLOCK_SIZE * Vec2(row, col - 1));
	this->setCol(col - 1);
}