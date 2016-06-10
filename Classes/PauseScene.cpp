#include "PauseScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* Pause::createScene()
{
	auto scene = Scene::create();
	auto layer = Pause::create();
	scene->addChild(layer);
	return scene;
}

bool Pause::init()
{
	if(!Layer::init())
		return false;

	auto size = Director::getInstance()->getWinSize();

	TTFConfig config("Marker Felt.ttf", 80);

	auto label = Label::createWithTTF(config, "Pause");
	label->setColor(Color3B::BLUE);
	label->setPosition(Vec2(size.width/2, size.height/2));
	this->addChild(label);

	// add continue button
	auto menuItemContinue = MenuItemFont::create("Continue", CC_CALLBACK_1(Pause::menuCallBack, this));
	menuItemContinue->setColor(Color3B::BLUE);

	auto menu = Menu::create(menuItemContinue, NULL);
	menu->setPosition(Vec2::ZERO);
	menuItemContinue->setPosition(Vec2(size.width/2, size.height/4));
	this->addChild(menu);

	return true;
}

void Pause::menuCallBack(Ref* obj)
{
	this->removeFromParent();
	Director::getInstance()->resume();
}