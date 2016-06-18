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

	TTFConfig config("fonts/Marker Felt.ttf", 100);

	auto GameName = Label::createWithTTF(config, "Teris");
	GameName->setPosition(Vec2(size.width / 2, size.height * 0.6));
	this->addChild(GameName);

	auto GameAuthor = Label::createWithTTF(config, "GAME BY CodeLadyJJY");
	GameAuthor->setPosition(Vec2(size.width / 2, size.height * 0.5));
	GameAuthor->setScale(0.2);
	this->addChild(GameAuthor);

	//Ìí¼Ó°´Å¥
	auto menuItemStart = MenuItemFont::create("Start", CC_CALLBACK_1(MainMenu::menuCallBack, this));

	auto menu = Menu::create(menuItemStart, NULL);
	menu->setPosition(Vec2::ZERO);

	menuItemStart->setPosition(Vec2(size.width / 2, size.height * 0.3));

	this->addChild(menu);

	return true;
}

void MainMenu::menuCallBack(Ref* obj)
{
	Scene * scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}