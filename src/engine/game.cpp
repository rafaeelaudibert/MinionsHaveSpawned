#include "engine/game.hpp"
#include "engine/matrices.hpp"
#include "engine/resource_manager.hpp"
#include "model/base.hpp"
#include "model/cube.hpp"
#include "model/bunny.hpp"
#include "model/dummy.hpp"
#include "model/skybox.hpp"
#include "utils/helpers.hpp"

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

    Bunny* bunny = new Bunny("bunny", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject*>::value_type ("bunny", bunny));
    printf("[GAME] Bunny created\n");

    SkyBox* skybox = new SkyBox("skybox");
    objects.insert(std::map<std::string, GameObject*>::value_type ("skybox", skybox));
    printf("[GAME] Skybox created\n");

    Cube* plane = new Cube("plane", glm::vec4(0.0f, -0.005f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(200.0f, 0.01f, 200.0f));
    objects.insert(std::map<std::string, GameObject*>::value_type ("plane", plane));
    printf("[GAME] Plane created\n");

    Dummy* dummy = new Dummy("dummy", glm::vec4(0.0f, 3.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), 0, glm::vec3(4.0f, 4.0f, 4.0f));
    objects.insert(std::map<std::string, GameObject*>::value_type ("dummy", dummy));
    printf("[GAME] Dummy created\n");
}

void Game::new_frame()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Game::update()
{
    // Update falling speed
    y_speed = utils::clamping(y_speed + GRAVITY * this->deltaTime, 100.0f, MAX_SPEED);

    // Restrict camera position
    camera.position.y = utils::clamping(camera.position.y + y_speed * this->deltaTime, MAX_HEIGHT, player_status == STANDING ? CHARACTER_HEIGHT : CHARACTER_CROUCHING_HEIGHT);
}

void Game::process_input()
{
    // Movement
    if (keys[GLFW_KEY_W] == GL_TRUE)
        camera.process_keyboard(FORWARD, this->deltaTime * (player_status == STANDING ? 1 : CROUCHING_SPEED_MULTIPLIER));
    if (keys[GLFW_KEY_S] == GL_TRUE)
        camera.process_keyboard(BACKWARD, this->deltaTime * (player_status == STANDING ? 1 : CROUCHING_SPEED_MULTIPLIER));
    if (keys[GLFW_KEY_A] == GL_TRUE)
        camera.process_keyboard(LEFT, this->deltaTime * (player_status == STANDING ? 1 : CROUCHING_SPEED_MULTIPLIER));
    if (keys[GLFW_KEY_D] == GL_TRUE)
        camera.process_keyboard(RIGHT, this->deltaTime * (player_status == STANDING ? 1 : CROUCHING_SPEED_MULTIPLIER));

    // Stopped uncrouching
    if (camera.position.y >= CHARACTER_HEIGHT && player_status == UNCROUCHING) {
        player_status = STANDING;
        camera.position.y = CHARACTER_HEIGHT;
        y_speed = MAX_SPEED;
        printf("[GAME] Back to standing\n");
    }

    // Stopped jumping
    if (camera.position.y <= CHARACTER_HEIGHT && player_status == JUMPING) {
        player_status = STANDING;
        camera.position.y = CHARACTER_HEIGHT;
        printf("[GAME] Back to standing\n");
    }

    // Jump
    if (keys[GLFW_KEY_SPACE] == GL_TRUE && player_status == STANDING) {
        y_speed = JUMP_SPEED;
        player_status = JUMPING;
        printf("[GAME] Started jumping\n");
    }

    // Crouching
    if (keys[GLFW_KEY_LEFT_SHIFT] == GL_TRUE && player_status == STANDING) {
        y_speed = CROUCHING_SPEED;
        player_status = CROUCHING;
        printf("[GAME] Started crouching\n");
    }

    // Uncrouching
    if (keys[GLFW_KEY_LEFT_SHIFT] == GL_FALSE && player_status == CROUCHING) {
        y_speed = 2 * JUMP_SPEED;
        player_status = UNCROUCHING;
        printf("[GAME] Started uncrouching\n");
    }

}

void Game::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Calculate the view and projection matrices */
    glm::mat4 view = this->camera.get_view_matrix();
    glm::mat4 projection = matrix::perspective_matrix(glm::radians(this->camera.zoom), screen_ratio, -0.1f, -100.0f);

    // Render objects
    for (const auto& object : this->objects)
    {
        object.second->render(view, projection);
    }

}
