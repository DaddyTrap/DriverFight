#ifndef __GameScene_SCENE_H__
#define __GameScene_SCENE_H__

#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class GameScene : public cocos2d::Layer {
public:
  static PhysicsWorld* world;
  static cocos2d::Scene* createScene();
  void setPhysicsWorld(PhysicsWorld * world);
  virtual bool init();

  // a selector callback

  // implement the "static create()" method manually
  CREATE_FUNC(GameScene);
  cocos2d::ProgressTimer* hp1;
  cocos2d::ProgressTimer* spc1;
  cocos2d::ProgressTimer* hp2;
  cocos2d::ProgressTimer* spc2;
  PhysicsWorld* m_world;
};

#endif