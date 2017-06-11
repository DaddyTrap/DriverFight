#ifndef __StartMenu_SCENE_H__
#define __StartMenu_SCENE_H__

#include "cocos2d.h"

class StartMenu : public cocos2d::Layer
{
public:
  static cocos2d::Scene* createScene();

  virtual bool init();

  // a selector callback
  void menuCallback(cocos2d::Ref* pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(StartMenu);
};

#endif // __StartMenu_SCENE_H__