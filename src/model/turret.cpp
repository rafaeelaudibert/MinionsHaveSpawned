#include "model/turret.hpp"

void Turret::update(float delta_time, std::map<std::string, Enemy *> enemies) {
    // Do nothing for now, just stand
}

bool Turret::can_place() {
    return false;
}
