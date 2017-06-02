#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "SkillManager.h"

USING_NS_CC;

class TestScene : public cocos2d::Layer {
public:
  static cocos2d::Scene* createScene();

  virtual bool init();

  CREATE_FUNC(TestScene);

private:
  Size visibleSize;
  Vec2 origin;

  SkillManager *skill_manager;

  void addListener();
  void onKeyPressed(EventKeyboard::KeyCode code, Event *ev);
  void onKeyReleased(EventKeyboard::KeyCode code, Event *ev);

  void update(float dt) override;
};

#endif