#ifndef __SKILL_MANAGER_H__
#define __SKILL_MANAGER_H__

#include "cocos2d.h"
#include "Skill.h"

USING_NS_CC;

class SkillManager : public Node {
public:
  SkillManager();
  ~SkillManager();

  CREATE_FUNC(SkillManager);

  void addSkill(Skill* skill);
  void removeSkill(Skill* skill);
  void update(float dt) override;
  void pressKey(Skill::VirtualKey key);

private:
  std::list<Skill*> skills;
};

#endif