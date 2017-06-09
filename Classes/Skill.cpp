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
    // �����������ڴ�����
    if (passed_time > keys[state].second) {
      // ���볬ʱ������״̬�����ü�ʱ
      should_reset_time = true;
      state = 0;
      return false;
    }
    // δ��ʱ��������һ״̬
    ++state;
    should_reset_time = true;
    if (state >= keys.size()) {
      // ������һ��״̬��������ʽ��������¼�
      /*if (eventName != "") {
        EventCustom custom(eventName);
        custom.setUserData(new SkillEventArgs(name, FINISHED));
        state = 0;
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&custom);
      }*/

      state = 0;
      // ����true��ʾ״̬���
      return true;
    }
  } else {
    // ����������Ҫ�İ���������
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

