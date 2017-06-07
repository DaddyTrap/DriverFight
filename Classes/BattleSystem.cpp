#include "BattleSystem.h"
#include "Fighter.h"
#include "Attack.h"
BattleSystem::BattleSystem() : fighters() {}

BattleSystem::~BattleSystem() {}

void BattleSystem::update(float dt) {
  // �㶨Ҫɾ�ĺ�Ҫ�ӵ�Attack
  for (auto &i : todel_attacks) {
    attacks.remove(i);
    this->removeChild(i);
  }
  todel_attacks.clear();
  for (auto &i : toadd_attacks) {
    attacks.push_back(i);
    this->addChild(i);
  }
  toadd_attacks.clear();

  auto dispatcher = Director::getInstance()->getEventDispatcher();
  // ��⹥���Ƿ���Fighter��ײ
  for (auto &i : attacks) {
    // ��ײ���
    if (i->getOwner() == fighters[0]) {
      if (fighters[1] && i->detectCollision(fighters[1])) {
        dispatcher->dispatchCustomEvent(fighters_hit_eventname[1], new AttackHitEventArgs(i));
      }
    } else if (i->getOwner() == fighters[1]) {
      if (fighters[1] && i->detectCollision(fighters[0])) {
        dispatcher->dispatchCustomEvent(fighters_hit_eventname[0], new AttackHitEventArgs(i));
      }
    }
  }
  if (fighters[0]) fighters[0]->update(dt);
  if (fighters[1]) fighters[1]->update(dt);
}

void BattleSystem::setFighter(Fighter * fighter, int i) {
  // ����ս��ϵͳΪ����
  fighter->setBattleSystem(this);
  // ����Skill�������������¼�������
  auto skills = fighter->getSkills();
  fighter->setSkillCallback(fighters_skill_eventname[i]); // ������
  for (auto &val : skills) {
    val->setEventName(fighters_skill_eventname[i]); // ������
  }
  // ����Hit������ (��������update����ײ�����)
  _eventDispatcher->addCustomEventListener(fighters_hit_eventname[i], CC_CALLBACK_1(Fighter::hitByOtherCallback, fighter));
  // set fighter
  if (fighters[i]) this->removeChild(fighters[i], true);
  fighters[i] = fighter;
  this->addChild(fighter);
}

void BattleSystem::addAttack(Attack * atk) {
  this->toadd_attacks.push_back(atk);
}

void BattleSystem::removeAttack(Attack * atk) {
  this->todel_attacks.push_back(atk);
}

void BattleSystem::pressKey(EventKeyboard::KeyCode code) {
  auto fighter_key_pair = keymap[code];
  fighters[fighter_key_pair.first]->pressKey(fighter_key_pair.second);
}

void BattleSystem::releaseKey(EventKeyboard::KeyCode code) {
  auto fighter_key_pair = keymap[code];
  fighters[fighter_key_pair.first]->releaseKey(fighter_key_pair.second);
}

void BattleSystem::bindButtonToFighter(const std::map<EventKeyboard::KeyCode, std::pair<int, VirtualKey>>& keymap) {
  this->keymap = keymap;
}
