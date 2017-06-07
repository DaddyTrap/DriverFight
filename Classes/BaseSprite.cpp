#include "BaseSprite.h"

BaseSprite::BaseSprite(const Rect & bounding, BattleSystem *system) : DFBoudingBox(bounding), system(system) {}

BaseSprite::BaseSprite() {}

BaseSprite::~BaseSprite() {}

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
  Sprite::setPosition(pos);
  Vec2 offset = pos - this->getPosition();
  this->DFBoudingBox.origin += offset;
}

bool BaseSprite::detectCollision(const BaseSprite * other) {
  return other->getDFBoundingBox().intersectsRect(this->getDFBoundingBox());
}
