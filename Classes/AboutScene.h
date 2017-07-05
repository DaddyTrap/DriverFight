#ifndef __AboutScene_SCENE_H__
#define __AboutScene_SCENE_H__

#include "cocos2d.h"
class AboutScene : public cocos2d::Layer {
public:
  cocos2d::Label* text;
  static cocos2d::Scene* createScene();
  void BackCallback(Ref* ps);
  virtual bool init();
  void rollText(float);
  // a selector callback

  // implement the "static create()" method manually
  CREATE_FUNC(AboutScene);
};

#endif