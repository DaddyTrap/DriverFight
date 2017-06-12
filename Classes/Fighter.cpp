#include "Fighter.h"
#include "SkillManager.h"
#include "json/document.h"
#include "Attack.h"
#include "SimpleAudioEngine.h"


using namespace CocosDenshion;

static auto audio = SimpleAudioEngine::getInstance();

const std::list<Fighter::State> Fighter::VALID_NEXT_STATE[8] = {
  { Fighter::State::JUMP, Fighter::State::ATTACK, Fighter::State::MOVE, Fighter::State::STUN, Fighter::State::SQUAT, Fighter::State::DEFENCE },    // IDLE
  { Fighter::State::IDLE, /*Fighter::State::MOVE,*/ Fighter::State::JUMP2, Fighter::State::ATTACK, Fighter::State::STUN, Fighter::State::DEFENCE },    // JUMP
  { Fighter::State::IDLE, Fighter::State::ATTACK },                                                 // JUMP2
  { /*Fighter::State::IDLE,*/ Fighter::State::STUN },                                                   // ATTACK
  { Fighter::State::IDLE, Fighter::State::JUMP, Fighter::State::DEFENCE, Fighter::State::STUN, Fighter::State::ATTACK, Fighter::State::DEFENCE },  // MOVE
  { Fighter::State::IDLE, Fighter::State::JUMP },                                                                         // STUN
  { Fighter::State::IDLE, Fighter::State::MOVE, Fighter::State::DEFENCE },                                                   // DEFENCE
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
  // TODO: 在此处插入 return 语句
  return skills;
}

void Fighter::setSkills(const std::list<Skill*> &skills) {
  this->skills = skills;
  for (auto &i : skills) {
    i->setOwner(this);
  }
}

