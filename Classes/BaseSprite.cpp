#include "BaseSprite.h"
#include "BattleSystem.h"

BaseSprite::BaseSprite(const Rect & bounding, BattleSystem *system) : DFBoudingBox(bounding), system(system) {
  if (bounding.equals(Rect())) {
    DFBoudingBox = getBoundingBox();
  }
}

BaseSprite::BaseSprite() {}

BaseSprite::~BaseSprite() {
#ifdef __DF_DEBUG__
  system->removeChild(_bounding_display_node);
#endif
}

const Rect & BaseSprite::getDFBoundingBox() const {
  // TODO: 在此处插入 return 语句
  return DFBoudingBox;
}

void BaseSprite::setDFBoundingBox(const Rect & boundingBox) {
  DFBoudingBox = boundingBox;
}

BattleSystem * BaseSprite::getBattleSystem() {
  return this->system;
}

void BaseSprite::setBattleSystem(BattleSystem * system) {
  this->system = system;
}

void BaseSprite::setPosition(const Vec2 & pos) {
  Vec2 offset = pos - this->getPosition();
  Sprite::setPosition(pos);
  this->DFBoudingBox.origin += offset;
}

bool BaseSprite::detectCollision(const BaseSprite * other) {
  return other->getDFBoundingBox().intersectsRect(this->getDFBoundingBox());
}

void BaseSprite::update(float dt) {
#ifdef __DF_DEBUG__
  if (!_bounding_display_node) {
    _bounding_display_node = DrawNode::create();
    system->addChild(_bounding_display_node);
  }
  _bounding_display_node->clear();
  auto bounding = this->getBoundingBox();
  /*Vec2 new_origin;
  new_origin.x = DFBoudingBox.origin.x - DFBoudingBox.size.width / 2;
  new_origin.y = DFBoudingBox.origin.y - DFBoudingBox.size.height / 2;*/
  _bounding_display_node->drawRect(bounding.origin, bounding.origin + bounding.size, Color4F::BLUE);
  _bounding_display_node->drawRect(DFBoudingBox.origin, DFBoudingBox.origin + DFBoudingBox.size, Color4F::RED);
#endif
}
