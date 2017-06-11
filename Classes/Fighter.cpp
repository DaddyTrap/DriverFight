#include "Fighter.h"
#include "SkillManager.h"
#include "json/document.h"
#include "Attack.h"

const std::list<Fighter::State> Fighter::VALID_NEXT_STATE[8] = {
  { Fighter::State::JUMP, Fighter::State::ATTACK, Fighter::State::MOVE, Fighter::State::STUN, Fighter::State::SQUAT },    // IDLE
  { Fighter::State::IDLE, /*Fighter::State::MOVE,*/ Fighter::State::JUMP2, Fighter::State::ATTACK, Fighter::State::STUN },    // JUMP
  { Fighter::State::IDLE, Fighter::State::ATTACK },                                                 // JUMP2
  { /*Fighter::State::IDLE,*/ Fighter::State::STUN },                                                   // ATTACK
  { Fighter::State::IDLE, Fighter::State::JUMP, Fighter::State::DEFENCE, Fighter::State::STUN, Fighter::State::ATTACK },  // MOVE
  { Fighter::State::IDLE, Fighter::State::JUMP },                                                                         // STUN
  { Fighter::State::IDLE, Fighter::State::MOVE },                                                   // DEFENCE
  { Fighter::State::IDLE, Fighter::State::STUN, Fighter::State::ATTACK }                            // SQUAT
};

std::map<std::string, BattleSystem::VirtualKey> Fighter::STRING_KEY_MAP = {
  { "UP", BattleSystem::UP },
  { "DOWN", BattleSystem::DOWN },
  { "RIGHT", BattleSystem::RIGHT },
  { "LEFT", BattleSystem::LEFT },
  { "A", BattleSystem::A },
  { "B", BattleSystem::B },
  { "C", BattleSystem::C },
  { "D", BattleSystem::D },
  { "E", BattleSystem::E },
  { "NOTHING", BattleSystem::NOTHING },
};

Fighter::Fighter() {}

Fighter::~Fighter() {}

Fighter::Fighter(int max_hp, int max_spc, const std::list<Skill*> &skills, const Rect &bounding, BattleSystem *system) :
  max_hp(max_hp), max_spc(max_spc), hp(max_hp), spc(max_spc), skills(skills), BaseSprite(bounding, system) {
  /*_skill_manager.reset(new SkillManager);
  for (auto &val : skills) {
    this->_skill_manager->addSkill(val);
  }*/
  this->skills = skills;
  for (auto &i : skills) {
    i->setOwner(this);
  }
}

