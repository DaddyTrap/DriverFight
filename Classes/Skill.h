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
  state的first表示当前所在状态，second表示当前状态已过时间
  keys[i]的first表示按键，second表示该状态保持时间
  */
  int state = 0;
  unsigned int passed_time = 0;
  bool should_reset_time = 0;
  string eventName;
  std::vector<std::pair<VirtualKey, int>> keys;
};

#endif