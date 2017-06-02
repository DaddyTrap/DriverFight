#include "GameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

void GameScene::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }
Scene* GameScene::createScene() {
  srand((unsigned)time(NULL));
  auto scene = Scene::createWithPhysics();

  scene->getPhysicsWorld()->setAutoStep(false);


  scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
  auto layer = 
    GameScene::create();
  layer->setPhysicsWorld(scene->getPhysicsWorld());
  scene->addChild(layer);
  return scene;
}

// on "init" you need to initialize your instance  
bool GameScene::init() {
  //////////////////////////////  
  // 1. super init first  
  if (!Layer::init()) {
    return false;
  }

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
  Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

  hp1 = ProgressTimer::create(sp);
  hp1->setScaleX(90);
  hp1->setAnchorPoint(Vec2(0, 0));
  hp1->setType(ProgressTimerType::BAR);
  hp1->setBarChangeRate(Point(1, 0));
  hp1->setMidpoint(Point(0, 1));
  hp1->setPercentage(100);
  hp1->setPosition(Vec2(origin.x + 14 * hp1->getContentSize().width, origin.y + visibleSize.height - 2 * hp1->getContentSize().height));
  addChild(hp1, 2);
  sp0->setAnchorPoint(Vec2(0, 0));
  sp0->setPosition(Vec2(origin.x + hp1->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
  addChild(sp0, 1);

  Sprite* sp1 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
  Sprite* sp2 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
  spc1 = ProgressTimer::create(sp2);
  spc1->setColor(cocos2d::Color3B::BLUE);
  spc1->setScaleX(90);
  spc1->setAnchorPoint(Vec2(0, 0));
  spc1->setType(ProgressTimerType::BAR);
  spc1->setBarChangeRate(Point(1, 0));
  spc1->setMidpoint(Point(0, 1));
  spc1->setPercentage(100);
  spc1->setPosition(Vec2(origin.x + 14 * spc1->getContentSize().width, origin.y + visibleSize.height - 2 * spc1->getContentSize().height - sp0->getContentSize().height));
  addChild(spc1, 2);
  sp1->setAnchorPoint(Vec2(0, 0));
  sp1->setPosition(Vec2(origin.x + spc1->getContentSize().width, origin.y + visibleSize.height - sp1->getContentSize().height - sp0->getContentSize().height));
  addChild(sp1, 1);

  Sprite* ship;
  ship = Sprite::create("ship.png");
  ship->setScale(visibleSize.width / ship->getContentSize().width * 0.97, 1.2f);
  ship->setPosition(visibleSize.width / 2, 110);
  auto shipbody = PhysicsBody::createBox(ship->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
  shipbody->setCategoryBitmask(0xFFFFFFFF);
  shipbody->setCollisionBitmask(0xFFFFFFFF);
  shipbody->setContactTestBitmask(0xFFFFFFFF);
  shipbody->setDynamic(false);
  ship->setPhysicsBody(shipbody);
  this->addChild(ship, 1);
  return true;
}