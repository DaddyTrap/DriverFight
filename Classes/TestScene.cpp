#include "TestScene.h"
#include "SkillManager.h"
#include "Fighter.h"
#include "Attack.h"
#include "StartMenu.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

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
  end = false;
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
  AttackInfo fireball_info = AttackInfo("attacks/ball.json");
  fireball_info.routeFunc = [](float dt) {
    if (dt < 0.5f) return Vec2(-1000.0f, -1000.0f);
    return Vec2(-3.2f * (dt - 0.5f), 0);
  };


  // back To main
  auto itemFont = Label::createWithTTF("Back", "fonts/LiheiPro.ttf", 30);
  itemFont->setColor(Color3B::BLACK);
  auto item = MenuItemLabel::create(itemFont, CC_CALLBACK_1(TestScene::BackCallback , this));
  auto menu = Menu::create(item, NULL);
  menu->setPosition(Vec2(visibleSize.width/ 2, visibleSize.height-50));
  addChild(menu, 5);


  auto fighter = Fighter::create(1000, 1000);
  fighter->setSkills("skills/fighter_skill.json");
  fighter->initWithSpriteFrame(frame0);
  fighter->idle_animation = AnimationCache::getInstance()->getAnimation("fighter_idle");
  fighter->move_animation = AnimationCache::getInstance()->getAnimation("fighter_move");
  fighter->jump_animation = AnimationCache::getInstance()->getAnimation("fighter_jump");
  fighter->stun_animation = AnimationCache::getInstance()->getAnimation("fighter_stun");
  fighter->defence_animation = AnimationCache::getInstance()->getAnimation("fighter_defence");
  fighter->attack_animations[0] = AnimationCache::getInstance()->getAnimation("fighter_punch");
  fighter->attack_animations[1] = AnimationCache::getInstance()->getAnimation("fighter_kick");
  fighter->attack_animations[2] = AnimationCache::getInstance()->getAnimation("fighter_skill");
  fighter->attack_animations[3] = AnimationCache::getInstance()->getAnimation("fighter_jump_punch");
  auto bounding = fighter->getBoundingBox();
  fighter->setDFBoundingBox(Rect(-0.37975 * bounding.size.width / 2, 0, 0.37975 * bounding.size.width, bounding.size.height));
  fighter->punch_info = punch_info;
  fighter->kick_info = kick_info;
  fighter->fireball_info = fireball_info;
  battle_system->setFighter(fighter, 0);
  fighter->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

  fighter = Fighter::create(1000, 1000);
  fighter->setSkills("skills/fighter_skill.json");
  fighter->initWithSpriteFrame(frame0);
  fighter->idle_animation = AnimationCache::getInstance()->getAnimation("fighter_idle");
  fighter->move_animation = AnimationCache::getInstance()->getAnimation("fighter_move");
  fighter->jump_animation = AnimationCache::getInstance()->getAnimation("fighter_jump");
  fighter->stun_animation = AnimationCache::getInstance()->getAnimation("fighter_stun");
  fighter->defence_animation = AnimationCache::getInstance()->getAnimation("fighter_defence");
  fighter->attack_animations[0] = AnimationCache::getInstance()->getAnimation("fighter_punch");
  fighter->attack_animations[1] = AnimationCache::getInstance()->getAnimation("fighter_kick");
  fighter->attack_animations[2] = AnimationCache::getInstance()->getAnimation("fighter_skill");
  fighter->attack_animations[3] = AnimationCache::getInstance()->getAnimation("fighter_jump_punch");
  bounding = fighter->getBoundingBox();
  fighter->setDFBoundingBox(Rect(-0.37975 * bounding.size.width / 2, 0, 0.37975 * bounding.size.width, bounding.size.height));
  fighter->punch_info = punch_info;
  fighter->kick_info = kick_info;
  fighter->fireball_info = fireball_info;
  battle_system->setFighter(fighter, 1);
  fighter->setPosition(Vec2(origin.x + visibleSize.width / 2 + 300.0f, origin.y + visibleSize.height / 2));

  battle_system->setAnchorPoint(Vec2(0.5, 0.5));
  battle_system->setPositionY(100.0f + origin.y + 100.0f);

  this->addChild(battle_system);

  SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/bgm.mp3", true);

  // Set Hp
  hp1 = battle_system->fighters[0]->hp;
  hp2 = battle_system->fighters[1]->hp;
  Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 950, 200)));
  Sprite* sp = Sprite::create("hp_tick.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 10, 10)));

  pT1 = ProgressTimer::create(sp);
  pT1->setScaleX(29.0f);
  pT1->setScaleY(3.15f);
  pT1->setAnchorPoint(Vec2(0, 0));
  pT1->setType(ProgressTimerType::BAR);
  pT1->setBarChangeRate(Point(1, 0));
  pT1->setMidpoint(Point(0, 1));
  pT1->setPercentage(100);
  pT1->setPosition(Vec2(70, visibleSize.height - 54.5f));
  addChild(pT1, 2);
  sp0->setAnchorPoint(Vec2(0, 1));
  sp0->setPosition(Vec2(0, visibleSize.height));
  sp0->setScale(0.5f);
  addChild(sp0, 1);

  Sprite* sp1 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 950, 200)));
  Sprite* sp2 = Sprite::create("hp_tick.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 10, 10)));

  pT2 = ProgressTimer::create(sp2);
  pT2->setScaleX(29.0f);
  pT2->setScaleY(3.15f);
  pT2->setAnchorPoint(Vec2(1, 0));
  pT2->setType(ProgressTimerType::BAR);
  pT2->setBarChangeRate(Point(1, 0));
  pT2->setMidpoint(Point(1, 1));
  pT2->setPercentage(100);
  pT2->setPosition(Vec2(visibleSize.width - 70 + origin.x, visibleSize.height - 54.5f));
  addChild(pT2, 2);
  sp1->setFlipX(true);
  sp1->setAnchorPoint(Vec2(1, 1));
  sp1->setPosition(Vec2(visibleSize.width, visibleSize.height));
  sp1->setScale(0.5f);
  addChild(sp1, 1);

  scheduleUpdate();

  return true;
}


