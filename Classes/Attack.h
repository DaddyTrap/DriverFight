#ifndef __ATTACK_H__
#define __ATTACK_H__

#include "cocos2d.h"
#include "BaseSprite.h"
#include "Fighter.h"

USING_NS_CC;

class Attack : public BaseSprite {
public:
  Attack();
  ~Attack();
  CREATE_FUNC(Attack);
  int damage;            // 造成的伤害
  float lifetime;        // 生命时间
  float stuntime;        // 造成的硬直时间
  Vec2 velocity;         // 移速
  Fighter* getOwner() const;
  void setOwner(Fighter* owner);
  void destroy();
  void update(float dt) override;
private:
  Fighter *owner;
  bool is_destroy;
};

#endif // !__ATTACK_H__
