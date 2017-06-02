#ifndef __FIGHTER_H__
#define __FIGHTER_H__

#include "cocos2d.h"
#include "BattleSystem.h"
#include "Skill.h"
#include <list>

USING_NS_CC;

class Fighter : public Node {
public:
  Fighter();
  Fighter(int max_hp, int max_spc, std::list<Skill*> skills = std::list<Skill*>());
  ~Fighter();
  CREATE_FUNC(Fighter);
  int hp, max_hp;
  int spc, max_spc;
  const std::list<Skill*>& getSkills() const;
  void setSkills(std::list<Skill*> skills);
  void update(float dt) override;

  void punch();
  void kick();

  void hitOtherCallback(BattleSystem::AttackHitEventArgs args);
  void hitByOtherCallback(BattleSystem::AttackHitEventArgs args);

private:
  std::list<Skill*> skills;
  bool flip;
  int current_state_time;
  int stun_time, attack_time;
};

#endif // !__FIGHTER_H__
