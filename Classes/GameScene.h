#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Shape.h"
#include <vector>

const int BOARD_WIDTH = 13;
const int BOARD_HEIGHT = 26;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int level);
	virtual bool init();
	CREATE_FUNC(GameScene);
	void menuCallBack(cocos2d::Ref* obj);

private:
	int m_score;
	float downSpeed;
	Shape* nextShape;
	Shape* curShape;
	std::vector<int> clearlines;
	int map[BOARD_WIDTH][BOARD_HEIGHT];
	Vector<Block*> blocks;

	Shape* createNextShape();
	void pushNextShape();

	void dropDown(float t);
	void moveLeft();
	void moveRight();
	void moveDown();

	void roateShape();

	bool checkBorder(int type);

	void setMap();

	bool isClear();
	void blockClear(float t);
};

#endif