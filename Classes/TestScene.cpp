#include "TestScene.h"
#include "SkillManager.h"

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

  // °´¼ü¹ÜÀí
  skill_manager = SkillManager::create();
  auto skill = new Skill("test", std::vector<std::pair<Skill::VirtualKey, int>>({
    {Skill::DOWN, 10},
    {Skill::RIGHT, 10},
    {Skill::A, 10}
  }), "test_listener");
  skill_manager->addSkill(skill);

  skill = new Skill("test1", std::vector<std::pair<Skill::VirtualKey, int>>({
    { Skill::DOWN, 10 },
    { Skill::LEFT, 10 },
    { Skill::A, 10 }
  }), "test_listener");
  skill_manager->addSkill(skill);

  skill = new Skill("test2", std::vector<std::pair<Skill::VirtualKey, int>>({
    { Skill::LEFT, 10 },
    { Skill::RIGHT, 10 },
    { Skill::A, 10 }
  }), "test_listener");
  skill_manager->addSkill(skill);

  skill = new Skill("test3", std::vector<std::pair<Skill::VirtualKey, int>>({
    { Skill::RIGHT, 10 },
    { Skill::LEFT, 10 },
    { Skill::A, 10 }
  }), "test_listener");
  skill_manager->addSkill(skill);

  this->addChild(skill_manager);

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
  switch (code) {
  case EventKeyboard::KeyCode::KEY_W:
    skill_manager->pressKey(Skill::UP);
    break;
  case EventKeyboard::KeyCode::KEY_A:
    skill_manager->pressKey(Skill::LEFT);
    break;
  case EventKeyboard::KeyCode::KEY_S:
    skill_manager->pressKey(Skill::DOWN);
    break;
  case EventKeyboard::KeyCode::KEY_D:
    skill_manager->pressKey(Skill::RIGHT);
    break;
  case EventKeyboard::KeyCode::KEY_J:
    skill_manager->pressKey(Skill::A);
    break;
  default:
    break;
  }
}

void TestScene::onKeyReleased(EventKeyboard::KeyCode code, Event * ev) {}

void TestScene::update(float dt) {
  skill_manager->update(dt);
}
