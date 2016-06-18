#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Shape.h"
#include <vector>

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	void menuCallBack(cocos2d::Ref* obj);

private:
	int m_score;
	int m_level;
	int m_lines;
	float m_speed;
	Shape* nextShape;
	Shape* curShape;
	std::vector<int> clearlines;
	int map[BOARD_WIDTH][BOARD_HEIGHT];
	Vector<Block*> blocks;
	EventListenerKeyboard* listener;

	void createNextShape();
	void pushNextShape(float t);

	void dropDown(float t);
	void moveLeft();
	void moveRight();
	void moveDown();

	void roateShape();

	bool checkBorder(int type);

	void setMap();

	int calClearNum();
	void blockClear();

	void addScore(int num);

	bool isGameOver();
	void gameOver();
};

#endif