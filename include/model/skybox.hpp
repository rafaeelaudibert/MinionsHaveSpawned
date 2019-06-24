#include "model\base.hpp"

#ifndef SKYBOX_H_
#define SKYBOX_H_

// Skybox used in the game, currently having 3 different textures representing different states of the game
class SkyBox : public GameObject
{
protected:
    CubemapTexture2D texture;
    CubemapTexture2D texture_victory;
    CubemapTexture2D texture_defeat;

public:
    SkyBox(std::string name) : GameObject(name)
    {
        this->build();
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);
};
#endif // SKYBOX_H_

