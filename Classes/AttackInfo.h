#ifndef __ATTACK_INFO_H__
#define __ATTACK_INFO_H__

#include "cocos2d.h"

USING_NS_CC;

struct AttackInfo {
public:
  int damage;
  float lifetime, stuntime, x_speed;
  Vec2 initial_pos, size;
  std::function<float(float)> routeFunc;
  AttackInfo();
  AttackInfo(const std::string& filepath);
  AttackInfo(int damage, float lifetime, float stuntime, float x_speed, Vec2 initial_pos, std::function<float(float)> routeFunc);
};

#endif // !__ATTACK_INFO_H__
