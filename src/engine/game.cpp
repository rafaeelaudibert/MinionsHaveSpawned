#include "engine/game.hpp"
#include "engine/matrices.hpp"
#include "engine/resource_manager.hpp"
#include "model/base.hpp"
#include "model/cube.hpp"
#include "utils/helpers.hpp"

Game::Game(GLuint width, GLuint height)
    : state(GAME_ACTIVE), width(width), height(height), screen_ratio((float)width / (float)height)
{
}

Game::~Game()
{
}

void Game::init()
{
    /* GAME OBJECTS CREATION */
    Cube* cube = new Cube("cube");

    /* GAME OBJECTS INSERTION TO THE GAME OBJECTS MAP */
    objects.insert(std::map<std::string, GameObject*>::value_type ("cube", cube));
}

void Game::update(GLfloat dt)
{
    y_speed = utils::clamping(y_speed + GRAVITY * dt, 100.0f, MAX_SPEED);

    camera.position.y = utils::clamping(camera.position.y + y_speed * dt, 100.0f, 0.0f);
}

void Game::process_input(GLfloat dt)
{
    // Movement
    if (keys[GLFW_KEY_W] == GL_TRUE)
        camera.process_keyboard(FORWARD, dt);
    if (keys[GLFW_KEY_S] == GL_TRUE)
        camera.process_keyboard(BACKWARD, dt);
    if (keys[GLFW_KEY_A] == GL_TRUE)
        camera.process_keyboard(LEFT, dt);
    if (keys[GLFW_KEY_D] == GL_TRUE)
        camera.process_keyboard(RIGHT, dt);

    // Jump
    if (keys[GLFW_KEY_SPACE] == GL_TRUE && camera.position.y <= 0.0f)
        y_speed = JUMP_SPEED;
}

void Game::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Calculate the view and projection matrixes */
    glm::mat4 view = this->camera.get_view_matrix();
    glm::mat4 projection = matrix::perspective_matrix(glm::radians(this->camera.zoom), screen_ratio, -0.1f, -100.0f);

    // render boxes
    GameObject* cube = this->objects.find("cube")->second;
    for (unsigned int i = 0; i < 10; i++)
    {
        cube->render(view, projection, i);
    }

}
