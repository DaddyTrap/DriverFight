#ifndef __ GuidanceScene_SCENE_H__
#define __ GuidanceScene_SCENE_H__

#include "cocos2d.h"
class GuidanceScene : public cocos2d::Layer {
public:
  cocos2d::Label* text;
  static cocos2d::Scene* createScene();
  void BackCallback(Ref* ps);
  virtual bool init();
  // a selector callback

  // implement the "static create()" method manually
  CREATE_FUNC(GuidanceScene);
};

#endif