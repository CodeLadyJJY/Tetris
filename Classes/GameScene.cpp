#include "MainMenu.h"
#include "GameScene.h"
#include "PauseScene.h"

USING_NS_CC;
using namespace std;

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
	addChild(bg);

	// add label "score"
	TTFConfig config("fonts/Marker Felt.ttf", 30); 

	auto labelScoreTitle = Label::createWithTTF(config, "Score");
	labelScoreTitle->setColor(Color3B::BLACK);
	labelScoreTitle->setPosition(Vec2(size.width * 5 / 6, size.height * 5 / 8));
	addChild(labelScoreTitle);

	// add label score
	m_score = 0;
	auto labelScore = Label::createWithTTF(config, "0");
	labelScore->setTag(110);
	labelScore->setColor(Color3B::BLACK);
	labelScore->setPosition(Vec2(size.width * 5 / 6, size.height / 2));
	addChild(labelScore);

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
	
	addChild(menu);

	// add keyboard listener
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){
		switch (k)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			moveLeft();
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			moveRight();
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			roateShape();
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			moveDown();
			break;
		default:
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// game start
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
	addChild(s);

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
	downSpeed = 0.2f;
	schedule(schedule_selector(GameScene::dropDown), downSpeed);

	nextShape = createNextShape();
}

void GameScene::dropDown(float t)
{
	if(checkBorder(1))
		curShape->dropDown();
	else
	{
		unschedule(schedule_selector(GameScene::dropDown));
		setMap();
		if (isClear())
			scheduleOnce(schedule_selector(GameScene::blockClear), 0.0f);
		pushNextShape();
	}
}

void GameScene::moveLeft()
{
	if (checkBorder(2))
		curShape->moveLeft();
}

void GameScene::moveRight()
{
	if (checkBorder(3))
		curShape->moveRight();
}

void GameScene::moveDown()
{
	unschedule(schedule_selector(GameScene::dropDown));
	schedule(schedule_selector(GameScene::dropDown), 0.0f);
}

void GameScene::roateShape()
{
	curShape->roateShape();
}

bool GameScene::checkBorder(int type)
{
	auto blocks = curShape->getBlocks();
	int fx = curShape->getRow();
	int fy = curShape->getCol();

	for (auto b : blocks)
	{
		int x = b->getRow();
		int y = b->getCol();

		switch (type)
		{
		case 1:
			if (fy + y - 1 < 0 || map[fx + x - 1][fy + y - 1])
				return false;
			break;
		case 2:
			if (fx + x - 1 < 1 || map[fx + x - 2][fy + y])
				return false;
			break;
		case 3:
			if (fx + x + 1 > BOARD_WIDTH || map[fx + x][fy + y])
				return false;
			break;
		case 4:
			if (fy + y - 1 < 0 || map[fx + x - 1][fy + y - 1])
				return false;
			break;
		default:
			break;
		}
	}

	return true;
}

void GameScene::setMap()
{
	auto shape_blocks = curShape->getBlocks();
	int fx = curShape->getRow();
	int fy = curShape->getCol();

	for (auto b : shape_blocks)
	{
		int x = b->getRow();
		int y = b->getCol();

		map[fx + x - 1][fy + y] = 1;

		auto new_b = Block::create();
		new_b->setBlockType(b->getBlockType());
		new_b->setPosition(BLOCK_SIZE * Vec2(fx + x, fy + y));
		new_b->setRow(fx + x);
		new_b->setCol(fy + y);
		this->addChild(new_b);
		blocks.pushBack(new_b);
	}

	curShape->removeFromParent();
	curShape = nullptr;
}

bool GameScene::isClear()
{
	// calculate clear lines
	for (int j = 0; j < BOARD_HEIGHT; ++j)
	{
		bool flag = true;
		for (int i = 0; i < BOARD_WIDTH; ++i)
		{
			if (!map[i][j])
			{
				flag = false;
				break;
			}
		}

		if (flag)
			clearlines.push_back(j);
	}

	if (clearlines.size() > 0)
		return true;

	return false;
}

void GameScene::blockClear(float t)
{
	for (int t = 0; t < clearlines.size(); ++t)
	{
		// clear blocks
		int j = clearlines.at(t);
		for (int i = 0; i < blocks.size();)
		{
			Block* b = blocks.at(i);
			if (b->getCol() == j)
			{
				b->removeFromParent();
				blocks.erase(i);
			}
			else
			{
				i++;
			}
		}

		// move down
		for (int i = 0; i < blocks.size(); ++i)
		{
			Block* b = blocks.at(i);
			if (b->getCol() > j)
			{
				int x = b->getRow();
				int y = b->getCol();
				b->setPosition(BLOCK_SIZE * Vec2(x, y - 1));
				b->setCol(y - 1);
			}
		}

		// update map
		for (int k = j; k < BOARD_HEIGHT - 1; ++k)
		{
			for (int i = 0; i < BOARD_WIDTH; ++i)
			{
				map[i][k] = map[i][k + 1];
			}
		}
		for (int i = 0; i < BOARD_WIDTH; ++i)
		{
			map[i][BOARD_HEIGHT - 1] = 0;
		}

		// update new clear lines
		for (int k = t + 1; k < clearlines.size(); ++k)
		{
			if (clearlines.at(k) > j)
				clearlines[k]--;
		}
	}

	clearlines.clear();
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