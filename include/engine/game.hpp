#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
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

// Represents the player status
enum PlayerStatus
{
    STANDING,
    CROUCHING,
    UNCROUCHING,
    JUMPING
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
    const GLfloat GRAVITY = -26.0f;                      // Gravity Force
    const GLfloat MAX_SPEED = -180.0f;                   // Maximum falling speed due to air resistance
    const GLfloat CHARACTER_HEIGHT = 3.0f;              // Height of the character, in meters
    const GLfloat JUMP_SPEED = 6.5f;                    // Instantaneous jump speed
    const GLfloat MAX_HEIGHT = 100.0f;                  // Maximum height the character can possibly jump to
    const GLfloat CHARACTER_CROUCHING_HEIGHT = 2.3f;    // Height of the character while crouching, in meters
    const GLfloat CROUCHING_SPEED = -3.5f;              // Instantaneous crouching speed
    const GLfloat CROUCHING_SPEED_MULTIPLIER = 0.4f;    // How much of the normal speed the player runs while crouching
    PlayerStatus player_status = STANDING;              // Current player status
    GLfloat y_speed = 0;


    // Time variables
    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;

    // Camera
    Camera camera = Camera(glm::vec4(0.0f, CHARACTER_HEIGHT, 0.0f, 1.0f));

    // Objects map
    std::map<std::string, GameObject*> objects;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void init();

    // GameLoop helpers
    void new_frame();
    void process_input();
    void update();
    void render();
};

#endif
