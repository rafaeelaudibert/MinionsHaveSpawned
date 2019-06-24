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
#include "model/turret.hpp"
#include "model/hand.hpp"
#include "model/nexus.hpp"

// Represents the current state of the game
enum GameState
{
    GAME_ACTIVE,
    GAME_WAIT,
    GAME_WIN,
    GAME_LOSE
};

// Represents the player status
enum PlayerStatus
{
    STANDING,
    CROUCHING,
    UNCROUCHING,
    JUMPING
};

// Represents the turrets you can hold on hand
enum Turrets
{
    NONE = GLFW_KEY_0,
    BILGERWATER_CHAOS,
    BILGERWATER_ORDER,
    HOWLING_CHAOS,
    HOWLING_ORDER,
    SIEGE_CHAOS,
    SUMMONERS_CHAOS,
    SUMMONERS_ORDER
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
// Some of the attributes are static, to avoid to pass parameters
// all around the functions
class Game
{
public:
    // GameState variables
    static GameState state;
    GLboolean keys[1024] = {false};
    GLuint width, height;
    float screen_ratio;

    // Mechanic variables
    static float character_height;                          // Height of the character, in meters
    PlayerStatus player_status = PlayerStatus::STANDING;    // Current player status
    GLfloat y_speed = 0;                                    // Current character falling velocity
    glm::vec4 aiming_position;                              // Position we are aiming at/placing a turret

    // Time variables
    GLfloat lastFrame = 0.0f;
    GLfloat deltaTime = 0.0f;
    GLfloat delta_since_last_spawn = 0.0f;

    // Camera
    static Camera camera;

    /* MAPS */
    static std::map<std::string, GameObject *> objects;             // All objects map
    static std::map<std::string, Collisive *> collisive_objects;    // Collisive objects map
    static std::map<std::string, Enemy *> enemy_objects;            // Enemy objects map
    static std::map<std::string, Turret *> turret_objects;         // Turrets objects map

    /* HORDES OF MINIONS CONTROL */
    std::vector<std::vector<Enemy *> > enemy_hordes;
    std::vector<std::vector<Enemy *> >::iterator hordes_outer_iterator;
    std::vector<Enemy *>::iterator horde_inner_iterator;
    bool wave_finished = false;

    // Hand
    Hand *hand;

    // Game nexus
    static Nexus *order_nexus, *chaos_nexus;

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

private:
    // Helpers to process game input
    void update_hand_turret();
    void check_place_turret();
};

#endif
