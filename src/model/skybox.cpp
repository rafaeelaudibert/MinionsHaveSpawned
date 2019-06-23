#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/skybox.hpp"
#include "engine/game.hpp"

void SkyBox::build()
{
    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/skybox.vs", "../../src/shaders/skybox.fs", nullptr, this->name);

    std::vector<std::string> faces_normal {
        "../../src/textures/skybox_right.jpg",
        "../../src/textures/skybox_left.jpg",
        "../../src/textures/skybox_top.jpg",
        "../../src/textures/skybox_bottom.jpg",
        "../../src/textures/skybox_front.jpg",
        "../../src/textures/skybox_back.jpg"};
    this->texture = ResourceManager::load_cubemap_texture(faces_normal, "skybox");

    std::vector<std::string> faces_victory {
        "../../src/textures/skybox_right_victory.jpg",
        "../../src/textures/skybox_left_victory.jpg",
        "../../src/textures/skybox_top_victory.jpg",
        "../../src/textures/skybox_bottom_victory.jpg",
        "../../src/textures/skybox_front_victory.jpg",
        "../../src/textures/skybox_back_victory.jpg"};
    this->texture_victory = ResourceManager::load_cubemap_texture(faces_victory, "skybox_victory");

    std::vector<std::string> faces_defeat {
        "../../src/textures/skybox_right_defeat.jpg",
        "../../src/textures/skybox_left_defeat.jpg",
        "../../src/textures/skybox_top_defeat.jpg",
        "../../src/textures/skybox_bottom_defeat.jpg",
        "../../src/textures/skybox_front_defeat.jpg",
        "../../src/textures/skybox_back_defeat.jpg"};
    this->texture_defeat = ResourceManager::load_cubemap_texture(faces_defeat, "skybox_defeat");


    this->shader.use();
    this->shader.set("skybox", 0); // Not required

    // Vertex definition
    float vertexes[] =
        {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f};

    // VBO and VAO
    unsigned int VBO; // VAO is declared in the object
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), &vertexes, GL_STATIC_DRAW);

    // Vertexes are in ==> location = 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Drawing methods definition
    this->drawMode = GL_TRIANGLES;
    this->indexesLength = sizeof(vertexes) / sizeof(float) / 3;
    this->indexesOffset = 0;

    return;
}

void SkyBox::render(glm::mat4 view, glm::mat4 projection)
{
    // Change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);

    // Set to use this shader
    this->shader.use();

    // Configure view and projection matrices
    this->shader.set_matrix("view", glm::mat4(glm::mat3(view))); // remove translation from the view matrix
    this->shader.set_matrix("projection", projection);

    // Bind the VAO
    glBindVertexArray(this->VAO);

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    switch (Game::state) {
    case GameState::GAME_ACTIVE:
    case GameState::GAME_WAIT:
        this->texture.bind();
        break;
    case GameState::GAME_WIN:
        this->texture_victory.bind();
        break;
    case GameState::GAME_LOSE:
        this->texture_defeat.bind();
        break;
    default:
        throw std::runtime_error("[ERROR] Error when trying to load the Skybox texture - Non existent GameState.");
    }

    // Draw the element
    glDrawArrays(this->drawMode, 0, 36);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);

    // Set depth function back to default
    glDepthFunc(GL_LESS);
}
