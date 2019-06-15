#include "engine/resource_manager.hpp"
#include "engine/matrices.hpp"
#include "model/skybox.hpp"

void SkyBox::build()
{
    // Initialize shaders
    this->shader = ResourceManager::load_shader("../../src/shaders/skybox.vs", "../../src/shaders/skybox.fs", nullptr, this->name);

    std::vector<std::string> faces{
        "../../src/textures/skybox_right.jpg",
        "../../src/textures/skybox_left.jpg",
        "../../src/textures/skybox_top.jpg",
        "../../src/textures/skybox_bottom.jpg",
        "../../src/textures/skybox_front.jpg",
        "../../src/textures/skybox_back.jpg"};
    this->texture = ResourceManager::load_cubemap_texture(faces, "skybox");
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
    this->texture.bind();

    // Draw the element
    glDrawArrays(this->drawMode, 0, 36);

    // Unbind the VAO to prevent bugs
    glBindVertexArray(0);

    // Set depth function back to default
    glDepthFunc(GL_LESS);
}
