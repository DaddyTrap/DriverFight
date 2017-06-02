#include "AboutScene.h"
USING_NS_CC;
Scene* AboutScene::createScene() {
  // 'scene' is an autorelease object  
  auto scene = Scene::create();

  // 'layer' is an autorelease object  
  auto layer = AboutScene::create();

  // add layer as a child to scene  
  scene->addChild(layer);

  // return the scene  
  return scene;
}

// on "init" you need to initialize your instance  
bool AboutScene::init() {
  //////////////////////////////  
  // 1. super init first  
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();


  return true;
}