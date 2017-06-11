#include "StartMenu.h"  
#include "RotateMenu.h"  
#include "TestScene.h"
#include "AboutScene.h"
USING_NS_CC;
typedef struct SceneList {
  const char *name;
  std::function<cocos2d::Scene*()> callback;
}SceneList;
SceneList sceneList[] = {
  { "Demo1", []() {return StartMenu::createScene(); } }
};
const unsigned int sceneCount = sizeof(sceneList) / sizeof(SceneList);
#define LINE_SPACE 40  

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

// on "init" you need to initialize your instance  
bool StartMenu::init() {
  //////////////////////////////  
  // 1. super init first  
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  /////////////////////////////  
  // 2. add a menu item with "X" image, which is clicked to quit the program  
  //    you may modify it.  

  // add a "close" icon to exit the progress. it's an autorelease object  
  auto closeItem = MenuItemImage::create(
    "CloseNormal.png",
    "CloseSelected.png",
    CC_CALLBACK_1(StartMenu::menuCloseCallback, this));

  closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
    origin.y + closeItem->getContentSize().height / 2));

  //  create menu, it's an autorelease object  
  /*  auto menu = Menu::create(closeItem, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);*/
  auto item1 = MenuItemImage::create("start.jpg", "start.jpg", CC_CALLBACK_1(StartMenu::menuItem1Callback, this));
  auto item2 = MenuItemImage::create("about.jpg", "about.jpg", CC_CALLBACK_1(StartMenu::menuItem2Callback, this));
  auto item3 = MenuItemImage::create("end.jpg", "end.jpg", CC_CALLBACK_1(StartMenu::menuItem3Callback, this));

  RotateMenu *menu = RotateMenu::create();

  menu->addMenuItem(item1);
  menu->addMenuItem(item2);
  menu->addMenuItem(item3);

  menu->setPosition(visibleSize / 2);
  this->addChild(menu, 2);



  /////////////////////////////  
  // 3. add your codes below...  


  // add a label shows "Hello World"  
  // create and initialize a label  

  auto label = LabelTTF::create("Hello World", "Arial", 24);

  // position the label on the center of the screen  
  label->setPosition(Vec2(origin.x + visibleSize.width / 2,
    origin.y + visibleSize.height - label->getContentSize().height));

  // add the label as a child to this layer  
  this->addChild(label, 1);




  return true;
}


void StartMenu::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)  
  MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
  return;
#endif  

  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
  exit(0);
#endif  
}

void StartMenu::menuItem1Callback(cocos2d::Ref* pSender) {
  Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, TestScene::createScene()));
}
void StartMenu::menuItem2Callback(cocos2d::Ref* pSender) {
  Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, AboutScene::createScene()));
}
void StartMenu::menuItem3Callback(cocos2d::Ref* pSender) {
  Director::getInstance()->end();
}


