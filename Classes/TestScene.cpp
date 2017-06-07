#include "TestScene.h"
#include "SkillManager.h"
#include "Fighter.h"

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
  });

  auto fighter = Fighter::create(1000, 1000);
  fighter->initWithFile("HelloWorld.png");
  
  fighter->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

  battle_system->setFighter(fighter, 0);

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
