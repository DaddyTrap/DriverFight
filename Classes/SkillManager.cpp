#include "SkillManager.h"
#include "Skill.h"

SkillManager::SkillManager() {}

SkillManager::~SkillManager() {
  for (auto &i : skills) {
    delete i;
  }
}

void SkillManager::addSkill(Skill * skill) {
  skills.push_back(skill);
}

void SkillManager::removeSkill(Skill * skill) {
  auto iter = std::find(skills.begin(), skills.end(), skill);
  if (iter != skills.end()) {
    delete *iter;
    skills.erase(iter);
  }
}

void SkillManager::removeAllSkills() {
  for (auto &i : skills) {
    delete i;
  }
}

void SkillManager::update(float dt) {
  for (auto &i : skills) {
    i->stepTime();
  }
}

void SkillManager::pressKey(const BattleSystem::VirtualKey &key) {
  for (auto i : skills) {
    i->pressKey(key);
  }
}
