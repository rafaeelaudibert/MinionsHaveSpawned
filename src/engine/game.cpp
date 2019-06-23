#include "engine/constants.hpp"
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
#include "model/turret/bilgerwater_chaos.hpp"
#include "model/turret/bilgerwater_order.hpp"
#include "model/turret/summoners_chaos.hpp"
#include "model/turret/summoners_order.hpp"
#include "model/turret/howling_chaos.hpp"
#include "model/turret/howling_order.hpp"
#include "model/turret/siege_chaos.hpp"

Game::Game(GLuint width, GLuint height)
    : width(width), height(height), screen_ratio((float)width / (float)height)
{
}

Game::~Game()
{
}

float Game::character_height = Constants::CHARACTER_STANDING_HEIGHT;
Camera Game::camera =  Camera(glm::vec4(0.0f, Game::character_height, 0.0f, 1.0f));
GameState Game::state = GameState::GAME_ACTIVE;
std::map<std::string, GameObject *> Game::objects;
std::map<std::string, Collisive *> Game::collisive_objects;
std::map<std::string, Enemy *> Game::enemy_objects;
std::map<std::string, Turret *> Game::turret_objects;

void Game::init()
{
    printf("[GAME] Game initialization\n");

    SkyBox *skybox = new SkyBox("skybox");
    objects.insert(std::map<std::string, GameObject *>::value_type("skybox", skybox));
    printf("[GAME] Skybox created\n");

    Cube *plane = new Cube("plane", glm::vec4(0.0f, -0.005f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 0, glm::vec3(Constants::MAP_SIZE, 0.01f, Constants::MAP_SIZE), "../../src/textures/full_map.jpg", "full_map");
    objects.insert(std::map<std::string, GameObject *>::value_type("plane", plane));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("plane", plane));
    printf("[GAME] Plane created\n");

    Cube *wall_1 = new Cube("wall_1", glm::vec4(-40.0f, 1.5f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(0.005f, 3.0f, 80.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_1", wall_1));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("wall_1", wall_1));
    printf("[GAME] Wall 1 created\n");

    Cube *wall_2 = new Cube("wall_2", glm::vec4(40.0f, 1.5f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(0.005f, 3.0f, 80.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_2", wall_2));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("wall_2", wall_2));
    printf("[GAME] Wall 2 created\n");

    Cube *wall_3 = new Cube("wall_3", glm::vec4(0.0f, 1.5f, -40.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(80.0f, 3.0f, 0.005f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_3", wall_3));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("wall_3", wall_3));
    printf("[GAME] Wall 3 created\n");

    Cube *wall_4 = new Cube("wall_4", glm::vec4(0.0f, 1.5f, 40.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(80.0f, 3.0f, 0.005f));
    objects.insert(std::map<std::string, GameObject *>::value_type("wall_4", wall_4));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("wall_4", wall_4));
    printf("[GAME] Wall 4 created\n");

    Dummy *dummy = new Dummy("dummy", glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("dummy", dummy));
    printf("[GAME] Dummy created\n");

    Enemy *om = new OrderMelee("om", EnemyColor::RED, glm::vec4(3.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("om", om));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("om", om));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("om", om));
    printf("[GAME] OrderMelee created\n");

    Enemy *org = new OrderRanged("org", EnemyColor::RED, glm::vec4(5.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("org", org));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("org", org));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("org", org));
    printf("[GAME] OrderRanged created\n");

    Enemy *osg = new OrderSiege("osg", EnemyColor::RED, glm::vec4(7.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("osg", osg));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("osg", osg));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("osg", osg));
    printf("[GAME] OrderSiege created\n");

    Enemy *osp = new OrderSuper("osp", EnemyColor::RED, glm::vec4(9.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(1.5f, 1.5f, 1.5f));
    objects.insert(std::map<std::string, GameObject *>::value_type("osp", osp));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("osp", osp));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("osp", osp));
    printf("[GAME] OrderSuper created\n");

    Enemy *cm = new ChaosMelee("cm", EnemyColor::RED, glm::vec4(11.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("cm", cm));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("cm", cm));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("cm", cm));
    printf("[GAME] ChaosMelee created\n");

    Enemy *cr = new ChaosRanged("cr", EnemyColor::RED, glm::vec4(13.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(4.0f, 4.0f, 4.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("cr", cr));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("cr", cr));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("cr", cr));
    printf("[GAME] ChaosRanged created\n");

    Enemy *csg = new ChaosSiege("csg", EnemyColor::RED, glm::vec4(15.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("csg", csg));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("csg", csg));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("csg", csg));
    printf("[GAME] ChaosSiege created\n");

    Enemy *csp = new ChaosSuper("csp", EnemyColor::RED, glm::vec4(17.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0);
    objects.insert(std::map<std::string, GameObject *>::value_type("csp", csp));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("csp", csp));
    enemy_objects.insert(std::map<std::string, Enemy *>::value_type("csp", csp));
    printf("[GAME] ChaosSuper created\n");

    Turret *bc = new BilgerwaterChaos("bc", TurretColor::RED, true, 2.0f, glm::vec4(1.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("bc", bc));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("bc", bc));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("bc", bc));
    printf("[GAME] BilgerwaterChaos created\n");

    Turret *bo = new BilgerwaterOrder("bo", TurretColor::RED, true, 2.0f, glm::vec4(5.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("bo", bo));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("bo", bo));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("bo", bo));
    printf("[GAME] BilgerwaterOrder created\n");

    Turret *sc = new SummonersChaos("sc", TurretColor::BLUE, true, 2.0f, glm::vec4(9.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("sc", sc));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("sc", sc));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("sc", sc));
    printf("[GAME] SummonersChaos created\n");

    Turret *so = new SummonersOrder("so", TurretColor::RED, true, 2.0f, glm::vec4(13.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("so", so));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("so", so));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("so", so));
    printf("[GAME] SummonersOrder created\n");

    Turret *hc = new HowlingChaos("hc", TurretColor::BLUE, true, 2.0f, glm::vec4(17.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("hc", hc));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("hc", hc));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("hc", hc));
    printf("[GAME] HowlingChaos created\n");

    Turret *ho = new HowlingOrder("ho", TurretColor::RED, true, 2.0f, glm::vec4(21.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("ho", ho));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("ho", ho));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("ho", ho));
    printf("[GAME] HowlingOrder created\n");

    Turret *sgc = new SiegeChaos("sgc", TurretColor::RED, true, 2.0f, glm::vec4(25.0f, 0.0f, -5.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("sgc", sgc));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("sgc", sgc));
    turret_objects.insert(std::map<std::string, Turret *>::value_type("sgc", sgc));
    printf("[GAME] SiegeChaos created\n");

    this->order_nexus = new Nexus("nos", NexusColorSide::CHAOS_BLUE, glm::vec4(33.0f, 0.0f, -33.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("nos", this->order_nexus));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("nos", this->order_nexus));
    printf("[GAME] Order Nexus created\n");

    this->chaos_nexus = new Nexus("ncs", NexusColorSide::CHAOS_RED, glm::vec4(-33.5f, 0.0f, 32.5f, 1.0f));
    objects.insert(std::map<std::string, GameObject *>::value_type("ncs", this->chaos_nexus));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("ncs", this->chaos_nexus));
    printf("[GAME] Chaos Nexus created\n");

    // Create hand
    this->hand = new Hand("hand");
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
    if(camera.process_movement(CameraMovement::DOWN, this->deltaTime, collisive_objects, *this))
    {
        y_speed = utils::clamping(y_speed + Constants::GRAVITY * this->deltaTime, 100.0f, Constants::MAX_FALLING_SPEED);
    }
    else
    {
        // Makes the character stop falling if it hit the top of an object
        if (player_status == PlayerStatus::JUMPING)
        {
            player_status = PlayerStatus::STANDING;
            printf("[GAME] Back to standing\n");
        }
    }

    // Update character height for crouching
    if (player_status == PlayerStatus::CROUCHING)
        this->character_height = utils::clamping(this->character_height + this->y_speed * this->deltaTime, Constants::CHARACTER_STANDING_HEIGHT, Constants::CHARACTER_CROUCHING_HEIGHT);


    // Update charachter height for uncrouching
    if (player_status == PlayerStatus::UNCROUCHING)
        this->character_height = utils::clamping(this->character_height + this->y_speed * this->deltaTime, Constants::CHARACTER_STANDING_HEIGHT, Constants::CHARACTER_CROUCHING_HEIGHT);

    // Prevent player from falling indefinitely
    if (this->camera.position.y < Constants::MINIMUM_HEIGHT)
        this->camera.position.y = character_height + 0.01f;

    // Update character "hand turret" position and angle, computing it
    if (this->hand->turret != nullptr)
    {
        float camera_distance = Constants::SPAWNING_TURRET_MAX_DISTANCE;
        float denominator = matrix::dotproduct(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), this->camera.front);
        if (fabs(denominator) > Constants::EPSILON)
        {
            float camera_plane_intersection = matrix::dotproduct(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) - this->camera.position, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)) / denominator;
            if (camera_plane_intersection >= Constants::EPSILON)
                camera_distance = std::min(camera_distance, camera_plane_intersection);
        }
        this->hand->turret->position = this->camera.position + matrix::normalize(this->camera.front) * camera_distance;
        this->hand->turret->position.y = 0.0f;
        this->hand->turret->position.w = 1.0f;
        this->hand->turret->angle = std::atan2(this->camera.position.x - this->hand->turret->position.x, this->camera.position.z - this->hand->turret->position.z);
    }

    // Update all objects in the game
    for (const auto &object : this->enemy_objects)
    {
        object.second->update(this->deltaTime);
    }

    // Moves the hand with the camera
    this->hand->position = camera.position;
    this->hand->position.y -= Constants::HAND_LESS_HEIGHT;
    this->hand->position -= matrix::normalize(matrix::crossproduct(camera.right, camera.world_up)) / 2.0f;
    this->hand->angle = camera.yaw / 360.0f * 3.14159 * -2;

    // Update game state (win or lost)
    if (this->chaos_nexus->is_dead())
    {
        Game::state = GameState::GAME_WIN;
    }
    else if (this->order_nexus->is_dead())
    {
        Game::state = GameState::GAME_LOSE;
    }
}

