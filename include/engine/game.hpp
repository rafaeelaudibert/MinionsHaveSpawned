#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

#include "engine\camera.hpp"
#include "model\base.hpp"

// Represents the current state of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // GameState variables
    GameState state = GAME_ACTIVE;
    GLboolean keys[1024] = {false};
    GLuint width, height;
    float screen_ratio;

    // Mechanic variables
    const GLfloat GRAVITY = -9.8f;
    const GLfloat MAX_SPEED = -53.0f; // Maximum falling speed due to air resistance]
    const GLfloat JUMP_SPEED = 2.5f;
    GLfloat y_speed = 0;

    // Camera
    Camera camera;

    // Objects map
    std::vector<GameObject> objects;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void init();

    // GameLoop helpers
    void process_input(GLfloat dt);
    void update(GLfloat dt);
    void render();
};

#endif
