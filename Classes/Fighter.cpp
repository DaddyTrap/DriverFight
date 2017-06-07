#include "Fighter.h"
#include "SkillManager.h"
#include "json/document.h"

const std::list<Fighter::State> Fighter::VALID_NEXT_STATE[8] = {
  { Fighter::State::JUMP, Fighter::State::ATTACK, Fighter::State::MOVE, Fighter::State::STUN, Fighter::State::SQUAT },    // IDLE
  { Fighter::State::IDLE, Fighter::State::MOVE, Fighter::State::JUMP2, Fighter::State::ATTACK, Fighter::State::STUN },    // JUMP
  { Fighter::State::IDLE, Fighter::State::ATTACK },                                                 // JUMP2
  { Fighter::State::IDLE, Fighter::State::STUN },                                                   // ATTACK
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
  max_hp(max_hp), max_spc(max_spc), skills(skills), BaseSprite(bounding, system) {
  _skill_manager.reset(new SkillManager);
  for (auto &val : skills) {
    this->_skill_manager->addSkill(val);
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
    for (int j = 0; j < size; ++j) {
      keys.push_back(std::make_pair(STRING_KEY_MAP[json_keys[i][0].GetString()], json_keys[i][1].GetInt()));
    }
    skills.push_back(new Skill(name, keys));
  }
}

void Fighter::update(float dt) {
  CCLOG("%d", state);
  // ״̬ʱ��仯
  if (state_time > 0.0f) {
    // ֻ��stun��attack����ʱ�䴥��״̬ת�䣬�һ�ת��Ϊidle
    if (state_time - dt <= 0.0f) {
      // ��ʱ�������һ״̬��
      resetState();
      state_time = -1;
    } else {
      // ������ʱ
      state_time -= dt;
    }
  }

  // ˮƽ�ƶ�״̬
  if (state == MOVE || (state == JUMP && (left_holding || right_holding))) {
    dir = right_holding;
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
  _skill_manager->update(dt);
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
  atk->setOwner(this);
  this->system->addAttack(atk);
  return atk;
}

void Fighter::pressKey(const BattleSystem::VirtualKey & key) {
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
      this->dir = false;
    }
    break;
  case BattleSystem::RIGHT:
    right_holding = true;
    if (this->setState(MOVE)) {
      this->dir = true;
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
  _skill_manager->pressKey(key);
}

void Fighter::releaseKey(const BattleSystem::VirtualKey & key) {
  switch (key) {
  case BattleSystem::UP:
    break;
  case BattleSystem::DOWN:
    break;
  case BattleSystem::LEFT:
    left_holding = false;
    if (right_holding) {
      this->dir = true;
      break;
    }
    if (resetState()) {
      this->dir = false;
    }
    break;
  case BattleSystem::RIGHT:
    right_holding = false;
    if (left_holding) {
      this->dir = false;
      break;
    }
    if (resetState()) {
      this->dir = true;
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

bool Fighter::resetState() {
  // ������ڰ��������Ը�ΪMOVE״̬
  if (left_holding || right_holding) {
    bool result = setState(MOVE);
    // �ɹ���return��ʧ�������
    if (result) return true;
  }
  // (������JUMP��������IDLE)
  if (fabs(this->getPosition().y - 0.0f) < 0.01f) {
    return setState(IDLE);
  } else {
    return setState(JUMP);
  }
}

void Fighter::hitOtherCallback(EventCustom *custom) {
  auto args = static_cast<BattleSystem::AttackHitEventArgs*>(custom->getUserData());

  delete args;
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

bool Fighter::setState(Fighter::State next_state, float time) {
  if (this->state == next_state) return false;
  bool flag = false;
  for (auto &j : VALID_NEXT_STATE[this->state]) {
    if (next_state == j) {
      flag = true;
      break;
    }
  }
  if (flag) {
    state = next_state;
    return true;
  }
  return false;
}

void Fighter::setSkillCallback(std::string eventname) {
  auto listener = EventListenerCustom::create(eventname, CC_CALLBACK_1(Fighter::triggerSkill, this));
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
