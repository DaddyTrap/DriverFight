#include "BattleSystem.h"
#include "Fighter.h"
#include "Attack.h"
BattleSystem::BattleSystem() : fighters() {}

BattleSystem::~BattleSystem() {}

void BattleSystem::update(float dt) {
  CCLOG("Fighter1 state: %d, hp: %f; Fighter2 state: %d, hp: %f", fighters[0] ? fighters[0]->getState() : -1, fighters[0]->hp, fighters[1] ? fighters[1]->getState() : -1, fighters[1]->hp);
  // 搞定要删的和要加的Attack
  if (!todel_attacks.empty()) {
    for (auto &i : todel_attacks) {
      attacks.remove(i);
      this->removeChild(i);
    }
    todel_attacks.clear();
  }
  if (!toadd_attacks.empty()) {
    for (auto &i : toadd_attacks) {
      attacks.push_back(i);
      this->addChild(i);
    }
    toadd_attacks.clear();
  }

  //auto dispatcher = Director::getInstance()->getEventDispatcher();
  // 检测攻击是否与Fighter碰撞
  for (auto &i : attacks) {
    // 碰撞检测
    if (i->getOwner() == fighters[0] && !i->is_destroy) {
      if (fighters[1] && i->detectCollision(fighters[1])) {
        i->hit();
        // dispatcher->dispatchCustomEvent(fighters_hit_eventname[1], new AttackHitEventArgs(i));
        fighters[1]->hitByOtherCallback(AttackHitEventArgs(i));
      }
    } else if (i->getOwner() == fighters[1] && !i->is_destroy) {
      if (fighters[1] && i->detectCollision(fighters[0])) {
        i->hit();
        // dispatcher->dispatchCustomEvent(fighters_hit_eventname[0], new AttackHitEventArgs(i));
        fighters[0]->hitByOtherCallback(AttackHitEventArgs(i));
      }
    }
    i->update(dt);
  }
  if (fighters[0]) fighters[0]->update(dt);
  if (fighters[1]) fighters[1]->update(dt);
}

void BattleSystem::setFighter(Fighter * fighter, int i) {
  // 设置战斗系统为自身
  fighter->setBattleSystem(this);
  // 设置Skill发生监听器和事件发生器
  //auto skills = fighter->getSkills();
  //fighter->setSkillCallback(fighters_skill_eventname[i]); // 监听器
  //for (auto &val : skills) {
  //  val->setEventName(fighters_skill_eventname[i]); // 发生器
  //}
  // 设置Hit监听器 (发生器在update的碰撞检测中)
  // _eventDispatcher->addCustomEventListener(fighters_hit_eventname[i], CC_CALLBACK_1(Fighter::hitByOtherCallback, fighter));
  // set fighter
  if (fighters[i]) this->removeChild(fighters[i], true);
  fighters[i] = fighter;
  this->addChild(fighter);
}

void BattleSystem::addAttack(Attack * atk) {
  atk->setBattleSystem(this);
  this->toadd_attacks.push_back(atk);
}

void BattleSystem::removeAttack(Attack * atk) {
  //atk->setBattleSystem(nullptr);
  this->todel_attacks.push_back(atk);
}

void BattleSystem::pressKey(EventKeyboard::KeyCode code) {
  if (keymap.find(code) == keymap.end()) return;
  auto fighter_key_pair = keymap[code];
  fighters[fighter_key_pair.first]->pressKey(fighter_key_pair.second);
}

void BattleSystem::releaseKey(EventKeyboard::KeyCode code) {
  if (keymap.find(code) == keymap.end()) return;
  auto fighter_key_pair = keymap[code];
  fighters[fighter_key_pair.first]->releaseKey(fighter_key_pair.second);
}

void BattleSystem::bindButtonToFighter(const std::map<EventKeyboard::KeyCode, std::pair<int, VirtualKey>>& keymap) {
  this->keymap = keymap;
}

int BattleSystem::isGameOver() {
  if (fighters[0]->hp <= 0 || fighters[1]->hp <= 0) {
    if (fighters[0]->hp <= 0 && fighters[1]->hp <= 0) {
      return -1;
    }
    if (fighters[0]->hp <= 0) {
      return 2;
    } else {
      return 1;
    }
  }
  return false;
}
