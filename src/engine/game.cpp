#include "engine/game.hpp"
#include "engine/matrices.hpp"
#include "engine/resource_manager.hpp"
#include "model/base.hpp"
#include "model/cube.hpp"
#include "model/bunny.hpp"
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

    // World space positions of our cubes
    glm::vec3 cubePositions[] =
    {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    /* GAME OBJECTS CREATION */
    for (int i = 0; i < 10; i++){
        Cube* cube = new Cube("cube", glm::vec4(cubePositions[i], 1.0f), glm::vec4(1.0f, 0.4f, 2.14f, 0.0f), i * 22.5f);

        /* GAME OBJECTS INSERTION TO THE GAME OBJECTS MAP */
         // We might have some error here as we are adding the index to the string in a bad way,
         // so sometimes we have some overflow, and some blocks don't render properly,
         // but we just need a flush after it
        objects.insert(std::map<std::string, GameObject*>::value_type ("cube" + i, cube));
        printf("[GAME] Cube %d created\n", i);
    }

    Bunny* bunny = new Bunny("bunny", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    objects.insert(std::map<std::string, GameObject*>::value_type ("bunny", bunny));
    printf("[GAME] Bunny created\n");
}

void Game::new_frame(){
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
}

void Game::update()
{
    y_speed = utils::clamping(y_speed + GRAVITY * this->deltaTime, 100.0f, MAX_SPEED);

    camera.position.y = utils::clamping(camera.position.y + y_speed * this->deltaTime, 100.0f, 0.0f);
}

void Game::process_input()
{
    // Movement
    if (keys[GLFW_KEY_W] == GL_TRUE)
        camera.process_keyboard(FORWARD, this->deltaTime);
    if (keys[GLFW_KEY_S] == GL_TRUE)
        camera.process_keyboard(BACKWARD, this->deltaTime);
    if (keys[GLFW_KEY_A] == GL_TRUE)
        camera.process_keyboard(LEFT, this->deltaTime);
    if (keys[GLFW_KEY_D] == GL_TRUE)
        camera.process_keyboard(RIGHT, this->deltaTime);

    // Jump
    if (keys[GLFW_KEY_SPACE] == GL_TRUE && camera.position.y <= 0.0f)
        y_speed = JUMP_SPEED;
}

void Game::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Calculate the view and projection matrices */
    glm::mat4 view = this->camera.get_view_matrix();
    glm::mat4 projection = matrix::perspective_matrix(glm::radians(this->camera.zoom), screen_ratio, -0.1f, -100.0f);

    // render boxes
    for (const auto& object : this->objects)
    {
        object.second->render(view, projection);
    }

}
