#include "StartMenu.h"
USING_NS_CC;
Scene* StartMenu::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = StartMenu::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool StartMenu::init() {

  if (!Layer::init()) {
    return false;
  }
  Size visibleSize = Director::getInstance()->getVisibleSize();
  auto bg = Sprite::create("StartBackGround.jpg");
  bg->setPosition(visibleSize.width/2, visibleSize.height/2);

  return true;
}

void StartMenu::menuCallback(cocos2d::Ref* pSender) {

}