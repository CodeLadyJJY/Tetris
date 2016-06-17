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
		Block* b = Block::create();
		this->addChild(b);
		blocks.pushBack(b);
	}

	dir = 0;
	this->type = type;
	this->setBlocks();

	return true;
}

void Shape::setBlocks()
{
	int bit = 0x8000;
	int block = shape_array[type][dir];
	int row = 0, col = 0;
	int c = 0;

	for (; bit > 0; bit = bit >> 1)
	{
		if (bit & block)
		{
			Block* b = blocks.at(c++);
			b->setBlockType(type);
			b->setPosition(BLOCK_SIZE * Vec2(col, 3 - row));
			b->setRow(col);
			b->setCol(3 - row);
		}
		if (++col == 4)
		{
			col = 0;
			row++;
		}
	}
}

void Shape::dropDown()
{
	this->setPosition(BLOCK_SIZE * Vec2(row, col - 1));
	this->setCol(col - 1);
}

void Shape::moveLeft()
{
	this->setPosition(BLOCK_SIZE * Vec2(row - 1, col));
	this->setRow(row - 1);
}

void Shape::moveRight()
{
	this->setPosition(BLOCK_SIZE * Vec2(row + 1, col));
	this->setRow(row + 1);
}

void Shape::roateShape()
{
	dir = (dir + 1) % 4;
	int bit = 0x8000;
	int block = shape_array[type][dir];
	int row = 0, col = 0;
	int c = 0;

	for (; bit > 0; bit = bit >> 1)
	{
		if (bit & block)
		{
			Block* b = blocks.at(c++);
			b->setPosition(BLOCK_SIZE * Vec2(col, 3 - row));
			b->setRow(col);
			b->setCol(3 - row);
		}
		if (++col == 4)
		{
			col = 0;
			row++;
		}
	}
}