void Game::process_input()
{
    // Movement
    if (keys[GLFW_KEY_W] == GL_TRUE)
        camera.process_movement(CameraMovement::FORWARD, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : Constants::CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);
    if (keys[GLFW_KEY_S] == GL_TRUE)
        camera.process_movement(CameraMovement::BACKWARD, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : Constants::CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);
    if (keys[GLFW_KEY_A] == GL_TRUE)
        camera.process_movement(CameraMovement::LEFT, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : Constants::CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);
    if (keys[GLFW_KEY_D] == GL_TRUE)
        camera.process_movement(CameraMovement::RIGHT, this->deltaTime * (player_status == PlayerStatus::STANDING || player_status == PlayerStatus::JUMPING ? 1 : Constants::CROUCHING_SPEED_MULTIPLIER), collisive_objects, *this);

    // Stopped uncrouching
    if (character_height >= Constants::CHARACTER_STANDING_HEIGHT && player_status == PlayerStatus::UNCROUCHING)
    {
        player_status = PlayerStatus::STANDING;
        y_speed = 0;
        printf("[GAME] Back to standing\n");
    }

    // Jump
    if (keys[GLFW_KEY_SPACE] == GL_TRUE && player_status == PlayerStatus::STANDING)
    {
        y_speed = Constants::JUMP_SPEED;
        player_status = PlayerStatus::JUMPING;
        printf("[GAME] Started jumping\n");
    }

    // Crouching
    if (keys[GLFW_KEY_LEFT_SHIFT] == GL_TRUE && player_status == PlayerStatus::STANDING)
    {
        y_speed = Constants::CROUCHING_SPEED;
        player_status = PlayerStatus::CROUCHING;
        printf("[GAME] Started crouching\n");
    }

    // Uncrouching
    if (keys[GLFW_KEY_LEFT_SHIFT] == GL_FALSE && player_status == PlayerStatus::CROUCHING)
    {
        y_speed = Constants::JUMP_SPEED;
        player_status = PlayerStatus::UNCROUCHING;
        printf("[GAME] Started uncrouching\n");
    }

    // Update the turret being hold on hand
    this->update_hand_turret();

    // Puts the turret on the ground, if asked too
    this->check_place_turret();
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

    // Render enemies health_bar (need to render later because of their transparency)
    for (const auto &object : this->enemy_objects)
    {
        object.second->render_health_bar(view, projection);
    }

    // Render nexus health bar (need to render later because of their transparency)
    this->order_nexus->render_health_bar(view, projection);
    this->chaos_nexus->render_health_bar(view, projection);

    // Render hand and its turret
    this->hand->render(view, projection);
    this->hand->render_turret(view, projection);
}

