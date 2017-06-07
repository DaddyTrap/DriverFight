#ifndef __SKILL_H__
#define __SKILL_H__

#include <functional>
#include <cocos2d.h>
#include "BattleSystem.h"

USING_NS_CC;
using std::string;

#ifndef SKILL_MAX_KEY
#define SKILL_MAX_KEY 16
#endif

class Skill {
public:
  
  enum SkillState {
    FINISHED,
    HOLDING
  };

  struct SkillEventArgs {
    string name;
    SkillState state;
    SkillEventArgs(string name, SkillState state) : name(name), state(state) {}
  };

  Skill(const string &name, const std::vector<std::pair<BattleSystem::VirtualKey, int>>& keys, const string& eventName="");
  // Skill(const string &filepath);
  ~Skill();
  const string name;
  void pressKey(const BattleSystem::VirtualKey &key);
  bool setEventName(const string &eventName);

  void stepTime();

private:
  /*
  state��first��ʾ��ǰ����״̬��second��ʾ��ǰ״̬�ѹ�ʱ��
  keys[i]��first��ʾ������second��ʾ��״̬����ʱ��
  */
  int state = 0;
  unsigned int passed_time = 0;
  bool should_reset_time = 0;
  string eventName;
  std::vector<std::pair<BattleSystem::VirtualKey, int>> keys;
};

#endif