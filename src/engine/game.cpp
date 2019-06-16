#include "engine/game.hpp"
#include "engine/matrices.hpp"
#include "engine/resource_manager.hpp"
#include "engine/camera.hpp"
#include "model/base.hpp"
#include "model/cube.hpp"
#include "model/bunny.hpp"
#include "model/dummy.hpp"
#include "model/dragon.hpp"
#include "model/skybox.hpp"
#include "utils/helpers.hpp"
#include "model/enemy/order_melee.hpp"
#include "model/enemy/order_ranged.hpp"
#include "model/enemy/order_siege.hpp"
#include "model/enemy/order_super.hpp"
#include "model/enemy/chaos_melee.hpp"
#include "model/enemy/chaos_ranged.hpp"
#include "model/enemy/chaos_siege.hpp"
#include "model/enemy/chaos_super.hpp"

Game::Game(GLuint width, GLuint height)
    : state(GAME_ACTIVE), width(width), height(height), screen_ratio((float)width / (float)height)
{
}

Game::~Game()
{
}

void Game::init()
{
    printf("[GAME] Game initialization\n");

    SkyBox *skybox = new SkyBox("skybox");
    objects.insert(std::map<std::string, GameObject *>::value_type("skybox", skybox));
    printf("[GAME] Skybox created\n");

    Cube *plane = new Cube("plane", glm::vec4(0.0f, -0.005f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 0.0f), 0, glm::vec3(80.0f, 0.01f, 80.0f), "../../src/textures/full_map.jpg", "full_map");
    objects.insert(std::map<std::string, GameObject *>::value_type("plane", plane));
    printf("[GAME] Plane created\n");

    Cube *wall_1 = new Cube("wall_1", glm::vec4(-40.0f, 1.5f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(0.005f, 3.0f, 80.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_1", wall_1));
    printf("[GAME] Wall 1 created\n");

    Cube *wall_2 = new Cube("wall_2", glm::vec4(40.0f, 1.5f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(0.005f, 3.0f, 80.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_2", wall_2));
    printf("[GAME] Wall 2 created\n");

    Cube *wall_3 = new Cube("wall_3", glm::vec4(0.0f, 1.5f, -40.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(80.0f, 3.0f, 0.005f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_3", wall_3));
    printf("[GAME] Wall 3 created\n");

    Cube *wall_4 = new Cube("wall_4", glm::vec4(0.0f, 1.5f, 40.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(80.0f, 3.0f, 0.005f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_4", wall_4));
    printf("[GAME] Wall 4 created\n");

    Dummy *dummy = new Dummy("dummy", glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("dummy", dummy));
    printf("[GAME] Dummy created\n");

    Enemy *om = new OrderMelee("om", EnemyColor::RED, glm::vec4(3.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("om", om));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("om", om));
    printf("[GAME] OrderMelee created\n");

    Enemy *org = new OrderRanged("org", EnemyColor::RED, glm::vec4(5.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("org", org));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("org", org));
    printf("[GAME] OrderRanged created\n");

    Enemy *osg = new OrderSiege("osg", EnemyColor::RED, glm::vec4(7.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("osg", osg));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("osg", osg));
    printf("[GAME] OrderSiege created\n");

    Enemy *osp = new OrderSuper("osp", EnemyColor::RED, glm::vec4(9.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("osp", osp));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("osp", osp));
    printf("[GAME] OrderSuper created\n");

    Enemy *cm = new ChaosMelee("cm", EnemyColor::RED, glm::vec4(11.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("cm", cm));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("cm", cm));
    printf("[GAME] ChaosMelee created\n");

    Enemy *cr = new ChaosRanged("cr", EnemyColor::RED, glm::vec4(13.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("cr", cr));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("cr", cr));
    printf("[GAME] ChaosRanged created\n");

    Enemy *csg = new ChaosSiege("csg", EnemyColor::RED, glm::vec4(15.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("csg", csg));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("csg", csg));
    printf("[GAME] ChaosSiege created\n");

    Enemy *csp = new ChaosSuper("csp", EnemyColor::RED, glm::vec4(17.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("csp", csp));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("csp", csp));
    printf("[GAME] ChaosSuper created\n");
}

void Game::new_frame()
{
    GLfloat currentFrame = glfwGetTime();
    static GLfloat LAST_FRAME_COUNTER = currentFrame;
    static int ITERATIONS = 0;
    ITERATIONS++;

    deltaTime = currentFrame - lastFrame;

    if (glfwGetTime() - LAST_FRAME_COUNTER > 1)
    {
        printf("[INFO] FPS: %d\n", ITERATIONS);
        LAST_FRAME_COUNTER = currentFrame;
        ITERATIONS = 0;
    }

    lastFrame = currentFrame;
}

void Game::update()
{

    // Updates the falling speed, only if we are falling and not colliding with something
    if(camera.process_movement(CameraMovement::DOWN, this->deltaTime, collisive_objects, *this)) {
        y_speed = utils::clamping(y_speed + GRAVITY * this->deltaTime, 100.0f, MAX_SPEED);
    } else {
        y_speed = 0;

        // Makes the character stop falling if it hit the top of an object
        if (player_status == PlayerStatus::JUMPING)
            player_status = PlayerStatus::STANDING;
    }

}

void Game::process_input()
{
    // Movement
    if (keys[GLFW_KEY_W] == GL_TRUE)
        camera.process_movement(CameraMovement::FORWARD, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);
    if (keys[GLFW_KEY_S] == GL_TRUE)
        camera.process_movement(CameraMovement::BACKWARD, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);
    if (keys[GLFW_KEY_A] == GL_TRUE)
        camera.process_movement(CameraMovement::LEFT, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);
    if (keys[GLFW_KEY_D] == GL_TRUE)
        camera.process_movement(CameraMovement::RIGHT, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);

    // Stopped uncrouching
    if (camera.position.y >= CHARACTER_HEIGHT && player_status == PlayerStatus::UNCROUCHING)
    {
        player_status = PlayerStatus::STANDING;
        y_speed = 0;
        printf("[GAME] Back to standing\n");
    }

    // Stopped jumping
    if (camera.position.y <= CHARACTER_HEIGHT && player_status == PlayerStatus::JUMPING)
    {
        player_status = PlayerStatus::STANDING;
        printf("[GAME] Back to standing\n");
    }

    // Jump
    if (keys[GLFW_KEY_SPACE] == GL_TRUE && player_status == PlayerStatus::STANDING)
    {
        y_speed = JUMP_SPEED;
        player_status = PlayerStatus::JUMPING;
        printf("[GAME] Started jumping\n");
    }

    // Crouching
    if (keys[GLFW_KEY_LEFT_SHIFT] == GL_TRUE && player_status == PlayerStatus::STANDING)
    {
        y_speed = CROUCHING_SPEED;
        player_status = PlayerStatus::CROUCHING;
        printf("[GAME] Started crouching\n");
    }

    // Uncrouching
    if (keys[GLFW_KEY_LEFT_SHIFT] == GL_FALSE && player_status == PlayerStatus::CROUCHING)
    {
        y_speed = JUMP_SPEED;
        player_status = PlayerStatus::UNCROUCHING;
        printf("[GAME] Started uncrouching\n");
    }
}

void Game::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Calculate the view and projection matrices */
    glm::mat4 view = this->camera.get_view_matrix();
    glm::mat4 projection = matrix::perspective_matrix(glm::radians(this->camera.zoom), screen_ratio, -0.1f, -125.0f);

    // Render objects
    for (const auto &object : this->objects)
    {
        object.second->render(view, projection);
    }
}
