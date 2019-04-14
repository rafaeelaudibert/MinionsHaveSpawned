#include "engine/game.hpp"
#include "engine/matrices.hpp"

Game::Game(GLuint width, GLuint height)
    : state(GAME_ACTIVE), keys(), width(width), height(height)
{
}

Game::~Game()
{
}

void Game::init()
{
    this->camera = Camera();
}

void Game::update(GLfloat dt)
{
}

void Game::process_input(GLfloat dt)
{
    // Movement
    if (keys[GLFW_KEY_W] == true)
        camera.process_keyboard(FORWARD, dt);
    if (keys[GLFW_KEY_S] == true)
        camera.process_keyboard(BACKWARD, dt);
    if (keys[GLFW_KEY_A] == true)
        camera.process_keyboard(LEFT, dt);
    if (keys[GLFW_KEY_D] == true)
        camera.process_keyboard(RIGHT, dt);
}

void Game::render()
{
}
