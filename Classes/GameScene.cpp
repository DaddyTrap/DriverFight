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
  return true;
}