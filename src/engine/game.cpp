#include "engine/game.hpp"
#include "engine/resource_manager.hpp"

Game::Game(GLuint width, GLuint height)
    : state(GAME_ACTIVE), keys(), width(width), height(height)
{
}

Game::~Game()
{
}

void Game::init()
{
}

void Game::update(GLfloat dt)
{
}

void Game::process_input(GLfloat dt)
{
}

void Game::render()
{
}
