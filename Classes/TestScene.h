#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "SkillManager.h"
#include "Fighter.h"

USING_NS_CC;

class TestScene : public cocos2d::Layer {
public:
  static cocos2d::Scene* createScene();

  virtual bool init();

  CREATE_FUNC(TestScene);

private:
  Size visibleSize;
  Vec2 origin;
  ProgressTimer* pT1, *pT2;
  SkillManager *skill_manager;
  BattleSystem *battle_system;
  float hp1, hp2;
  void addListener();
  void onKeyPressed(EventKeyboard::KeyCode code, Event *ev);
  void onKeyReleased(EventKeyboard::KeyCode code, Event *ev);
  void BackCallback(Ref* ps);
  void update(float dt) override;
  void GameOver(int state);
  bool end;
};

#endif