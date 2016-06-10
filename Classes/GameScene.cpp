#include "MainMenu.h"
#include "GameScene.h"
#include "PauseScene.h"

USING_NS_CC;

int g_level;	// game level

Scene* GameScene::createScene(int level)
{
	g_level = level;

	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if(!Layer::init())
		return false;

	auto size = Director::getInstance()->getWinSize();

	// add background picture
	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(Vec2::ZERO);
	this->addChild(bg);

	// add label "score"
	TTFConfig config("fonts/Marker Felt.ttf", 30); 

	auto labelScoreTitle = Label::createWithTTF(config, "Score");
	labelScoreTitle->setColor(Color3B::BLACK);
	labelScoreTitle->setPosition(Vec2(size.width * 5 / 6, size.height * 5 / 8));
	this->addChild(labelScoreTitle);

	// add label score
	m_score = 0;
	auto labelScore = Label::createWithTTF(config, "0");
	labelScore->setTag(110);
	labelScore->setColor(Color3B::BLACK);
	labelScore->setPosition(Vec2(size.width * 5 / 6, size.height / 2));
	this->addChild(labelScore);

	// add button back and pause
	auto menuItemBack = MenuItemFont::create("Back", CC_CALLBACK_1(GameScene::menuCallBack, this));
	menuItemBack->setColor(Color3B::BLACK);
	auto menuItemPause = MenuItemFont::create("Pause", CC_CALLBACK_1(GameScene::menuCallBack, this));
	menuItemPause->setColor(Color3B::BLACK);
	menuItemBack->setTag(1);
	menuItemPause->setTag(2);

	auto menu = Menu::create(menuItemPause, menuItemBack, NULL);
	menu->setPosition(Vec2::ZERO);
	menuItemBack->setPosition(Vec2(size.width * 5 / 6, size.height * 2 / 9));
	menuItemPause->setPosition(Vec2(size.width * 5 / 6, size.height / 9));
	
	this->addChild(menu);

	// add keyboard listener
	auto listener2 = EventListenerKeyboard::create();

	listener2->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){
		switch (k)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			break;
		default:
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

	nextShape = createNextShape();
	pushNextShape();

	return true;
}

// create a random shape
Shape* GameScene::createNextShape()
{
	srand(time(NULL));
	int type = rand() % 7;

	auto s = Shape::create(type);
	s->setPosition(BLOCK_SIZE * Vec2(17, 22));
	this->addChild(s);
	shapes.pushBack(s);

	return s;
}

// push next shape in the screen
void GameScene::pushNextShape()
{
	curShape = nextShape;
	curShape->setPosition(BLOCK_SIZE * Vec2(6, 22));
	curShape->setRow(6);
	curShape->setCol(22);

	// down the current shape
	this->schedule(schedule_selector(GameScene::dropDown), 0.8f);
	downSpeed = 0.8f;

	nextShape = createNextShape();
}

void GameScene::dropDown(float t)
{
	curShape->moveDown();
}

// go back to main menu or pause
void GameScene::menuCallBack(Ref* obj)
{
	auto target = (Node *)obj;
	Scene * scene;

	switch (target->getTag())
	{
	case 1:
		scene = MainMenu::createScene();
		Director::getInstance()->replaceScene(scene);
		break;
	case 2:
		scene = Pause::createScene();
		this->addChild(scene);
		Director::getInstance()->pause();
		break;
	default:
		break;
	}
	
}