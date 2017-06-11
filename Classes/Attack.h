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
  
  int damage;            // ��ɵ��˺�
  float lifetime;        // ����ʱ��
  float stuntime;        // ��ɵ�Ӳֱʱ��
  float x_speed;         // ����
  float real_x_speed, real_y_speed;
  std::function<float(float)> routeFunc; // ����Ϊdx������ֵΪdy
  Vec2 initial_pos, size;      // ��ʼλ�� (���fighter��ƫ��)

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