void Game::update_hand_turret()
{
    if (keys[Turrets::NONE] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = nullptr;   // Store no turret on hand
        keys[Turrets::NONE] = GL_FALSE;
        printf("[GAME] Deselected any turret\n");
    }
    else if (keys[Turrets::BILGERWATER_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new BilgerwaterChaos("bc_hand", TurretColor::BLUE);
        keys[Turrets::BILGERWATER_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a BilgerwaterChaos turret\n");
    }
    else if (keys[Turrets::BILGERWATER_ORDER] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new BilgerwaterOrder("bo_hand", TurretColor::BLUE);
        keys[Turrets::BILGERWATER_ORDER] = GL_FALSE;
        printf("[GAME] Selected a BilgerwaterOrder turret\n");
    }
    else if (keys[Turrets::HOWLING_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new HowlingChaos("hc_hand", TurretColor::BLUE);
        keys[Turrets::HOWLING_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a HowlingChaos turret\n");
    }
    else if (keys[Turrets::HOWLING_ORDER] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new HowlingOrder("ho_hand", TurretColor::BLUE);
        keys[Turrets::HOWLING_ORDER] = GL_FALSE;
        printf("[GAME] Selected a HowlingOrder turret\n");
    }
    else if (keys[Turrets::SIEGE_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new SiegeChaos("sgc_hand", TurretColor::BLUE);
        keys[Turrets::SIEGE_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a SiegeChaos turret\n");
    }
    else if (keys[Turrets::SUMMONERS_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new SummonersChaos("sc_hand", TurretColor::BLUE);
        keys[Turrets::SUMMONERS_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a SummonersChaos turret\n");
    }
    else if (keys[Turrets::SUMMONERS_ORDER] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new SummonersOrder("so_hand", TurretColor::BLUE);
        keys[Turrets::SUMMONERS_ORDER] = GL_FALSE;
        printf("[GAME] Selected a SummonersOrder turret\n");
    }
}

void Game::check_place_turret()
{
    // If there is a turret that can be placed, and the user clicks, place it
    if (keys[GLFW_MOUSE_BUTTON_1] == GL_TRUE && this->hand->turret != nullptr && this->hand->turret->can_place()) {
        this->objects.insert(std::map<std::string, GameObject *>::value_type("turret" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        collisive_objects.insert(std::map<std::string, Collisive *>::value_type("turret" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        turret_objects.insert(std::map<std::string, Turret *>::value_type("turret" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        this->hand->turret->placed = true;      // Mark as placed
        this->hand->turret = nullptr;           // Updated it
        keys[GLFW_MOUSE_BUTTON_1] = GL_FALSE;   // Remove the flag

        printf("[GAME] Turret current holded on hand created\n");
    } else if (keys[GLFW_MOUSE_BUTTON_2] == GL_TRUE && this->hand->turret != nullptr) { // If uses right button, deselects turret
        delete this->hand->turret;
        this->hand->turret = nullptr;
        printf("[GAME] Deselected any turret\n");
    }
}
