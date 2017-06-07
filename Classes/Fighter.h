#ifndef __FIGHTER_H__
#define __FIGHTER_H__

#include "cocos2d.h"
#include "BattleSystem.h"
#include "Skill.h"
#include "BaseSprite.h"
#include "Attack.h"
#include <list>

USING_NS_CC;

constexpr float G = 200.0f;

class Fighter : public BaseSprite {
public:
  /*
  出招表Json格式
  [{
    "name": String,
    [
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

  int hp, max_hp;
  int spc, max_spc;
  Vec2 velocity;
  std::shared_ptr<SkillManager> _skill_manager; // 技能管理
  bool flip = false;
  bool dir = true; // right is true, left is false

  static const std::list<State> VALID_NEXT_STATE[8];
  static std::map<std::string, BattleSystem::VirtualKey> STRING_KEY_MAP;

  const std::list<Skill*>& getSkills() const;
  void setSkills(const std::list<Skill*> &skills);
  void setSkills(const std::string &filepath);
  bool setState(State next_state, float time = -1);
  void setSkillCallback(std::string eventname);

  void update(float dt) override;

  void punch();
  void kick();
  void damage(Attack *source);
  Attack* spawnAttack(int damage, float lifetime, const std::string& filepath);
  void pressKey(const BattleSystem::VirtualKey &key);
  void releaseKey(const BattleSystem::VirtualKey &key);
  bool resetState();

  void hitOtherCallback(EventCustom *custom);
  void hitByOtherCallback(EventCustom *custom);
  void triggerSkill(EventCustom *custom);

private:
  std::list<Skill*> skills;
  float state_time = -1;
  float speed = 100.0f;
  bool left_holding = false, right_holding = false;
  State state = IDLE;
};

#endif // !__FIGHTER_H__
