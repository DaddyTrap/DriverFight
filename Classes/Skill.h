#pragma once

#ifndef __SKILL_H__
#define __SKILL_H__

#include <functional>
#include <cocos2d.h>

USING_NS_CC;
using std::string;

#ifndef SKILL_MAX_KEY
#define SKILL_MAX_KEY 16
#endif

class Skill {
public:
  enum VirtualKey {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A, B, C, D, E, NOTHING
  };

  enum SkillState {
    FINISHED,
    HOLDING
  };

  struct SkillEventArgs {
    string name;
    SkillState state;
    SkillEventArgs(string name, SkillState state) : name(name), state(state) {}
  };

  Skill(const string &name, const std::vector<std::pair<VirtualKey, int>>& keys, const string& eventName="");
  Skill(const string &filepath);
  ~Skill();
  const string name;
  void pressKey(const VirtualKey &key);
  bool setCallBack(const string &eventName);

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
  std::vector<std::pair<VirtualKey, int>> keys;
};

#endif