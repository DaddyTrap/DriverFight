#include "Skill.h"
#include "json/document.h"
#include "BattleSystem.h"

USING_NS_CC;

using std::string;

Skill::Skill(const string &name, const std::vector<std::pair<BattleSystem::VirtualKey, int>> &keys, const string &eventName) : name(name), keys(keys), eventName(eventName), state(0) {}

Skill::~Skill() {}

bool Skill::pressKey(const BattleSystem::VirtualKey &key) {
  if (name == "fireball")
  CCLOG("Pressed %d, Current state is %d, Passed Time is %d, size %d", key, state, passed_time, keys.size());
  if (keys[state].first == key) {
    // 按键正是所期待按键
    if (passed_time > keys[state].second) {
      // 输入超时，重置状态、重置计时
      should_reset_time = true;
      state = 0;
      return false;
    }
    // 未超时，进入下一状态
    ++state;
    should_reset_time = true;
    if (state >= keys.size()) {
      // 完成最后一个状态，触发招式输入完成事件
      /*if (eventName != "") {
        EventCustom custom(eventName);
        custom.setUserData(new SkillEventArgs(name, FINISHED));
        state = 0;
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&custom);
      }*/

      state = 0;
      // 返回true表示状态完成
      return true;
    }
  } else {
    // 按键不是所要的按键，重置
    state = 0;
    should_reset_time = true;
  }
  return false;
}

bool Skill::setEventName(const string& eventName) {
  this->eventName = eventName;
  return true;
}

Fighter * Skill::getOwner() {
  return owner;
}

void Skill::setOwner(Fighter * fighter) {
  owner = fighter;
}

void Skill::stepTime() {
  if (should_reset_time || state == 0) {
    passed_time = 0;
    should_reset_time = false;
  } else {
    ++passed_time;
  }
}

