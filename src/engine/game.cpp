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
Camera Game::camera =  Camera(glm::vec4(0.0f, Game::character_height + 0.05f, 0.0f, 1.0f));
GameState Game::state = GameState::GAME_WAIT;
std::map<std::string, GameObject *> Game::objects;
std::map<std::string, Collisive *> Game::collisive_objects;
std::map<std::string, Enemy *> Game::enemy_objects;
std::map<std::string, Turret *> Game::turret_objects;
Nexus *Game::chaos_nexus, *Game::order_nexus;
float Game::gold  = Constants::START_GOLD;

// Function called to start all the objects, textures and shaders
void Game::init()
{
    printf("[GAME] Game initialization\n");

    // Create the skybox
    SkyBox *skybox = new SkyBox("skybox");
    objects.insert(std::map<std::string, GameObject *>::value_type("skybox", skybox));
    printf("[GAME] Skybox created\n");

    // Create the plane
    Cube *plane = new Cube("plane", glm::vec4(0.0f, -1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), 0, glm::vec3(Constants::MAP_SIZE, 2.0f, Constants::MAP_SIZE), "../../src/textures/full_map.jpg", "full_map");
    objects.insert(std::map<std::string, GameObject *>::value_type("plane", plane));
    collisive_objects.insert(std::map<std::string, Collisive *>::value_type("plane", plane));
    printf("[GAME] Plane created\n");

    // Create the walls
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

    // Create both nexus
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
    // OrderMelee("", Route::MID, EnemyColor::RED);
    // OrderMelee("", Route::MID, EnemyColor::BLUE);
    // OrderRanged("", Route::MID, EnemyColor::RED);
    // OrderRanged("", Route::MID, EnemyColor::BLUE);
    // OrderSiege("", Route::MID, EnemyColor::RED);
    // OrderSiege("", Route::MID, EnemyColor::BLUE);
    // OrderSuper("", Route::MID, EnemyColor::RED);
    // OrderSuper("", Route::MID, EnemyColor::BLUE);
    ChaosMelee("", Route::MID, EnemyColor::RED);
    // ChaosMelee("", Route::MID, EnemyColor::BLUE);
    ChaosRanged("", Route::MID, EnemyColor::RED);
    // ChaosRanged("", Route::MID, EnemyColor::BLUE);
    ChaosSiege("", Route::MID, EnemyColor::RED);
    // ChaosSiege("", Route::MID, EnemyColor::BLUE);
    ChaosSuper("", Route::MID, EnemyColor::RED);
    // ChaosSuper("", Route::MID, EnemyColor::BLUE);

    /* CREATE THE HORDES */
    this->enemy_hordes = this->generate_enemy_hordes();

    this->hordes_outer_iterator = this->enemy_hordes.begin();
}

// Function called to process a new frame
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

// Function called to update the game state
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
            printf("[INFO] Minion %s hit the nexus dealing %f damage\n", it->second->name.c_str(), it->second->max_life_points  * Constants::MINION_LIFE_DAMAGE_MULTIPLIER);
            Game::order_nexus->hit(it->second->current_life_points * Constants::MINION_LIFE_DAMAGE_MULTIPLIER);

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

            // Remove from the collisive map
            this->collisive_objects.erase(this->collisive_objects.find(it->second->name));
            printf("[INFO] Removed it from the collisive_objects map\n");

            // Add the gold to the player
            Game::gold += it->second->gold_reward;

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

        // Give extra money to the player as a reward
        Game::gold += Constants::GOLD_PER_WAVE;
        printf("[GAME] Current player balance: %.2f\n", this->gold);
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

            this->delta_since_last_spawn -= Constants::SPAWN_RATE; // Update delta_time
            for(auto enemy : *horde_inner_iterator)
            {
                collisive_objects.insert(std::map<std::string, Collisive *>::value_type(enemy->name, enemy));
                enemy_objects.insert(std::map<std::string, Enemy *>::value_type(enemy->name, enemy));
                printf("[GAME] Spawned %s\n", enemy->name.c_str());
            }

            this->horde_inner_iterator++; // Update iterator
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

// Function to update the input of the game
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

