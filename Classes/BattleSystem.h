#ifndef __BATTLE_SYSTEM_H__
#define __BATTLE_SYSTEM_H__

#include "cocos2d.h"
// #include "SkillManager.h"
// #include "Fighter.h"
// #include "Attack.h"
#include <memory>

class Fighter;
class Attack;
class SkillManager;

USING_NS_CC;

constexpr char* fighters_hit_eventname[2] = { "__fighter0_hit__" , "__fighter1_hit__" };
constexpr char* fighters_skill_eventname[2] = { "__fighter0_skill__" , "__fighter1_skill__" };

class BattleSystem : public Node {
public:
  BattleSystem();
  ~BattleSystem();

  enum VirtualKey {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A, B, C, D, E, NOTHING
  };

  CREATE_FUNC(BattleSystem);
  struct AttackHitEventArgs {
    Attack *atk;
    AttackHitEventArgs(Attack *atk) {
      this->atk = atk;
    };
  };

  Fighter *fighters[2];
  std::list<Attack*> attacks, todel_attacks, toadd_attacks;
  std::map<EventKeyboard::KeyCode, std::pair<int, VirtualKey>> keymap;

  void update(float dt) override;

  void setFighter(Fighter *fighter, int i);
  void addAttack(Attack *atk);
  void removeAttack(Attack *atk);

  void pressKey(EventKeyboard::KeyCode code);
  void releaseKey(EventKeyboard::KeyCode code);
  void bindButtonToFighter(const std::map<EventKeyboard::KeyCode, std::pair<int, VirtualKey>> &keymap);
private:
};


#endif // !__BATTLE_SYSTEM_H__
