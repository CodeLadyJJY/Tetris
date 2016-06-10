#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Shape.h"

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
	Vector<Shape*> shapes;

	Shape* createNextShape();
	void pushNextShape();
	Vector<Shape*> getShapes() { return shapes; }

	void dropDown(float t);
};

#endif