Fighter * Fighter::create(int max_hp, int max_spc, const std::list<Skill*> &skills, const Rect &bounding, BattleSystem *system) {
  Fighter *pRet = new(std::nothrow) Fighter(max_hp, max_spc, skills, bounding, system);
  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

const std::list<Skill*>& Fighter::getSkills() const {
  // TODO: �ڴ˴����� return ���
  return skills;
}

void Fighter::setSkills(const std::list<Skill*> &skills) {
  this->skills = skills;
  for (auto &i : skills) {
    i->setOwner(this);
  }
}
/*
���б�Json��ʽ
  [{
    "name": String,
    "keys": [
      [Key : String, Frame Count : int], ...
    ]
  },...]
*/
void Fighter::setSkills(const std::string & filepath) {
  auto data = FileUtils::getInstance()->getStringFromFile(filepath);
  rapidjson::Document json_doc;
  json_doc.Parse(data.c_str());

  auto size = json_doc.Capacity();
  for (int i = 0; i < size; ++i) {
    std::string name = json_doc[i]["name"].GetString();
    std::vector<std::pair<BattleSystem::VirtualKey, int>> keys;
    auto key_size = json_doc[i]["keys"].Capacity();
    auto &json_keys = json_doc[i]["keys"];
    auto keys_size = json_keys.Capacity();
    for (int j = 0; j < keys_size; ++j) {
      keys.push_back(std::make_pair(STRING_KEY_MAP[json_keys[j][0].GetString()], json_keys[j][1].GetInt()));
    }
    skills.push_back(new Skill(name, keys));
  }
  for (auto &i : skills) {
    i->setOwner(this);
  }
}

void Fighter::update(float dt) {
  BaseSprite::update(dt);
  // ״̬ʱ��仯
  if (state_time > 0.0f) {
    // ֻ��stun��attack����ʱ�䴥��״̬ת�䣬�һ�ת��Ϊidle
    if (state_time - dt <= 0.0f) {
      // ��ʱ�������һ״̬��
      resetState(true);
      state_time = -1;
    } else {
      // ������ʱ
      state_time -= dt;
    }
  }

  // ˮƽ�ƶ�״̬
  if (state == MOVE || (state == JUMP && (left_holding || right_holding))) {
    this->setDir(right_holding);
    velocity.x = dir ? speed : -speed;
  } else {
    velocity.x = 0;
  }

  // ��ֱ�ƶ�״̬
  if (fabs(this->getPosition().y) > 0.01) { // ��ֱ�˶�δֹͣ
    velocity.y -= G * dt;
  }
  // �ƶ�״̬
  // �ж��Ƿ��ŵ� (����ı䡰�ء��߶ȣ���ı�BattleSystem�ĸ߶�)
  auto predict_pos = this->getPosition() + velocity * dt;
  if (predict_pos.y <= 0.0f) {
    // ���ŵأ�ʹ���Ƚ��ŵز�ȥ����ֱ�ٶ�
    predict_pos.y = 0.0f;
    velocity.y = 0.0f;
    if (state == JUMP)
      resetState();
  } else {
    setState(JUMP);
  }
  this->setPosition(predict_pos); // ���������ƶ���λ��

  // Skill������
  //_skill_manager->update(dt);
  for (auto &i : skills) {
    i->stepTime();
  }
}

void Fighter::punch() {}

void Fighter::kick() {}

void Fighter::damage(Attack * source) {
  this->hp -= source->damage;
}

Attack * Fighter::spawnAttack(int damage, float lifetime, const std::string& filepath) {
  auto atk = Attack::create();
  atk->initWithFile(filepath);
  atk->damage = damage;
  atk->lifetime = lifetime;
  atk->stuntime = 2.0f;
  // set bounding
  atk->setDFBoundingBox(atk->getBoundingBox());
  /*auto offset = Vec2(this->getBoundingBox().size.width / 2 + atk->getBoundingBox().size.width / 2, 0);
  if (dir) {
    atk->setPosition(this->getPosition() + offset);
  } else {
    atk->setPosition(this->getPosition() - offset);
  }*/
  atk->setOwner(this);
  this->system->addAttack(atk);
  return atk;
}

Attack * Fighter::spawnAttack(const AttackInfo &info) {
  auto atk = Attack::create();
  atk->initWithAttackInfo(info);
  atk->setOwner(this);
  this->system->addAttack(atk);
  return atk;
}

void Fighter::pressKey(const BattleSystem::VirtualKey & key) {
  //_skill_manager->pressKey(key);
  Skill::SkillEventArgs skill_args_to_trigger("", Skill::FINISHED);
  bool skill_flag = false;
  for (auto &i : skills) {
    if (i->pressKey(key) && !skill_flag) {
      skill_flag = true;
      skill_args_to_trigger = Skill::SkillEventArgs(i->name, Skill::FINISHED);
    }
  }
  if (skill_flag) {
    this->triggerSkill(skill_args_to_trigger);
    return;
  }

  switch (key) {
  case BattleSystem::UP:
    if (this->setState(JUMP)) {
      this->velocity.y = 200.0f;
    }
    break;
  case BattleSystem::DOWN:
    break;
  case BattleSystem::LEFT:
    left_holding = true;
    if (this->setState(MOVE)) {
      this->setDir(false);
    }
    break;
  case BattleSystem::RIGHT:
    right_holding = true;
    if (this->setState(MOVE)) {
      this->setDir(true);
    }
    break;
  case BattleSystem::A:
    this->atk_type = PUNCH;
    if (this->setState(ATTACK)) {
      this->spawnAttack(punch_info);
    }
    break;
  case BattleSystem::B:
    this->atk_type = KICK;
    if (this->setState(ATTACK)) {
      this->spawnAttack(kick_info);
    }
    break;
  case BattleSystem::C:
    break;
  default:
    break;
  }
}

void Fighter::releaseKey(const BattleSystem::VirtualKey & key) {
  switch (key) {
  case BattleSystem::UP:
    break;
  case BattleSystem::DOWN:
    break;
  case BattleSystem::LEFT:
    left_holding = false;
    if (right_holding && (state == MOVE || state == JUMP)) {
      this->setDir(true);
      break;
    }
    if (resetState()) {
      this->setDir(false);
    }
    break;
  case BattleSystem::RIGHT:
    right_holding = false;
    if (left_holding && (state == MOVE || state == JUMP)) {
      this->setDir(false);
      break;
    }
    if (resetState()) {
      this->setDir(true);
    }
    break;
  case BattleSystem::A:
    break;
  case BattleSystem::B:
    break;
  case BattleSystem::C:
    break;
  default:
    break;
  }
}

bool Fighter::resetState(bool force) {
  // ������ڰ��������Ը�ΪMOVE״̬
  if (left_holding || right_holding) {
    bool result = setState(MOVE, -1.0f, force);
    // �ɹ���return��ʧ�������
    if (result) return true;
  }
  // (������JUMP��������IDLE)
  if (fabs(this->getPosition().y - 0.0f) < 0.01f) {
    return setState(IDLE, -1.0f, force);
  } else {
    return setState(JUMP, -1.0f, force);
  }
}

void Fighter::hitOtherCallback(EventCustom *custom) {
  auto args = static_cast<BattleSystem::AttackHitEventArgs*>(custom->getUserData());

  delete args;
}

void Fighter::hitByOtherCallback(BattleSystem::AttackHitEventArgs args) {
  auto stuntime = args.atk->stuntime;
  this->setState(STUN, stuntime);
  this->damage(args.atk);
}

void Fighter::hitByOtherCallback(EventCustom *custom) {
  auto args = static_cast<BattleSystem::AttackHitEventArgs*>(custom->getUserData());
  auto stuntime = args->atk->stuntime;
  this->setState(STUN, stuntime);
  this->damage(args->atk);
  delete args;
}

void Fighter::triggerSkill(EventCustom *custom) {
  auto args = static_cast<Skill::SkillEventArgs*>(custom->getUserData());

  delete args;
}

void Fighter::triggerSkill(Skill::SkillEventArgs args) {
  CCLOG("Skill: %s", args.name.c_str());
}

bool Fighter::setState(Fighter::State next_state, float time, bool force) {
  bool flag = false;
  if (!force) {
    if (this->state == next_state) return false;
    if (this->state_time > 0) return false;
    for (auto &j : VALID_NEXT_STATE[this->state]) {
      if (next_state == j) {
        flag = true;
        break;
      }
    }
  }
  if (flag || force) {
    state_time = time;
    state = next_state;
    switch (state) {
    case MOVE:
      this->stopAllActions();
      this->runAction(RepeatForever::create(Animate::create(move_animation)));
      break;
    case IDLE:
      this->stopAllActions();
      this->runAction(RepeatForever::create(Animate::create(idle_animation)));
      break;
    case ATTACK:
      this->stopAllActions();
      if (atk_type == PUNCH) {
        this->runAction(Sequence::create(Animate::create(attack_animations[0]), CallFunc::create([this]() {this->resetState(true); }), nullptr));
      } else if (atk_type == KICK) {
        this->runAction(Sequence::create(Animate::create(attack_animations[1]), CallFunc::create([this]() {this->resetState(true); }), nullptr));
      } else if (atk_type == SKILL) {
        this->runAction(Sequence::create(Animate::create(attack_animations[2]), CallFunc::create([this]() {this->resetState(true); }), nullptr));
      }
      break;
    case JUMP:
      this->stopAllActions();
      this->runAction(Animate::create(jump_animation));
      break;
    default:
      break;
    }
    return true;
  }
  return false;
}

const Fighter::State & Fighter::getState() const {
  // TODO: �ڴ˴����� return ���
  return state;
}

void Fighter::setSkillCallback(std::string eventname) {
  //auto listener = EventListenerCustom::create(eventname, CC_CALLBACK_1(Fighter::triggerSkill, this));
  //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Fighter::setDir(bool dir) {
  this->setFlippedX(dir);
  this->dir = dir;
}
