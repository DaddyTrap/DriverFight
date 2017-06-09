#ifndef __ATTACK_H__
#define __ATTACK_H__

#include "cocos2d.h"

#include "Predefine.h"

#include "BaseSprite.h"
#include "Fighter.h"

#include "AttackInfo.h"

USING_NS_CC;

class Attack : public BaseSprite {
public:
  Attack();
  ~Attack();
  CREATE_FUNC(Attack);
  
  int damage;            // 造成的伤害
  float lifetime;        // 生命时间
  float stuntime;        // 造成的硬直时间
  float x_speed;         // 移速
  float real_x_speed, real_y_speed;
  std::function<float(float)> routeFunc; // 参数为dx，返回值为dy
  Vec2 initial_pos, size;      // 初始位置 (相对fighter的偏移)

  Fighter* getOwner() const;
  void setOwner(Fighter* owner);
  void initWithAttackInfo(const AttackInfo &info);

  void destroy();
  void hit();

  void update(float dt) override;
private:
  Fighter *owner;
  float dx = 0.0f, dy = 0.0f;
};

#endif // !__ATTACK_H__
