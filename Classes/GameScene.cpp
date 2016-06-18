#include "MainMenu.h"
#include "GameScene.h"
#include "PauseScene.h"

USING_NS_CC;
using namespace std;

Scene* GameScene::createScene()
{
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

	TTFConfig config("fonts/Marker Felt.ttf", 30);

	// add label "level"
	m_level = 1;
	auto labelLevelTitle = Label::createWithTTF(config, "Level");
	labelLevelTitle->setColor(Color3B::BLACK);
	labelLevelTitle->setPosition(Vec2(size.width * 6 / 7, size.height * 8 / 13));
	addChild(labelLevelTitle);

	// add label level
	auto labelLevel = Label::createWithTTF(config, StringUtils::format("%d", m_level));
	labelLevel->setTag(110);
	labelLevel->setColor(Color3B::BLACK);
	labelLevel->setPosition(Vec2(size.width * 6 / 7, size.height * 7 / 13));
	addChild(labelLevel);

	// add label "score"
	auto labelScoreTitle = Label::createWithTTF(config, "Score");
	labelScoreTitle->setColor(Color3B::BLACK);
	labelScoreTitle->setPosition(Vec2(size.width * 6 / 7, size.height * 6 / 14));
	addChild(labelScoreTitle);

	// add label score
	m_score = 0;
	auto labelScore = Label::createWithTTF(config, "0");
	labelScore->setTag(111);
	labelScore->setColor(Color3B::BLACK);
	labelScore->setPosition(Vec2(size.width * 6 / 7, size.height * 5 / 14));
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
	menuItemBack->setPosition(Vec2(size.width * 6 / 7, size.height * 2 / 9));
	menuItemPause->setPosition(Vec2(size.width * 6 / 7, size.height / 9));
	
	addChild(menu);

	// add keyboard listener
	listener = EventListenerKeyboard::create();

	listener->onKeyPressed = [&](EventKeyboard::KeyCode k, Event * e){
		if (curShape)
		{
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
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// init
	m_lines = 0;
	m_speed = 1.0f;

	// game start
	createNextShape();
	scheduleOnce(schedule_selector(GameScene::pushNextShape), 0.1f);

	return true;
}

// create a random shape
void GameScene::createNextShape()
{
	srand(time(NULL));
	int type = rand() % 7;

	auto s = Shape::create(type);
	s->setPosition(BLOCK_SIZE * Vec2(17, 23));
	addChild(s);

	nextShape = s;
}

// push next shape in the screen
void GameScene::pushNextShape(float t)
{
	curShape = nextShape;
	curShape->setPosition(BLOCK_SIZE * Vec2(6, 24));
	curShape->setRow(6);
	curShape->setCol(24);

	createNextShape();

	if (isGameOver())
	{
		gameOver();
		return;
	}

	// down the current shape
	log("%f", m_speed);
	schedule(schedule_selector(GameScene::dropDown), m_speed);
}

void GameScene::dropDown(float t)
{
	if(checkBorder(1))
		curShape->dropDown();
	else
	{
		unschedule(schedule_selector(GameScene::dropDown));
		setMap();
		int num = calClearNum();
		if (num > 0)
		{
			blockClear();
			addScore(num);
		}
		scheduleOnce(schedule_selector(GameScene::pushNextShape), 0.1f);
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
	if(curShape->canRoate())
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

int GameScene::calClearNum()
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

	return clearlines.size();
}

void GameScene::blockClear()
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

void GameScene::addScore(int num)
{
	// add score
	switch (num)
	{
	case 1:
		m_score += 40 * (m_level + 1);
		break;
	case 2:
		m_score += 100 * (m_level + 1);
		break;
	case 3:
		m_score += 300 * (m_level + 1);
		break;
	case 4:
		m_score += 1200 * (m_level + 1);
		break;
	default:
		break;
	}

	Label* label = (Label*)this->getChildByTag(111);
	label->setString(StringUtils::format("%d", m_score));

	// add level
	m_lines += num;
	if (m_lines / 10 >= m_level && m_lines / 10 < 10)
	{
		m_level++;
	}

	label = (Label*)this->getChildByTag(110);
	label->setString(StringUtils::format("%d", m_level));

	// add speed
	switch (m_level)
	{
	case 1:
		m_speed = 1.0f;
		break;
	case 2:
		m_speed = 0.8f;
		break;
	case 3:
		m_speed = 0.6f;
		break;
	case 4:
		m_speed = 0.4f;
		break;
	case 5:
		m_speed = 0.2f;
		break;
	case 6:
		m_speed = 0.1f;
		break;
	case 7:
		m_speed = 0.05f;
		break;
	case 8:
		m_speed = 0.01f;
		break;
	case 9:
		m_speed = 0.005f;
		break;
	case 10:
		m_speed = 0.0001f;
		break;
	default:
		break;
	}
}

bool GameScene::isGameOver()
{
	int fx = curShape->getRow();
	int fy = curShape->getCol();

	auto bs = curShape->getBlocks();

	for (auto b : bs)
	{
		int x = b->getRow();
		int y = b->getCol();

		if (map[fx + x - 1][fy + y])
			return true;
	}
	return false;
}

void GameScene::gameOver()
{
	auto size = Director::getInstance()->getWinSize();

	//show label "game over"
	auto label = Label::createWithSystemFont("GAME OVER", "Arial", 50);
	label->setPosition(size / 2);
	label->setColor(Color3B::RED);
	addChild(label, 3);

	this->unscheduleAllSelectors();
	_eventDispatcher->removeEventListener(listener);
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