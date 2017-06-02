#include "Skill.h"
#include "json/document.h"

USING_NS_CC;

using std::string;

Skill::Skill(const string &name, const std::vector<std::pair<VirtualKey, int>> &keys, const string &eventName) : name(name), keys(keys), eventName(eventName), state(0) {}

Skill::Skill(const string &filepath) : state(0) {
  auto data = FileUtils::getInstance()->getStringFromFile(filepath);
  rapidjson::Document json_doc;
  json_doc.Parse(data.c_str());
  // TODO: ���л����б�
}

Skill::~Skill() {}

void Skill::pressKey(const VirtualKey &key) {
  //CCLOG("Pressed %d, Current state is %d, Passed Time is %d, size %d", key, state, passed_time, keys.size());
  if (keys[state].first == key) {
    // �����������ڴ�����
    if (passed_time > keys[state].second) {
      // ���볬ʱ������״̬�����ü�ʱ
      should_reset_time = true;
      state = 0;
      return;
    }
    // δ��ʱ��������һ״̬
    ++state;
    should_reset_time = true;
    if (state >= keys.size()) {
      // ������һ��״̬��������ʽ��������¼�
      EventCustom custom(eventName);
      custom.setUserData(new SkillEventArgs(name, FINISHED));
      state = 0;
      auto dispatcher = Director::getInstance()->getEventDispatcher();
      dispatcher->dispatchEvent(&custom);
    }
  } else {
    // ����������Ҫ�İ���������
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

