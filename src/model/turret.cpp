#include "model/turret.hpp"
#include <algorithm>

bool Turret::can_place()
{
    // If is already placed, this doesn't make any sense
    if (this->placed)
        return true;

    // If the turet is out of the map, can't place
    if (this->out_of_map())
        return false;

    // If the camera is colliding with the turret, cant place
    if (this->collide(Game::camera.camera_bbox_min, Game::camera.camera_bbox_max))
        return false;

    // TODO: If the turret is on a place it can't be placed on the map, cant place
    // if (this->not_in_placeable_place())
    //    return false;

    // If the turret is colliding with any other collisive object, cant place
    return std::none_of(Game::collisive_objects.begin(),
                        Game::collisive_objects.end(),
                        [this](std::pair<std::string, Collisive *> entry)
    {
        return entry.second->collide(this);
    });
}

bool Turret::out_of_map()
{
    return this->position.x > Constants::MAP_SIZE / 2 ||
        this->position.x < -Constants::MAP_SIZE / 2 ||
        this->position.z > Constants::MAP_SIZE / 2 ||
        this->position.z < -Constants::MAP_SIZE / 2;
}
