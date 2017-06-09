#include "AttackInfo.h"
#include "json/document.h"

AttackInfo::AttackInfo() {}

/*
{
"damage": int,
"lifetime": float,
"stuntime": float,
"x_speed": float,
"initial_pos": [float, float]
}
*/
AttackInfo::AttackInfo(const std::string & filepath) {
  rapidjson::Document json_doc;
  json_doc.Parse(FileUtils::getInstance()->getStringFromFile(filepath).c_str());

  damage = json_doc["damage"].GetInt();
  lifetime = json_doc["lifetime"].GetDouble();
  stuntime = json_doc["stuntime"].GetDouble();
  x_speed = json_doc["x_speed"].GetDouble();
  initial_pos = Vec2(json_doc["initial_pos"][0].GetDouble(), json_doc["initial_pos"][1].GetDouble());
  size = Vec2(json_doc["size"][0].GetDouble(), json_doc["size"][1].GetDouble());

  routeFunc = [](float x) { return 0.0f; };
}

AttackInfo::AttackInfo(int damage, float lifetime, float stuntime, float x_speed, Vec2 initial_pos, std::function<float(float)> routeFunc)
  : damage(damage), lifetime(lifetime), stuntime(stuntime), x_speed(x_speed), routeFunc(routeFunc), initial_pos(initial_pos) {}
