#include "MainMenu.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init()
{
	if(!Layer::init())
		return false;

	auto size = Director::getInstance()->getWinSize();

	TTFConfig config("fonts/Marker Felt.ttf", 70);

	auto GameName = Label::createWithTTF(config, "Teris");
	GameName->setPosition(Vec2(size.width / 2, size.height * 0.85));
	this->addChild(GameName);

	auto GameAuthor = Label::createWithTTF(config, "GAME BY CodeLadyJJY");
	GameAuthor->setPosition(Vec2(size.width / 2, size.height * 0.7));
	GameAuthor->setScale(0.3);
	this->addChild(GameAuthor);

	auto label = Label::createWithSystemFont("Game Level", "Arial", 20);
	label->setPosition(Vec2(size.width / 2, size.height * 0.6));
	this->addChild(label);

	//Ìí¼Ó°´Å¥
	auto menuItemStart1 = MenuItemFont::create("1", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart1->setTag(1);

	auto menuItemStart2 = MenuItemFont::create("2", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart2->setTag(2);

	auto menuItemStart3 = MenuItemFont::create("3", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart3->setTag(3);

	auto menuItemStart4 = MenuItemFont::create("4", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart4->setTag(4);

	auto menuItemStart5 = MenuItemFont::create("5", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart5->setTag(5);

	auto menuItemStart6 = MenuItemFont::create("6", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart6->setTag(6);

	auto menuItemStart7 = MenuItemFont::create("7", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart7->setTag(7);

	auto menuItemStart8 = MenuItemFont::create("8", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart8->setTag(8);

	auto menuItemStart9 = MenuItemFont::create("9", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart9->setTag(9);

	auto menuItemStart10 = MenuItemFont::create("10", CC_CALLBACK_1(MainMenu::menuCallBack, this));
	menuItemStart10->setTag(10);

	auto menu = Menu::create(menuItemStart1, menuItemStart2, menuItemStart3, menuItemStart4, menuItemStart5, 
		menuItemStart6, menuItemStart7, menuItemStart8, menuItemStart9, menuItemStart10, NULL);
	menu->setPosition(Vec2::ZERO);

	menuItemStart1->setPosition(Vec2(size.width * 0.25, size.height * 0.5));
	menuItemStart2->setPosition(Vec2(size.width * 0.75, size.height * 0.5));
	menuItemStart3->setPosition(Vec2(size.width * 0.25, size.height * 0.4));
	menuItemStart4->setPosition(Vec2(size.width * 0.75, size.height * 0.4));
	menuItemStart5->setPosition(Vec2(size.width * 0.25, size.height * 0.3));
	menuItemStart6->setPosition(Vec2(size.width * 0.75, size.height * 0.3));
	menuItemStart7->setPosition(Vec2(size.width * 0.25, size.height * 0.2));
	menuItemStart8->setPosition(Vec2(size.width * 0.75, size.height * 0.2));
	menuItemStart9->setPosition(Vec2(size.width * 0.25, size.height * 0.1));
	menuItemStart10->setPosition(Vec2(size.width * 0.75, size.height * 0.1));

	this->addChild(menu);

	return true;
}

void MainMenu::menuCallBack(Ref* obj)
{
	auto target = (Node *)obj;
	Scene * scene;

	switch (target->getTag())
	{
	case 1:
		scene = GameScene::createScene(1);
		break;
	case 2:
		scene = GameScene::createScene(2);
		break;
	case 3:
		scene = GameScene::createScene(3);
		break;
	case 4:
		scene = GameScene::createScene(4);
		break;
	case 5:
		scene = GameScene::createScene(5);
		break;
	case 6:
		scene = GameScene::createScene(6);
		break;
	case 7:
		scene = GameScene::createScene(7);
		break;
	case 8:
		scene = GameScene::createScene(8);
		break;
	case 9:
		scene = GameScene::createScene(9);
		break;
	case 10:
		scene = GameScene::createScene(10);
		break;
	default:
		break;
	}
	Director::getInstance()->replaceScene(scene);
}