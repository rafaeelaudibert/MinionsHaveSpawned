#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>

#include "engine/camera.hpp"
#include "model/base.hpp"
#include "model/collisive.hpp"
#include "model/enemy.hpp"

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

    // Static constant variables
    static float constexpr GRAVITY = -26.0f;                    // Gravity Force
    static float constexpr MAX_FALLING_SPEED = -180.0f;         // Maximum falling speed due to air resistance
    static float constexpr JUMP_SPEED = 12.5f;                  // Instantaneous jump speed
    static float constexpr CROUCHING_SPEED = -3.5f;             // Instantaneous crouching speed
    static float constexpr CHARACTER_STANDING_HEIGHT = 3.0f;    // Height of the character while standing, in meters
    static float constexpr CHARACTER_CROUCHING_HEIGHT = 2.3f;   // Height of the character while crouching, in meters
    static float constexpr CROUCHING_SPEED_MULTIPLIER = 0.4f;   // How much of the normal speed the player runs while crouching
    static float constexpr SPEED = 10.0f;                       // Player speed

    // Mechanic variables
    static float character_height;                              // Height of the character, in meters
    PlayerStatus player_status = PlayerStatus::STANDING;        // Current player status
    GLfloat y_speed = 0;                                        // Current character falling velocity

    // Time variables
    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;

    // Camera
    Camera camera = Camera(glm::vec4(0.0f, Game::character_height, 0.0f, 1.0f));

    // Objects map
    std::map<std::string, GameObject *> objects;

    // Collisive objects map
    std::map<std::string, Collisive *> collisive_objects;

    // Enemy objects map
    std::map<std::string, Enemy *> enemy_objects;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void init();

    // GameLoop helpers
    void new_frame();
    void update();
    void render();

    // Processes game input
    void process_input();
};

#endif
