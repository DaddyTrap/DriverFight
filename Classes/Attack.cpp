#include "Attack.h"

Attack::Attack() {}

Attack::~Attack() {}

Fighter * Attack::getOwner() const {
  return owner;
}

void Attack::setOwner(Fighter * owner) {
  this->owner = owner;
}

void Attack::destroy() {
  this->is_destroy = true;
  this->DFBoudingBox = Rect(); // ²»Åö×²
}

void Attack::update(float dt) {
  if (is_destroy) return;
  if (lifetime < 0) {
    this->destroy();
    return;
  }
  lifetime -= dt;
  this->setPosition(this->getPosition() + velocity * dt);
}
