#include "model\base.hpp"

class Bunny : public GameObject
{
public:
    Bunny(std::string name) : GameObject(name)
    {
        this->build();
    };

    Bunny(std::string name, glm::vec4 position) : GameObject(name, position)
    {
        this->build();
    };

    Bunny(std::string name, glm::vec4 position, glm::vec4 orientation, float angle) :
            GameObject(name, position, orientation, angle)
    {
        this->build();
    };

    void build()
    {
        // Initialize shaders
        this->shader = ResourceManager::load_shader("../../src/shaders/bunny.vs", "../../src/shaders/bunny.fs", nullptr, this->name);

        // Configure the VAO, and bind it
        glGenVertexArrays(1, &this->VAO); // Set at the object level
        glBindVertexArray(this->VAO);

        // Initialize model, loaded from a .obj file
        ObjModel* model = new ObjModel("../../src/objects/bunny.obj");

        // Temporary structures to hold the object information
        std::vector<GLuint> indexes;
        std::vector<float>  model_coefficients;
        std::vector<float>  normal_coefficients;
        std::vector<float>  texture_coefficients;

        // TODO:
        // We assume that we have only one shape, if not we would need to iterate and store them in some object
        // and, after, render each of them individually, as well.
        // I didn't coded it, because I don't know if we will have any object with more than one shape
        // so for sake of simplicity, it only works this way, right now
        //
        // If we need to add this, we might compare the code below with the Lab4 one

        // TODO 2:
        // Move to a different function, which receveis indexes, model_coefficients,
        // normal_coefficients and texture_coefficients as parameters
        size_t num_triangles = model->shapes[0].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[0].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[0].mesh.indices[3*triangle + vertex];

                indexes.push_back(3*triangle + vertex);

                model_coefficients.push_back( model->attrib.vertices[3*idx.vertex_index + 0] ); // X
                model_coefficients.push_back( model->attrib.vertices[3*idx.vertex_index + 1] ); // Y
                model_coefficients.push_back( model->attrib.vertices[3*idx.vertex_index + 2] ); // Z
                model_coefficients.push_back( 1.0f ); // W

                // We check if there are normals in the object, if there are, we save them
                if ( idx.normal_index != -1 )
                {
                    normal_coefficients.push_back( model->attrib.normals[3*idx.normal_index + 0] ); // X
                    normal_coefficients.push_back( model->attrib.normals[3*idx.normal_index + 1] ); // Y
                    normal_coefficients.push_back( model->attrib.normals[3*idx.normal_index + 2] ); // Z
                    normal_coefficients.push_back( 0.0f ); // W
                }

                // We check if there are textures in the object, if there are, we save them
                if ( idx.texcoord_index != -1 )
                {
                    texture_coefficients.push_back( model->attrib.texcoords[2*idx.texcoord_index + 0] ); // U
                    texture_coefficients.push_back( model->attrib.texcoords[2*idx.texcoord_index + 1] ); // V
                }
            }
        }

        // VBO for the vertex
        GLuint VBO_model_coefficients_id;
        glGenBuffers(1, &VBO_model_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), model_coefficients.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); // Location = 0, vec4
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // VBO for the normals
        if ( !normal_coefficients.empty() )
        {
            GLuint VBO_normal_coefficients_id;
            glGenBuffers(1, &VBO_normal_coefficients_id);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
            glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), normal_coefficients.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0); // Location = 1, vec4
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // VBO for the texture
        if ( !texture_coefficients.empty() )
        {
            GLuint VBO_texture_coefficients_id;
            glGenBuffers(1, &VBO_texture_coefficients_id);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
            glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), texture_coefficients.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // Location = 2, vec2
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // We create an OpenGL buffer to store the indexes
        // We "turn on" the buffer. Notice that we use GL_ELEMENT_ARRAY_BUFFER now.
        // We allocate memory to the buffer, and copy the data to it.
        // Also notice, that we don't turn off the buffer
        GLuint indexes_id;
        glGenBuffers(1, &indexes_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLuint), indexes.data(), GL_STATIC_DRAW);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Wrong!

        // Drawing methods definition
        this->drawMode = GL_TRIANGLES;
        this->indexesLength = indexes.size();
        this->indexesOffset = 0;

        return;
    }

    void render(glm::mat4 view, glm::mat4 projection)
    {

        // Set to use this shader
        this->shader.use();

        // Configure view and projection matrices
        this->shader.set_matrix("view", view);
        this->shader.set_matrix("projection", projection);

        // Bind the VAO
        glBindVertexArray(this->VAO);

        // Calculate the model matrix
        glm::mat4 model = matrix::identity_matrix(); // make sure to initialize matrix to identity matrix first
        model *= matrix::translate_matrix(this->position);
        model *= matrix::rotate_matrix(this->angle, this->orientation);
        this->shader.set_matrix("model", model);

        // Draw the element
        glDrawElements(this->drawMode, this->indexesLength, GL_UNSIGNED_INT, this->indexesOffset);

        // Unbind the VAO to prevent bugs
        glBindVertexArray(0);

    }
};
