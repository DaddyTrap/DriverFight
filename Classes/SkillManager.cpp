#include "SkillManager.h"



SkillManager::SkillManager() {}


SkillManager::~SkillManager() {
  for (auto i : skills) {
    delete i;
  }
}

void SkillManager::addSkill(Skill * skill) {
  skills.push_back(skill);
}

void SkillManager::removeSkill(Skill * skill) {
  auto iter = std::find(skills.begin(), skills.end(), skill);
  if (iter != skills.end()) skills.erase(iter);
}

void SkillManager::update(float dt) {
  for (auto i : skills) {
    i->stepTime();
  }
}

void SkillManager::pressKey(Skill::VirtualKey key) {
  for (auto i : skills) {
    i->pressKey(key);
  }
}
