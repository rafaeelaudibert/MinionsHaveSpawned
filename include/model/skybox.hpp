#include "model\base.hpp"

class SkyBox : public GameObject
{
protected:
    CubemapTexture2D texture;

public:
    SkyBox(std::string name) : GameObject(name)
    {
        this->build();
    };

    void build();

    void render(glm::mat4 view, glm::mat4 projection);
};