// Function called to actually render the game information
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

// Helper function to update the turret which the hand is holding
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
        this->hand->turret = new BilgerwaterChaos("bc_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::BILGERWATER_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a BilgerwaterChaos turret\n");
    }
    else if (keys[Turrets::BILGERWATER_ORDER] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new BilgerwaterOrder("bo_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::BILGERWATER_ORDER] = GL_FALSE;
        printf("[GAME] Selected a BilgerwaterOrder turret\n");
    }
    else if (keys[Turrets::HOWLING_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new HowlingChaos("hc_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::HOWLING_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a HowlingChaos turret\n");
    }
    else if (keys[Turrets::HOWLING_ORDER] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new HowlingOrder("ho_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::HOWLING_ORDER] = GL_FALSE;
        printf("[GAME] Selected a HowlingOrder turret\n");
    }
    else if (keys[Turrets::SIEGE_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new SiegeChaos("sgc_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::SIEGE_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a SiegeChaos turret\n");
    }
    else if (keys[Turrets::SUMMONERS_CHAOS] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new SummonersChaos("sc_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::SUMMONERS_CHAOS] = GL_FALSE;
        printf("[GAME] Selected a SummonersChaos turret\n");
    }
    else if (keys[Turrets::SUMMONERS_ORDER] == GL_TRUE)
    {
        delete this->hand->turret;
        this->hand->turret = new SummonersOrder("so_hand" + std::to_string(turret_objects.size() + 1), TurretColor::BLUE);
        keys[Turrets::SUMMONERS_ORDER] = GL_FALSE;
        printf("[GAME] Selected a SummonersOrder turret\n");
    }
}

// Check if we should place the turret on the floor
void Game::check_place_turret()
{
    // If there is a turret that can be placed, and the user clicks, place it
    if (keys[GLFW_MOUSE_BUTTON_1] == GL_TRUE && this->hand->turret != nullptr && this->hand->turret->can_place())
    {
        collisive_objects.insert(std::map<std::string, Collisive *>::value_type("turret_" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        turret_objects.insert(std::map<std::string, Turret *>::value_type("turret_" + std::to_string(turret_objects.size() + 1), this->hand->turret));
        this->hand->turret->placed = true;          // Mark as placed
        Game::gold -= this->hand->turret->price;    // Pay for the turret
        this->hand->turret = nullptr;               // Updated it
        keys[GLFW_MOUSE_BUTTON_1] = GL_FALSE;       // Remove the flag

        printf("[GAME] Turret current holded on hand created\n");
        printf("[GAME] Current player balance: %.2f\n", this->gold);
    }
    else if (keys[GLFW_MOUSE_BUTTON_2] == GL_TRUE && this->hand->turret != nullptr)     // If uses right button, deselects turret
    {
        delete this->hand->turret;
        this->hand->turret = nullptr;
        printf("[GAME] Deselected any turret\n");
    }
}

std::vector<std::vector<std::vector<Enemy *> > > Game::generate_enemy_hordes()
{
    std::vector<std::vector<std::vector<Enemy *> > > enemies =
    {
        // HORDE 1 - 390 HP
        {
            {
                new ChaosMelee("h1_mid_m1", Route::MID)
            },
            {
                new ChaosRanged("h1_mid_r1", Route::MID)
            },
            {
                new ChaosMelee("h1_top_m1", Route::TOP), new ChaosMelee("h1_bottom_m1", Route::BOTTOM)
            },
            {
                new ChaosRanged("h1_top_r1", Route::TOP), new ChaosRanged("h1_bottom_r1", Route::BOTTOM)
            }
        },
        // HORDE 2 - 520 HP
        {
            {
                new ChaosMelee("h2_mid_m1", Route::MID)
            },
            {
                new ChaosMelee("h2_mid_m2", Route::MID), new ChaosMelee("h2_top_m1", Route::TOP), new ChaosMelee("h2_bottom_m1", Route::BOTTOM)
            },
            {
                new ChaosRanged("h2_mid_r1", Route::MID)
            },
            {
                new ChaosRanged("h2_mid_r2", Route::MID), new ChaosRanged("h2_top_r1", Route::TOP), new ChaosRanged("h2_bottom_r1", Route::BOTTOM)
            },
        },
        // HORDE 3 - 670 HP
        {
            {
                new ChaosSiege("h3_mid_sg1", Route::MID)
            },
            {
                new ChaosMelee("h3_mid_m1", Route::MID), new ChaosRanged("h3_top_r1", Route::TOP), new ChaosRanged("h3_bottom_r1", Route::BOTTOM)
            },
            {
                new ChaosMelee("h3_mid_m2", Route::MID), new ChaosRanged("h3_top_r2", Route::TOP), new ChaosRanged("h3_bottom_r2", Route::BOTTOM)
            },
            {
                new ChaosRanged("h3_mid_r1", Route::MID), new ChaosMelee("h3_top_m1", Route::TOP), new ChaosMelee("h3_bottom_m1", Route::BOTTOM)
            }
        },
        // HORDE 4 - 910HP
        {
            {
                new ChaosMelee("h4_mid_m1", Route::MID), new ChaosMelee("h4_top_m1", Route::TOP), new ChaosMelee("h4_mid_m1", Route::TOP)
            },
            {
                new ChaosMelee("h4_mid_m2", Route::MID), new ChaosMelee("h4_top_m2", Route::TOP), new ChaosMelee("h4_mid_m2", Route::TOP)
            },
            {
                new ChaosMelee("h4_mid_m3", Route::MID)
            },
            {

            },
            {
                new ChaosRanged("h4_mid_r1", Route::MID), new ChaosRanged("h4_top_r1", Route::TOP), new ChaosRanged("h4_mid_r1", Route::TOP)
            },
            {
                new ChaosRanged("h4_mid_r2", Route::MID), new ChaosRanged("h4_top_r2", Route::TOP), new ChaosRanged("h4_mid_r2", Route::TOP)
            },
            {
                new ChaosRanged("h4_mid_r3", Route::MID)
            }
        },
        // HORDE 5 - 1140HP
        {
            {
                new ChaosSiege("h5_mid_sg1", Route::MID), new ChaosSiege("h5_top_sg1", Route::TOP), new ChaosSiege("h5_bottom_sg1", Route::BOTTOM),
            },
            {
                new ChaosRanged("h5_top_r1", Route::TOP), new ChaosRanged("h5_bottom_r1", Route::BOTTOM)
            },
            {
                new ChaosRanged("h5_top_r2", Route::TOP), new ChaosRanged("h5_bottom_r2", Route::BOTTOM)
            },
            {
                new ChaosSiege("h5_mid_sg2", Route::MID), new ChaosRanged("h5_top_r3", Route::TOP), new ChaosRanged("h5_bottom_r3", Route::BOTTOM)
            },
            {
                new ChaosMelee("h5_mid_m1", Route::MID), new ChaosMelee("h5_top_m1", Route::TOP), new ChaosMelee("h5_bottom_m1", Route::BOTTOM)
            }
        },
        // HORDE 6 - 1240HP
        {
            {
                new ChaosSiege("h6_top_sg1", Route::TOP), new ChaosSiege("h6_bottom_sg1", Route::BOTTOM)
            },
            {
                new ChaosMelee("h6_top_m1", Route::TOP), new ChaosMelee("h6_bottom_m1", Route::BOTTOM)
            },
            {
                new ChaosMelee("h6_top_m2", Route::TOP), new ChaosMelee("h6_bottom_m2", Route::BOTTOM)
            },
            {
                new ChaosMelee("h6_top_m3", Route::TOP), new ChaosMelee("h6_bottom_m3", Route::BOTTOM)
            },
            {
                new ChaosMelee("h6_top_m4", Route::TOP), new ChaosMelee("h6_bottom_m4", Route::BOTTOM)
            },
            {
                new ChaosSiege("h6_top_sg2", Route::TOP), new ChaosSiege("h6_bottom_sg2", Route::BOTTOM)
            }
        },
        // HORDE 7 - 1725HP
        {
            {
                new ChaosMelee("h7_top_m1", Route::TOP), new ChaosMelee("h7_bottom_m1", Route::BOTTOM)
            },
            {
                new ChaosMelee("h7_mid_m1", Route::MID), new ChaosRanged("h7_top_r1", Route::TOP), new ChaosRanged("h7_bottom_r1", Route::BOTTOM)
            },
            {
                new ChaosMelee("h7_mid_m2", Route::MID), new ChaosRanged("h7_top_r2", Route::TOP), new ChaosRanged("h7_bottom_r2", Route::BOTTOM)
            },
            {
                new ChaosSuper("h7_mid_sp1", Route::MID), new ChaosRanged("h7_top_r3", Route::TOP), new ChaosRanged("h7_bottom_r3", Route::BOTTOM)
            },
            {
                new ChaosMelee("h7_mid_m3", Route::MID), new ChaosMelee("h7_top_m2", Route::TOP), new ChaosMelee("h7_bottom_m2", Route::BOTTOM)
            },
            {
                new ChaosMelee("h7_mid_m4", Route::MID), new ChaosMelee("h7_top_m3", Route::TOP), new ChaosMelee("h7_bottom_m3", Route::BOTTOM)
            }
        },
        // HORDE 8 - 1915HP
        {
            {
                new ChaosSiege("h8_mid_sg1", Route::MID), new ChaosSiege("h8_top_sg1", Route::TOP), new ChaosSiege("h8_bottom_sg1", Route::BOTTOM)
            },
            {
                new ChaosMelee("h8_mid_m1", Route::MID), new ChaosMelee("h8_top_m1", Route::TOP), new ChaosMelee("h8_bottom_m1", Route::BOTTOM)
            },
            {
                new ChaosRanged("h8_mid_r1", Route::MID), new ChaosRanged("h8_top_r1", Route::TOP), new ChaosRanged("h8_bottom_r1", Route::BOTTOM)
            },
            {
                new ChaosSuper("h8_mid_sp1", Route::MID), new ChaosSuper("h8_top_sp1", Route::TOP), new ChaosSuper("h8_bottom_sp1", Route::BOTTOM)
            },
            {
                new ChaosRanged("h8_mid_r2", Route::MID)
            },
            {
                new ChaosRanged("h8_mid_r3", Route::MID)
            }
        },
        // HORDE 9 - 2675HP
        {
            {
                new ChaosSiege("h9_mid_sg1", Route::MID)
            },
            {

            },
            {
                new ChaosSiege("h9_mid_sg2", Route::MID), new ChaosSiege("h9_top_sg1", Route::TOP), new ChaosSiege("h9_bottom_sg1", Route::BOTTOM)
            },
            {
                new ChaosSiege("h9_mid_sg3", Route::MID), new ChaosSiege("h9_top_sg2", Route::TOP), new ChaosSiege("h9_bottom_sg2", Route::BOTTOM)
            },
            {
                new ChaosSuper("h9_top_sp1", Route::TOP), new ChaosSuper("h9_bottom_sp1", Route::BOTTOM)
            },
            {

            },
            {
                new ChaosSuper("h9_mid_sp1", Route::MID), new ChaosSuper("h9_top_sp2", Route::TOP), new ChaosSuper("h9_bottom_sp2", Route::BOTTOM)
            }
        },
        // HORDE 10 - 3900HP
        {
            {
                new ChaosSuper("h10_mid_sp1", Route::MID)
            },
            {
                new ChaosSuper("h10_mid_sp2", Route::MID)
            },
            {
                new ChaosSuper("h10_mid_sp3", Route::MID)
            },
            {
                new ChaosSuper("h10_mid_sp4", Route::MID), new ChaosSuper("h10_top_sp1", Route::TOP), new ChaosSuper("h10_bottom_sp1", Route::BOTTOM)
            },
            {
                new ChaosSuper("h10_mid_sp5", Route::MID), new ChaosSuper("h10_top_sp2", Route::TOP), new ChaosSuper("h10_bottom_sp2", Route::BOTTOM)
            },
            {
                new ChaosSuper("h10_mid_sp6", Route::MID), new ChaosSuper("h10_top_sp3", Route::TOP), new ChaosSuper("h10_bottom_sp3", Route::BOTTOM)
            }
        }
    };


    return enemies;
}
