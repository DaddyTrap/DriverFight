#ifndef __SKILL_MANAGER_H__
#define __SKILL_MANAGER_H__

#include "cocos2d.h"
#include "BattleSystem.h"

class Skill;

USING_NS_CC;

class SkillManager {
public:
  SkillManager();
  ~SkillManager();

  void addSkill(Skill* skill);
  void removeSkill(Skill* skill);
  void removeAllSkills();
  void update(float dt);
  void pressKey(const BattleSystem::VirtualKey &key);

private:
  std::list<Skill*> skills = {};
};

#endif