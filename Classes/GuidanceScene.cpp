#include "GuidanceScene.h"
#include "StartMenu.h"
USING_NS_CC;
Scene* GuidanceScene::createScene() {
  // 'scene' is an autorelease object  
  auto scene = Scene::create();

  // 'layer' is an autorelease object  
  auto layer = GuidanceScene::create();

  // add layer as a child to scene  
  scene->addChild(layer);

  // return the scene  
  return scene;
}

// on "init" you need to initialize your instance  
bool GuidanceScene::init() {
  //////////////////////////////  
  // 1. super init first  
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto bg = Sprite::create("menubg.png");
  bg->setScale(visibleSize.width / bg->getContentSize().width);
  bg->setPosition(visibleSize / 2);
  this->addChild(bg, 0);
  auto back = LayerColor::create(Color4B(255, 255, 255, 110));
  addChild(back, 1);
  auto blackItem = MenuItem::create(CC_CALLBACK_1(GuidanceScene::BackCallback, this));
  blackItem->setPosition(visibleSize.width / 2, visibleSize.height / 2);
  blackItem->setContentSize(visibleSize);//设置大小为整个屏幕的大小
  auto blackMenu = Menu::create(blackItem, NULL);
  blackMenu->setPosition(Point::ZERO);
  blackMenu->setAnchorPoint(Point::ZERO);
  this->addChild(blackMenu, 100);
  //创建要显示的文字  
  text = Label::createWithTTF("Guidance\nFor player1, WASD->move, J->punch, K->kick\nFor player2, Arrows->move, 1->punch, 2->kick\n", "fonts/LiheiPro.ttf", 30);
  text->setPosition(Vec2(visibleSize/2));
  text->setColor(Color3B::BLACK);
  this->addChild(text,2);

  //文字滚动，超出范围后从新开始  
  return true;

}

void GuidanceScene::BackCallback(Ref* ps) {
  Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, StartMenu::createScene()));
}