/*
出招表Json格式
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
  // 状态时间变化
  if (state_time > 0.0f) {
    // 只有stun和attack会由时间触发状态转变，且会转变为idle
    if (state_time - dt <= 0.0f) {
      // 是时候进入下一状态了
      resetState(true);
      state_time = -1;
    } else {
      // 继续计时
      state_time -= dt;
    }
  }

  // 水平移动状态
  if (state == MOVE || ((state == JUMP) && (left_holding || right_holding))) {
    this->setDir(right_holding);
    velocity.x = dir ? speed : -speed;
  } else {
    velocity.x = 0;
  }

  // 垂直移动状态
  if (fabs(this->getPosition().y) > 0.01) { // 垂直运动未停止
    velocity.y -= G * dt;
  }
  // 移动状态
  // 判断是否着地 (如需改变“地”高度，则改变BattleSystem的高度)
  auto predict_pos = this->getPosition() + velocity * dt;
  if (predict_pos.y <= 0.0f) {
    // 已着地，使其稳健着地并去掉垂直速度
    predict_pos.y = 0.0f;
    velocity.y = 0.0f;
    if (state == JUMP) {
      if (left_holding || right_holding) {
        setState(MOVE, -1.0f, true);
      } else {
        resetState();
      }
    }
  } else {
    setState(JUMP);
  }
  if (predict_pos.x <= 0 || predict_pos.x >= 800.0f) {
    predict_pos.x = this->getPositionX();
  }
  this->setPosition(predict_pos); // 真正设置移动后位置

  // Skill管理器
  //_skill_manager->update(dt);
  for (auto &i : skills) {
    i->stepTime();
  }
}

void Fighter::punch() {}

void Fighter::kick() {}

void Fighter::damage(Attack * source) {
  this->hp -= state == DEFENCE ? source->damage * 0.1f : source->damage;
}

Attack * Fighter::spawnAttack(int damage, float lifetime, const std::string& filepath) {
  auto atk = Attack::create();
  atk->initWithFile(filepath);
  atk->damage = damage;
  atk->lifetime = lifetime;
  atk->stuntime = 2.0f;
  // set bounding
  atk->setDFBoundingBox(atk->getBoundingBox());
  atk->setOwner(this);
  this->system->addAttack(atk);
  return atk;
}

Attack * Fighter::spawnAttack(const AttackInfo &info, const string &filepath) {
  auto atk = Attack::create();
  if (filepath != "")
    atk->initWithFile(filepath);
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
    auto flip_key = key;
    if (!dir && (flip_key == BattleSystem::VirtualKey::LEFT || flip_key == BattleSystem::VirtualKey::RIGHT)) {
      flip_key = flip_key == BattleSystem::VirtualKey::LEFT ? BattleSystem::VirtualKey::RIGHT : BattleSystem::VirtualKey::LEFT;
    }
    if (i->pressKey(flip_key) && !skill_flag) {
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
      this->velocity.y = 800.0f;
      audio->playEffect("sounds/jump.mp3");
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
      audio->playEffect("sounds/punch.mp3");
    }
    break;
  case BattleSystem::B:
    this->atk_type = KICK;
    if (this->setState(ATTACK)) {
      this->spawnAttack(kick_info);
      audio->playEffect("sounds/kick.mp3");
    }
    break;
  case BattleSystem::C:
    c_holding = true;
    if (this->setState(DEFENCE)) {
      
    }
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
    c_holding = false;
    resetState();
    break;
  default:
    break;
  }
}

bool Fighter::resetState(bool force) {
  // 如果正在按键，尝试改为MOVE状态
  if (left_holding || right_holding) {
    bool result = setState(MOVE, -1.0f, force);
    // 成功则return，失败则继续
    if (result) return true;
  }
  // (空中则JUMP，地上则IDLE)
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
  this->damage(args.atk);
  if (state == DEFENCE) return;
  auto stuntime = args.atk->stuntime;
  if (this->setState(STUN, stuntime)) {
    //SimpleAudioEngine::getInstance()->playEffect("sounds/hit.mp3");
  }
}

void Fighter::hitByOtherCallback(EventCustom *custom) {
  /*auto args = static_cast<BattleSystem::AttackHitEventArgs*>(custom->getUserData());
  auto stuntime = args->atk->stuntime;
  this->setState(STUN, stuntime);
  this->damage(args->atk);
  delete args;*/
}

void Fighter::triggerSkill(EventCustom *custom) {
  //auto args = static_cast<Skill::SkillEventArgs*>(custom->getUserData());

  //delete args;
}

void Fighter::triggerSkill(Skill::SkillEventArgs args) {
  CCLOG("Skill: %s", args.name.c_str());
  this->atk_type = SKILL;
  if (this->setState(ATTACK)) {
    this->spawnAttack(this->fireball_info, "attacks/fireball.png");
  }
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
        if (this->getPositionY() < 0.01f)
          this->runAction(Sequence::create(Animate::create(attack_animations[0]), CallFunc::create([this]() {this->resetState(true); }), nullptr));
        else
          this->runAction(Sequence::create(Animate::create(attack_animations[3]), CallFunc::create([this]() {this->resetState(true); }), nullptr));
      } else if (atk_type == KICK) {
        this->runAction(Sequence::create(Animate::create(attack_animations[1]), CallFunc::create([this]() {this->resetState(true); }), nullptr));
      } else if (atk_type == SKILL) {
        this->runAction(Sequence::create(Animate::create(attack_animations[2]), DelayTime::create(0.2f), CallFunc::create([this]() {this->resetState(true); }), nullptr));
      }
      break;
    case JUMP:
      this->stopAllActions();
      this->runAction(Animate::create(jump_animation));
      break;
    case STUN:
      this->stopAllActions();
      this->runAction(Animate::create(stun_animation));
      break;
    case DEFENCE:
      this->stopAllActions();
      this->runAction(Animate::create(defence_animation));
    default:
      break;
    }
    return true;
  }
  return false;
}

const Fighter::State & Fighter::getState() const {
  // TODO: 在此处插入 return 语句
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
