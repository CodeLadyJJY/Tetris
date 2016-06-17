#include "Block.h"

bool Block::init()
{
	if (!Sprite::init())
		return false;

	this->row = 0;
	this->col = 0;

	return true;
}

void Block::setBlockType(int type)
{
	this->type = type;
	if (type >= 0 && type <= 7)
	{
		auto s = Sprite::create(StringUtils::format("%d.png", type+1));
		s->setAnchorPoint(Vec2::ZERO);
		this->addChild(s);
	}
}