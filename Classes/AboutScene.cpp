#include "AboutScene.h"
#include "StartMenu.h"
USING_NS_CC;
Scene* AboutScene::createScene() {
  // 'scene' is an autorelease object  
  auto scene = Scene::create();

  // 'layer' is an autorelease object  
  auto layer = AboutScene::create();

  // add layer as a child to scene  
  scene->addChild(layer);

  // return the scene  
  return scene;
}

// on "init" you need to initialize your instance  
bool AboutScene::init() {
  //////////////////////////////  
  // 1. super init first  
  if (!Layer::init()) {
    return false;
  }
  auto back = LayerColor::create(Color4B(255,255,255,110));
  addChild(back, 1);
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto bg = Sprite::create("menubg.png");
  bg->setScale(visibleSize.width / bg->getContentSize().width);
  bg->setPosition(visibleSize / 2);
  this->addChild(bg, 0);
  CCSize size = CCDirector::sharedDirector()->getVisibleSize();
  auto blackItem = MenuItem::create(CC_CALLBACK_1(AboutScene::BackCallback, this));
  blackItem->setPosition(visibleSize.width / 2, visibleSize.height / 2);
  blackItem->setContentSize(visibleSize);//设置大小为整个屏幕的大小
  auto blackMenu = Menu::create(blackItem, NULL);
  blackMenu->setPosition(Point::ZERO);
  blackMenu->setAnchorPoint(Point::ZERO);
  this->addChild(blackMenu, 100);
  //创建要显示的文字  

  text = Label::createWithTTF("Driver Fight\nCreated by +C,zh,wh\nIt's a Game just for fun\nIt's Not about technology.\nit's about philosophy!\n", "fonts/LiheiPro.ttf", 30);
  text->setColor(Color3B::BLACK);
  text->setPosition(ccp(400, -120));

  //绘制裁剪区域  
  CCDrawNode* shap = CCDrawNode::create();
  CCPoint point[4] = { ccp(0,0), ccp(800, 0), ccp(800, 600), ccp(0, 600) };
  shap->drawPolygon(point, 4, ccc4f(255, 255, 255, 255), 2, ccc4f(255, 255, 255, 255));
  CCClippingNode* cliper = CCClippingNode::create();
  cliper->setStencil(shap);
  cliper->setAnchorPoint(ccp(.5, .5));
  cliper->setPosition(ccp(0, 0));
  addChild(cliper,2);
  //把要滚动的文字加入到裁剪区域  
  cliper->addChild(text);

  //文字滚动，超出范围后从新开始  
  schedule(schedule_selector(AboutScene::rollText));
  return true;

}

void AboutScene::rollText(float) {
  text->getPositionY() > 700 ? text->setPositionY(-150) : text->setPositionY(text->getPositionY() + 2);
}
void AboutScene::BackCallback(Ref* ps) {
  Director::getInstance()->replaceScene(CCTransitionFade::create(0.5, StartMenu::createScene()));
}