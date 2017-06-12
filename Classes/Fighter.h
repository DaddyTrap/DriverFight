#ifndef __FIGHTER_H__
#define __FIGHTER_H__

#include "cocos2d.h"

#include "Predefine.h"

#include "Skill.h"
//#include "BaseSprite.h"
#include "Attack.h"
#include <list>

#include "AttackInfo.h"

USING_NS_CC;

constexpr float G = 3200.0f;

class Fighter : public BaseSprite {
public:
  /*
  出招表Json格式
  [{
    "name": String,
    "keys": [
      [Key : String, Frame Count : int], ...
    ]
  },...]
  */
  Fighter();
  ~Fighter();
  Fighter(int max_hp, int max_spc, const std::list<Skill*> &skills, const Rect &bounding, BattleSystem *system);

  CREATE_FUNC(Fighter);
  static Fighter* create(int max_hp, int max_spc, const std::list<Skill*> &skills = {}, const Rect &bounding = {}, BattleSystem *system = nullptr);

  enum State {
    IDLE, JUMP, JUMP2, ATTACK, MOVE, STUN, DEFENCE, SQUAT
  };

  enum AttackType {
    PUNCH,
    KICK,
    SKILL
  };

  float hp, max_hp;
  float spc, max_spc;
  Vec2 velocity;
  // std::shared_ptr<SkillManager> _skill_manager; // 技能管理
  bool flip = false;
  bool dir = true; // right is true, left is false
  Animation *idle_animation, *move_animation, *jump_animation, *attack_animations[8], *stun_animation, *defence_animation;
  AttackInfo punch_info, kick_info, fireball_info;

  static const std::list<State> VALID_NEXT_STATE[8];
  static std::map<std::string, BattleSystem::VirtualKey> STRING_KEY_MAP;

  const std::list<Skill*>& getSkills() const;
  void setSkills(const std::list<Skill*> &skills);
  void setSkills(const std::string &filepath);
  bool setState(State next_state, float time = -1, bool force = false);
  const State& getState() const;
  void setSkillCallback(std::string eventname);
  void setDir(bool dir);

  void update(float dt) override;

  void punch();
  void kick();
  void damage(Attack *source);
  Attack* spawnAttack(int damage, float lifetime, const std::string& filepath);
  Attack* spawnAttack(const AttackInfo &info, const string &filepath = "");
  void pressKey(const BattleSystem::VirtualKey &key);
  void releaseKey(const BattleSystem::VirtualKey &key);
  bool resetState(bool force = false);

  void hitOtherCallback(EventCustom *custom);
  void hitByOtherCallback(BattleSystem::AttackHitEventArgs args);
  void hitByOtherCallback(EventCustom *custom);
  void triggerSkill(EventCustom *custom);
  void triggerSkill(Skill::SkillEventArgs args);

private:
  std::list<Skill*> skills;
  float state_time = -1;
  float speed = 150.0f;
  bool left_holding = false, right_holding = false, c_holding = false;
  State state = IDLE;
  AttackType atk_type = PUNCH;
};

#endif // !__FIGHTER_H__
