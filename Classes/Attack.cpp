#include "Attack.h"
#include "json/document.h"

Attack::Attack() {}

Attack::~Attack() {}

Fighter * Attack::getOwner() const {
  return owner;
}

// Caution! 副作用：根据 initial_pos ，将自身放置在与owner位置有关的位置上
// 将会设置好attack的sprite位置、DFBoundingBox位置
void Attack::setOwner(Fighter * owner) {
  this->owner = owner;
  //this->setPosition(owner->getPosition() + this->initial_pos);
  auto owner_bounding = owner->getDFBoundingBox();
  auto atk_sp_box = this->getBoundingBox();

  // DFBoundingBox size
  this->DFBoudingBox.size.width = this->size.x * owner_bounding.size.width;
  this->DFBoudingBox.size.height = this->size.y * owner_bounding.size.height;

  Vec2 new_initial = initial_pos;
  Vec2 atk_bounding_offset;
  if (!owner->dir) { // 向左
    atk_bounding_offset.x = owner_bounding.size.width * new_initial.x;
    atk_bounding_offset.y = owner_bounding.size.height * new_initial.y;
  } else {
    if (initial_pos.x < 0.5f) { // 对称轴 0.5f
      new_initial.x += (0.5f - initial_pos.x) * 2 - this->size.x;
    } else {
      new_initial.x -= (initial_pos.x - 0.5f) * 2 + this->size.x;
    }
    atk_bounding_offset.x += owner_bounding.size.width * new_initial.x;
    atk_bounding_offset.y += owner_bounding.size.height * new_initial.y;
  }
  this->setPosition(Vec2(atk_bounding_offset.x - owner_bounding.size.width / 2 + atk_sp_box.size.width / 2, atk_bounding_offset.y - owner_bounding.size.height / 2 + atk_sp_box.size.height / 2));
  this->DFBoudingBox.origin = owner_bounding.origin;
  this->DFBoudingBox.origin += atk_bounding_offset;
  this->real_x_speed = owner->dir ? -x_speed * owner_bounding.size.width : x_speed * owner_bounding.size.width;
  this->real_y_speed = owner_bounding.size.height;
}

void Attack::initWithAttackInfo(const AttackInfo & info) {
  this->damage = info.damage;
  this->lifetime = info.lifetime;
  this->stuntime = info.stuntime;
  this->x_speed = info.x_speed;
  this->routeFunc = info.routeFunc;
  this->initial_pos = info.initial_pos;
  this->size = info.size;
}

void Attack::destroy() {
  this->is_destroy = true;
  this->system->removeAttack(this);
}

void Attack::hit() {
  this->destroy();
}

void Attack::update(float dt) {
  BaseSprite::update(dt);
  if (is_destroy) return;
  if (lifetime < 0) {
    this->destroy();
    return;
  }
  lifetime -= dt;
  auto now_pos = getPosition();
  auto x = real_x_speed * dt;
  dx += x;
  dy = routeFunc(dx) * real_y_speed;

  this->setPosition(now_pos + Vec2(x, dy));
}
