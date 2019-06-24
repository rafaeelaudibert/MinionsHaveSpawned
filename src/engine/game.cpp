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
#include <algorithm>

Game::Game(GLuint width, GLuint height)
    : width(width), height(height), screen_ratio((float)width / (float)height)
{

}

Game::~Game()
{
}

float Game::character_height = Constants::CHARACTER_STANDING_HEIGHT;
Camera Game::camera =  Camera(glm::vec4(0.0f, Game::character_height, 0.0f, 1.0f));
GameState Game::state = GameState::GAME_WAIT;
std::map<std::string, GameObject *> Game::objects;
std::map<std::string, Collisive *> Game::collisive_objects;
std::map<std::string, Enemy *> Game::enemy_objects;
std::map<std::string, Turret *> Game::turret_objects;
Nexus *Game::chaos_nexus, *Game::order_nexus;

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

    this->order_nexus = new Nexus("nos", NexusColorSide::CHAOS_BLUE, Constants::ORDER_NEXUS_POSITION);
    objects.insert(std::map<std::string, GameObject *>::value_type("nos", this->order_nexus));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("nos", this->order_nexus));
    printf("[GAME] Order Nexus created\n");

    this->chaos_nexus = new Nexus("ncs", NexusColorSide::CHAOS_RED, Constants::CHAOS_NEXUS_POSITION);
    objects.insert(std::map<std::string, GameObject *>::value_type("ncs", this->chaos_nexus));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("ncs", this->chaos_nexus));
    printf("[GAME] Chaos Nexus created\n");

    // Create hand
    this->hand = new Hand("hand");

    // Loading TURRET textures, shaders and .objs (only loading blue ones, because the red are not used)
    BilgerwaterChaos("", TurretColor::BLUE);
    // BilgerwaterChaos("bc", TurretColor::RED);
    BilgerwaterOrder("", TurretColor::BLUE);
    // BilgerwaterOrder("bo", TurretColor::RED);
    SummonersChaos("", TurretColor::BLUE);
    // SummonersChaos("", TurretColor::RED);
    SummonersOrder("", TurretColor::BLUE);
    // SummonersOrder("", TurretColor::RED);
    HowlingChaos("", TurretColor::BLUE);
    // HowlingChaos("", TurretColor::RED);
    HowlingOrder("", TurretColor::BLUE);
    // HowlingOrder("", TurretColor::RED);
    SiegeChaos("", TurretColor::BLUE);
    // SiegeChaos("", TurretColor::RED);

    // Loading MINION textures, shaders and .objs (only loading red chaos ones, because the blue and order are not used)
    // OrderMelee("", EnemyColor::RED);
    // OrderMelee("", EnemyColor::BLUE);
    // OrderRanged("", EnemyColor::RED);
    // OrderRanged("", EnemyColor::BLUE);
    // OrderSiege("", EnemyColor::RED);
    // OrderSiege("", EnemyColor::BLUE);
    // OrderSuper("", EnemyColor::RED);
    // OrderSuper("", EnemyColor::BLUE);
    ChaosMelee("", EnemyColor::RED);
    // ChaosMelee("cm", EnemyColor::BLUE);
    ChaosRanged("", EnemyColor::RED);
    // ChaosRanged("cr", EnemyColor::BLUE);
    ChaosSiege("", EnemyColor::RED);
    // ChaosSiege("csg", EnemyColor::BLUE);
    ChaosSuper("", EnemyColor::RED);
    // ChaosSuper("csp", EnemyColor::BLUE);

    /* CREATE THE HORDES */
    this->enemy_hordes = {
       // HORDE 1 - 160 HP
       {
           new ChaosMelee("h1_m1"), new ChaosMelee("h1_m2"), new ChaosRanged("h1_r1"), new ChaosRanged("h1_r2")
       },
       // HORDE 2 - 240 HP
       {
           new ChaosMelee("h2_m1"), new ChaosMelee("h2_m2"), new ChaosMelee("h2_m3"), new ChaosRanged("h2_r1"), new ChaosRanged("h2_r2"), new ChaosRanged("h2_r3")
       },
       // HORDE 3 - 340 HP
       {
           new ChaosSiege("h3_sg1"), new ChaosMelee("h3_m1"), new ChaosMelee("h3_m2"), new ChaosMelee("h3_m3"), new ChaosRanged("h3_r1"), new ChaosRanged("h3_r2"), new ChaosRanged("h3_r3")
       },
       // HORDE 4 - 450HP
       {
           new ChaosMelee("h4_m1"), new ChaosMelee("h4_m2"), new ChaosMelee("h4_m3"), new ChaosRanged("h4_r1"), new ChaosRanged("h4_r2"), new ChaosRanged("h4_r3"), new ChaosMelee("h4_m4"), new ChaosMelee("h4_m5"), new ChaosMelee("h4_m6"), new ChaosRanged("h4_r4"), new ChaosRanged("h4_r5"), new ChaosRanged("h4_r6")
       },
       // HORDE 5 - 500HP
       {
           new ChaosSiege("h5_sg1"), new ChaosRanged("h5_r1"), new ChaosRanged("h5_r2"), new ChaosRanged("h5_r3"), new ChaosRanged("h5_r4"), new ChaosRanged("h5_r5"), new ChaosSiege("h5_sg2"), new ChaosRanged("h5_r6"), new ChaosRanged("h5_r7"), new ChaosRanged("h5_r8"), new ChaosRanged("h5_r9"), new ChaosRanged("h5_r10")
       },
       // HORDE 6 - 700HP
       {
           new ChaosSiege("h6_sg1"), new ChaosMelee("h6_m1"), new ChaosMelee("h6_m2"), new ChaosMelee("h6_m3"), new ChaosMelee("h6_m4"), new ChaosMelee("h6_m5"), new ChaosSiege("h6_sg2"), new ChaosMelee("h6_m6"), new ChaosMelee("h6_m7"), new ChaosMelee("h6_m8"), new ChaosMelee("h6_m9"), new ChaosMelee("h6_m10")
       },
       // HORDE 7 - 730HP
       {
           new ChaosMelee("h7_m1"), new ChaosMelee("h7_m2"), new ChaosMelee("h7_m3"), new ChaosRanged("h7_r1"), new ChaosRanged("h7_r2"), new ChaosRanged("h7_r3"), new ChaosSuper("h7_sp1"), new ChaosMelee("h7_m4"), new ChaosMelee("h7_m5"), new ChaosMelee("h7_m6"), new ChaosRanged("h7_r4"), new ChaosRanged("h7_r5"), new ChaosRanged("h7_r6")
       },
       // HORDE 8 - 750HP
       {
           new ChaosSiege("h8_sg1"), new ChaosMelee("h8_m1"), new ChaosMelee("h8_m2"), new ChaosMelee("h8_m3"), new ChaosRanged("h8_r1"), new ChaosRanged("h8_r2"), new ChaosRanged("h8_r3"), new ChaosSuper("h8_sp1"), new ChaosMelee("h8_m4"), new ChaosMelee("h8_m5"), new ChaosRanged("h8_r4"), new ChaosRanged("h8_r5")
       },
       // HORDE 9 - 1000HP
       {
           new ChaosSiege("h9_sg1"), new ChaosSiege("h9_sg2"), new ChaosSiege("h9_sg3"), new ChaosSuper("h9_sp1"), new ChaosSiege("h9_sg4"), new ChaosSiege("h9_sg5"), new ChaosSiege("h9_sg6"), new ChaosSuper("h9_sp2")
       },
       // HORDE 10 - 1250HP
       {
           new ChaosSuper("h10_sp1"), new ChaosSuper("h10_sp2"), new ChaosSuper("h10_sp3"), new ChaosSuper("h10_sp4"), new ChaosSuper("h10_sp5"),
       }
    };
    this->hordes_outer_iterator = this->enemy_hordes.begin();
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

    // Update all enemy objects in the game
    for (const auto &object : this->enemy_objects)
    {
        object.second->update(this->deltaTime);
    }

    // Update all turret objects in the game
    for (const auto &object: this->turret_objects)
    {
        object.second->update(this->deltaTime);
    }

    // Remove all minions which are far beneath the ground or hitted the enemy nexus, damaging them
    for (auto it = this->enemy_objects.cbegin(), next_it = it; it != this->enemy_objects.cend(); it = next_it)
    {
        ++next_it;
        if (!it->second->is_dead() && Game::order_nexus->collide(it->second))
        {
                // Damage the ally nexus
                printf("[INFO] Minion %s hit the nexus dealing %f damage\n", it->second->name.c_str(), it->second->max_life_points);
                Game::order_nexus->hit(it->second->max_life_points);

                // Kill the minion with a lot of damage
                it->second->hit(100000);
        }

        // Remove the camera from the minion if he died
        if (it->second->is_dead() && Game::camera.look_at_target == it->second)
        {
            printf("[WARN] Minion targeted by camera will be deleted, switching it\n");
            Game::camera.switch_camera_type();
        }

        if (it->second->is_dead() && it->second->position.y <= -2.5f)
        {
            printf("[INFO] Deleting minion %s\n", it->second->name.c_str());
            this->enemy_objects.erase(it);

            // We still need to check the code below, because the user might force to change to this minion
            // Check that if the camera is focusing this minion, it should change
            if (Game::camera.look_at_target == it->second)
            {
                printf("[WARN] Minion targeted by camera will be deleted, switching it\n");
                Game::camera.switch_camera_type();
            }

            // Now we can properly free it
            delete it->second;
        }
    }

    // Makes this stage finished
    if (this->state == GameState::GAME_ACTIVE && this->wave_finished && this->enemy_objects.size() == 0)
    {
        this->state = GameState::GAME_WAIT;
        this->chaos_nexus->hit(this->chaos_nexus->max_life_points / this->enemy_hordes.size());
        printf("[GAME] Stage finished\n");

        // TODO: Give extra money to the player
    }

    // Moves the hand with the camera
    this->hand->position = camera.position;
    this->hand->position.y -= Constants::HAND_LESS_HEIGHT;
    this->hand->position -= matrix::normalize(matrix::crossproduct(camera.right, camera.world_up)) / 2.0f;
    this->hand->angle = camera.yaw / 360.0f * 3.14159 * -2;

    // Processes current wave
    if (this->state == GameState::GAME_ACTIVE && horde_inner_iterator != (*hordes_outer_iterator).end() && !this->wave_finished)
    {
        this->delta_since_last_spawn += this->deltaTime;
        if (this->delta_since_last_spawn >= Constants::SPAWN_RATE)
        {
            Enemy *enemy = *horde_inner_iterator; // Grab iterator value
            this->delta_since_last_spawn -= Constants::SPAWN_RATE;
            collisive_objects.insert(std::map<std::string, Collisive *>::value_type(enemy->name, enemy));
            enemy_objects.insert(std::map<std::string, Enemy *>::value_type(enemy->name, enemy));

            this->horde_inner_iterator++; // Update iterator
            printf("[GAME] Spawned %s\n", enemy->name.c_str());
        }
    }

    // Updates outer vector
    if (horde_inner_iterator == (*hordes_outer_iterator).end())
    {
        this->wave_finished = true;
        this->hordes_outer_iterator++;          // Updates hordes_outer_iterator
    }

    // Processes next wave
    if (this->keys[GLFW_KEY_ENTER] == GL_TRUE && this->state == GameState::GAME_WAIT && this->hordes_outer_iterator != this->enemy_hordes.end())
    {
        printf("[GAME] Starting new wave\n");
        this->wave_finished = false;
        this->keys[GLFW_KEY_ENTER]= GL_FALSE;   // Reset key
        this->delta_since_last_spawn = 0.0f;    // Reset time
        this->state = GameState::GAME_ACTIVE;   // Update game state
        this->horde_inner_iterator = (*hordes_outer_iterator).begin();
    }

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

    // Process turret placement
    if (this->camera.camera_type == CameraType::LOOK_AROUND)
    {
        // Update the turret being hold on hand
        this->update_hand_turret();

        // Puts the turret on the ground, if asked too
        this->check_place_turret();
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

    // Render enemies
    for (const auto &object : this->enemy_objects)
    {
        object.second->render(view, projection);
    }

    // Render turrets
    for (const auto &object : this->turret_objects)
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

    // Render hand and its transparent turret IF CAMERA IS OK
    if (this->camera.camera_type == CameraType::LOOK_AROUND)
    {
        this->hand->render(view, projection);
        this->hand->render_turret(view, projection);
    }
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
    if (keys[GLFW_MOUSE_BUTTON_1] == GL_TRUE && this->hand->turret != nullptr && this->hand->turret->can_place())
    {
        collisive_objects.insert(std::map<std::string, Collisive *>::value_type("turret_" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        turret_objects.insert(std::map<std::string, Turret *>::value_type("turret_" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        this->hand->turret->placed = true;      // Mark as placed
        this->hand->turret = nullptr;           // Updated it
        keys[GLFW_MOUSE_BUTTON_1] = GL_FALSE;   // Remove the flag

        printf("[GAME] Turret current holded on hand created\n");
    }
    else if (keys[GLFW_MOUSE_BUTTON_2] == GL_TRUE && this->hand->turret != nullptr)     // If uses right button, deselects turret
    {
        delete this->hand->turret;
        this->hand->turret = nullptr;
        printf("[GAME] Deselected any turret\n");
    }
}
