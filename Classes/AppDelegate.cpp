#include "AppDelegate.h"
#include "StartMenu.h"
#include "AboutScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;
static cocos2d::Size designResolutionSize = cocos2d::Size(800, 600);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}


AppDelegate::~AppDelegate() {
}


//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
  //set OpenGL context attributions,now can only set six attributions:
  //red,green,blue,alpha,depth,stencil
  GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

  GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages() {
  return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
  // initialize director
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    glview = GLViewImpl::createWithRect("DriverFight", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
    glview = GLViewImpl::create("DriverFight");
#endif
    director->setOpenGLView(glview);
  }

  // turn on display FPS
  director->setDisplayStats(true);

  // set FPS. the default value is 1.0/60 if you don't call this
  director->setAnimationInterval(1.0 / 60);

  // Set the design resolution
  glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
  Size frameSize = glview->getFrameSize();
  // if the frame's height is larger than the height of medium size.
  if (frameSize.height > mediumResolutionSize.height) {
    director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
  }
  // if the frame's height is larger than the height of small size.
  else if (frameSize.height > smallResolutionSize.height) {
    director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
  }
  // if the frame's height is smaller than the height of medium size.
  else {
    director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
  }
  register_all_packages();

  // create a scene. it's an autorelease object


  auto sb = UserDefault::getInstance()->getStringForKey("first");

  if (sb == "")
    UserDefault::getInstance()->setStringForKey("first", "true");

  auto scene = sb == "" ? AboutScene::createScene() :StartMenu::createScene();


  // load music resources
  auto audio = SimpleAudioEngine::getInstance();
  audio->preloadBackgroundMusic("sounds/bgm.mp3");
  audio->preloadEffect("sounds/jump.mp3");
  audio->preloadEffect("sounds/kick.wav");
  audio->preloadEffect("sounds/punch.wav");
  audio->preloadEffect("sounds/kick.mp3");
  audio->preloadEffect("sounds/punch.mp3");
  audio->preloadEffect("sounds/select_start.mp3");
  audio->preloadEffect("sounds/switch.mp3");
  audio->preloadEffect("sounds/deep_dark_fantasy.mp3");
  audio->preloadEffect("sounds/hit.mp3");


  // load resources
  auto texture = director->getTextureCache()->addImage("animations/fighter_walk.png");
  auto anicache = AnimationCache::getInstance();
  Vector<SpriteFrame*> frames;
  
  // move animation
  for (int i = 0; i < 5; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.1f), "fighter_move");
  frames.clear();

  // idle animation
  frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 300, 300))));
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.1f), "fighter_idle");
  frames.clear();

  // load punch resource
  texture = director->getTextureCache()->addImage("animations/fighter_punch.png");
  // punch animation
  for (int i = 0; i < 16; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.04f), "fighter_punch");
  frames.clear();

  // load kick resource
  texture = director->getTextureCache()->addImage("animations/fighter_kick.png");
  // kick animation
  for (int i = 0; i < 22; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.03f), "fighter_kick");
  frames.clear();

  // load jump resource
  texture = director->getTextureCache()->addImage("animations/fighter_jump.png");
  // jump animation
  for (int i = 0; i < 11; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.04f), "fighter_jump");
  frames.clear();

  // load stun resource
  texture = director->getTextureCache()->addImage("animations/fighter_stun.png");
  // stun animation
  for (int i = 0; i < 7; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.06f), "fighter_stun");
  frames.clear();

  // load skill resource
  texture = director->getTextureCache()->addImage("animations/fighter_skill.png");
  // skill animation
  for (int i = 0; i < 8; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.06f), "fighter_skill");
  frames.clear();

  // load jump punch resource
  texture = director->getTextureCache()->addImage("animations/fighter_jump_punch.png");
  // jump punch animation
  for (int i = 0; i < 6; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.06f), "fighter_jump_punch");
  frames.clear();

  // load defence resource
  texture = director->getTextureCache()->addImage("animations/fighter_defence.png");
  // defence animation
  for (int i = 0; i < 3; ++i) {
    frames.pushBack(SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(i * 300, 0, 300, 300))));
  }
  anicache->addAnimation(Animation::createWithSpriteFrames(frames, 0.04f), "fighter_defence");
  frames.clear();

  // run
  director->runWithScene(scene);

  return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();

  // if you use SimpleAudioEngine, it must be pause
  // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();

  // if you use SimpleAudioEngine, it must resume here
  // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
