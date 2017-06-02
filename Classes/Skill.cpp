#include "Skill.h"
#include "json/document.h"

USING_NS_CC;

using std::string;

Skill::Skill(const string &name, const std::vector<std::pair<VirtualKey, int>> &keys, const string &eventName) : name(name), keys(keys), eventName(eventName), state(0) {}

Skill::Skill(const string &filepath) : state(0) {
  auto data = FileUtils::getInstance()->getStringFromFile(filepath);
  rapidjson::Document json_doc;
  json_doc.Parse(data.c_str());
  // TODO: 序列化出招表
}

Skill::~Skill() {}

void Skill::pressKey(const VirtualKey &key) {
  //CCLOG("Pressed %d, Current state is %d, Passed Time is %d, size %d", key, state, passed_time, keys.size());
  if (keys[state].first == key) {
    // 按键正是所期待按键
    if (passed_time > keys[state].second) {
      // 输入超时，重置状态、重置计时
      should_reset_time = true;
      state = 0;
      return;
    }
    // 未超时，进入下一状态
    ++state;
    should_reset_time = true;
    if (state >= keys.size()) {
      // 完成最后一个状态，触发招式输入完成事件
      EventCustom custom(eventName);
      custom.setUserData(new SkillEventArgs(name, FINISHED));
      state = 0;
      auto dispatcher = Director::getInstance()->getEventDispatcher();
      dispatcher->dispatchEvent(&custom);
    }
  } else {
    // 按键不是所要的按键，重置
    state = 0;
    should_reset_time = true;
  }
}

bool Skill::setCallBack(const string& eventName) {
  this->eventName = eventName;
  return false;
}

void Skill::stepTime() {
  if (should_reset_time || state == 0) {
    passed_time = 0;
    should_reset_time = false;
  } else {
    ++passed_time;
  }
}

