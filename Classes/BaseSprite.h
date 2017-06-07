#ifndef __BASE_SPRITE_H__
#define __BASE_SPRITE_H__

#include "cocos2d.h"
// #include "BattleSystem.h"

class BattleSystem;

USING_NS_CC;

class BaseSprite : public Sprite {
public:
  BaseSprite(const Rect& bounding, BattleSystem *system);
  BaseSprite();
  virtual ~BaseSprite();

  const Rect& getDFBoundingBox() const;
  void setDFBoundingBox(const Rect& boundingBox);

  BattleSystem* getBattleSystem();
  void setBattleSystem(BattleSystem *system);
  void setPosition(const Vec2& pos) override;

  bool detectCollision(const BaseSprite* other);

protected:
  Rect DFBoudingBox;
  BattleSystem *system;
};

#endif // !__BASE_SPRITE_H__
