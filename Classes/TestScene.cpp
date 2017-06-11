#include "TestScene.h"
#include "SkillManager.h"
#include "Fighter.h"
#include "Attack.h"

USING_NS_CC;

using std::string;

cocos2d::Scene * TestScene::createScene() {
  auto scene = Scene::create();
  auto layer = TestScene::create();
  scene->addChild(layer);
  return scene;
}

bool TestScene::init() {
  if (!Layer::init())
    return false;

  visibleSize = Director::getInstance()->getVisibleSize();
  origin = Director::getInstance()->getVisibleOrigin();

  addListener();

  battle_system = BattleSystem::create();
  battle_system->bindButtonToFighter({
    { EventKeyboard::KeyCode::KEY_W,{ 0, BattleSystem::VirtualKey::UP } },
    { EventKeyboard::KeyCode::KEY_S,{ 0, BattleSystem::VirtualKey::DOWN } },
    { EventKeyboard::KeyCode::KEY_A,{ 0, BattleSystem::VirtualKey::LEFT } },
    { EventKeyboard::KeyCode::KEY_D,{ 0, BattleSystem::VirtualKey::RIGHT } },
    { EventKeyboard::KeyCode::KEY_J,{ 0, BattleSystem::VirtualKey::A } },
    { EventKeyboard::KeyCode::KEY_K,{ 0, BattleSystem::VirtualKey::B } },
    { EventKeyboard::KeyCode::KEY_L,{ 0, BattleSystem::VirtualKey::C } },
    { EventKeyboard::KeyCode::KEY_UP_ARROW,{ 1, BattleSystem::VirtualKey::UP } },
    { EventKeyboard::KeyCode::KEY_DOWN_ARROW,{ 1, BattleSystem::VirtualKey::DOWN } },
    { EventKeyboard::KeyCode::KEY_LEFT_ARROW,{ 1, BattleSystem::VirtualKey::LEFT } },
    { EventKeyboard::KeyCode::KEY_RIGHT_ARROW,{ 1, BattleSystem::VirtualKey::RIGHT } },
    { EventKeyboard::KeyCode::KEY_KP_END,{ 1, BattleSystem::VirtualKey::A } },
    { EventKeyboard::KeyCode::KEY_KP_DOWN,{ 1, BattleSystem::VirtualKey::B } },
    { EventKeyboard::KeyCode::KEY_KP_PG_DOWN,{ 1, BattleSystem::VirtualKey::C } },
    { EventKeyboard::KeyCode::KEY_1,{ 1, BattleSystem::VirtualKey::A } },
    { EventKeyboard::KeyCode::KEY_2,{ 1, BattleSystem::VirtualKey::B } },
    { EventKeyboard::KeyCode::KEY_3,{ 1, BattleSystem::VirtualKey::C } },
  });

  auto bg = Sprite::create("bg.png");
  bg->setScaleX(visibleSize.width / bg->getContentSize().width);
  bg->setScaleY(visibleSize.height / bg->getContentSize().height);
  bg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
  this->addChild(bg, -1);

  auto texture = Director::getInstance()->getTextureCache()->textureForKey("animations/fighter_walk.png");
  auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 300, 300)));

  AttackInfo punch_info = AttackInfo("attacks/punch.json");
  punch_info.routeFunc = [](float dt) {
    return Vec2(-0.5825f * dt, 0);
  };
  AttackInfo kick_info = AttackInfo("attacks/kick.json");
  kick_info.routeFunc = [](float dt) {
    return Vec2(-3.2f * dt, 0);
  };

  auto fighter = Fighter::create(1000, 1000);
  fighter->setSkills("skills/fighter_skill.json");
  fighter->initWithSpriteFrame(frame0);
  fighter->idle_animation = AnimationCache::getInstance()->getAnimation("fighter_idle");
  fighter->move_animation = AnimationCache::getInstance()->getAnimation("fighter_move");
  fighter->jump_animation = AnimationCache::getInstance()->getAnimation("fighter_jump");
  fighter->stun_animation = AnimationCache::getInstance()->getAnimation("fighter_stun");
  fighter->attack_animations[0] = AnimationCache::getInstance()->getAnimation("fighter_punch");
  fighter->attack_animations[1] = AnimationCache::getInstance()->getAnimation("fighter_kick");
  fighter->attack_animations[2] = AnimationCache::getInstance()->getAnimation("fighter_skill");
  auto bounding = fighter->getBoundingBox();
  fighter->setDFBoundingBox(Rect(0, 0, 0.37975 * bounding.size.width, bounding.size.height));
  fighter->punch_info = punch_info;
  fighter->kick_info = kick_info;
  battle_system->setFighter(fighter, 0);
  fighter->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

  fighter = Fighter::create(1000, 1000);
  fighter->setSkills("skills/fighter_skill.json");
  fighter->initWithSpriteFrame(frame0);
  fighter->idle_animation = AnimationCache::getInstance()->getAnimation("fighter_idle");
  fighter->move_animation = AnimationCache::getInstance()->getAnimation("fighter_move");
  fighter->jump_animation = AnimationCache::getInstance()->getAnimation("fighter_jump");
  fighter->stun_animation = AnimationCache::getInstance()->getAnimation("fighter_stun");
  fighter->attack_animations[0] = AnimationCache::getInstance()->getAnimation("fighter_punch");
  fighter->attack_animations[1] = AnimationCache::getInstance()->getAnimation("fighter_kick");
  bounding = fighter->getBoundingBox();
  fighter->setDFBoundingBox(Rect(0, 0, 0.37975 * bounding.size.width, bounding.size.height));
  battle_system->setFighter(fighter, 1);
  fighter->setPosition(Vec2(origin.x + visibleSize.width / 2 + 300.0f, origin.y + visibleSize.height / 2));

  battle_system->setAnchorPoint(Vec2(0.5, 0.5));
  battle_system->setPositionY(100.0f + origin.y + 100.0f);

  this->addChild(battle_system);

  scheduleUpdate();

  return true;
}

void TestScene::addListener() {
  // Keyboard
  auto listener = EventListenerKeyboard::create();
  listener->onKeyPressed = CC_CALLBACK_2(TestScene::onKeyPressed, this);
  listener->onKeyReleased = CC_CALLBACK_2(TestScene::onKeyReleased, this);
  _eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

  // Custom Skill Listener
  auto skill_listener = EventListenerCustom::create("test_listener", [](EventCustom *custom) {
    auto skill_args = static_cast<Skill::SkillEventArgs*>(custom->getUserData());
    CCLOG("%s, %d", skill_args->name.c_str(), skill_args->state);
  });
  _eventDispatcher->addEventListenerWithFixedPriority(skill_listener, 1);

}

void TestScene::onKeyPressed(EventKeyboard::KeyCode code, Event * ev) {
  battle_system->pressKey(code);
}

void TestScene::onKeyReleased(EventKeyboard::KeyCode code, Event * ev) {
  battle_system->releaseKey(code);
}

void TestScene::update(float dt) {
  // skill_manager->update(dt);
  battle_system->update(dt);
}