void TestScene::BackCallback(Ref* ps) {
  Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, StartMenu::createScene()));
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
  battle_system->update(dt);
  float chp1 = battle_system->fighters[0]->hp;
  if (chp1 != hp1) {
    hp1 = chp1;
    pT1->runAction(ProgressTo::create(0.5f, hp1 * 100 / battle_system->fighters[0]->max_hp));
  }
  float chp2 = battle_system->fighters[1]->hp;
  if (chp2 != hp2) {
    hp2 = chp2;
    pT2->runAction(ProgressTo::create(0.5f, hp2 * 100 / battle_system->fighters[1]->max_hp));
  }
  if (battle_system->isGameOver() && !end) {
    end = true;
    GameOver();
  }
}

void TestScene::GameOver() {
  unschedule(schedule_selector(TestScene::update));
  _eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
  auto audio = SimpleAudioEngine::getInstance();
  audio->pauseBackgroundMusic();
  audio->playEffect("sounds/deep_dark_fantasy.mp3", false);
  Sprite* go = Sprite::create("gameOver.png");
  go->setPosition(visibleSize / 2);
  this->addChild(go, 3);
  auto blackItem = MenuItem::create(CC_CALLBACK_1(TestScene::BackCallback, this));
  blackItem->setPosition(visibleSize.width / 2, visibleSize.height / 2);
  blackItem->setContentSize(visibleSize);//设置大小为整个屏幕的大小
  auto blackMenu = Menu::create(blackItem, NULL);
  blackMenu->setPosition(Point::ZERO);
  blackMenu->setAnchorPoint(Point::ZERO);
  this->addChild(blackMenu, 100);
}