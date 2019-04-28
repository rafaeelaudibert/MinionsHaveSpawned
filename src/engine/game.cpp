#include "engine/game.hpp"
#include "engine/matrices.hpp"
#include "engine/resource_manager.hpp"
#include "model/base.hpp"
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
    /* SHADERS CONFIGURATION */

    // Initialize shaders
    ResourceManager::load_shader("../../src/cubes.vs", "../../src/cubes.fs", nullptr, "cubes");

    GLfloat vertexes[] =
    {
        //    X      Y     Z     W
        -0.5f, 0.5f, 0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f,
    };

    // VBO and VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint indexes[] =
    {
        // We define de vertex indexes which define the cube faces using
        // 12 triangles which will be drawn with the GL_TRIANGLES renderization technique
        0, 1, 2,
        7, 6, 5,
        3, 2, 6,
        4, 0, 3,
        4, 5, 1,
        1, 5, 6,
        0, 2, 3,
        7, 5, 4,
        3, 6, 7,
        4, 3, 7,
        4, 1, 0,
        1, 6, 2,
    };

    // We create an OpenGL bugger to store the indexes above
    // We "turn on" the buffer. Notice that we use GL_ELEMENT_ARRAY_BUFFER now.
    // We allocate memory to the buffer, and copy the data to it.
    GLuint indexes_id;
    glGenBuffers(1, &indexes_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    objects.insert(std::map<std::string, GameObject>::value_type ("cubes", GameObject("cubes", VAO)));
    ResourceManager::get_shader("cubes").use();
}

void Game::update(GLfloat dt)
{
    y_speed = utils::clamping(y_speed + GRAVITY * dt, 100.0f, MAX_SPEED);

    camera.position.y = utils::clamping(camera.position.y + y_speed * dt, 100.0f, 0.0f);
}

void Game::process_input(GLfloat dt)
{
    // Movement
    if (keys[GLFW_KEY_W] == GL_TRUE)
        camera.process_keyboard(FORWARD, dt);
    if (keys[GLFW_KEY_S] == GL_TRUE)
        camera.process_keyboard(BACKWARD, dt);
    if (keys[GLFW_KEY_A] == GL_TRUE)
        camera.process_keyboard(LEFT, dt);
    if (keys[GLFW_KEY_D] == GL_TRUE)
        camera.process_keyboard(RIGHT, dt);

    // Jump
    if (keys[GLFW_KEY_SPACE] == GL_TRUE && camera.position.y <= 0.0f)
        y_speed = JUMP_SPEED;
}

void Game::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // world space positions of our cubes
    glm::vec3 cubePositions[] =
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
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

    // camera/view transformation
    glm::mat4 view = camera.get_view_matrix();
    ResourceManager::get_shader("cubes").set_matrix("view", view);

    // Perspective projection transformation
    glm::mat4 projection = matrix::perspective_matrix(glm::radians(camera.zoom), screen_ratio, -0.1f, -100.0f);
    ResourceManager::get_shader("cubes").set_matrix("projection", projection);

    // render boxes
    glBindVertexArray(objects.find("cubes")->second.VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
        model *= matrix::translate_matrix(cubePositions[i]);
        model *= matrix::rotate_matrix(i * glfwGetTime(), glm::vec4(1.0f, 0.3f, 0.5f, 0.0f));
        ResourceManager::get_shader("cubes").set_matrix("model", model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

}
