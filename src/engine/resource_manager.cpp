#include "engine/resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

// Texture library
#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, CubemapTexture2D> ResourceManager::cubemapTextures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::load_shader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
    shaders[name] = load_shader_from_file(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}

Shader ResourceManager::get_shader(std::string name)
{
    return shaders[name];
}



Texture2D ResourceManager::load_texture(const GLchar *file, GLboolean alpha, std::string name)
{
    textures[name] = load_texture_from_file(file, alpha);
    return textures[name];
}

Texture2D ResourceManager::get_texture(std::string name)
{
    return textures[name];
}

CubemapTexture2D ResourceManager::load_cubemap_texture(std::vector<std::string> files, std::string name)
{
    cubemapTextures[name] = load_cubemap_texture_from_files(files);
    return cubemapTextures[name];
}

CubemapTexture2D ResourceManager::get_cubemap_texture(std::string name)
{
    return cubemapTextures[name];
}

ObjModel* ResourceManager::load_object(const char* filename, GameObject* gameObject, std::string name)
{
    // Configure the VAO, and bind it
    glGenVertexArrays(1, &gameObject->VAO); // Set at the object level
    glBindVertexArray(gameObject->VAO);

    // Initialize model, loaded from a .obj file
    ObjModel* model = new ObjModel(filename);
    ResourceManager::compute_normals(model); // If the model doesn't have normals in the .obj file, we compute them manually

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
    printf("[GAME] Fetched model attributes for %s from file\n", name.c_str());

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
        printf("[GAME] Fetched normal attributes for %s from file\n", name.c_str());
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
        printf("[GAME] Fetched texture attributes for %s from file\n", name.c_str());
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
    gameObject->drawMode = GL_TRIANGLES;
    gameObject->indexesLength = indexes.size();
    gameObject->indexesOffset = 0;

    return model;
}

Shader ResourceManager::load_shader_from_file(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();

    // 2. Now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const GLchar *filename, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;

    printf("Loading image \"%s\"... ", filename);

    if (alpha)
    {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);

    if ( image == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Now generate texture
    texture.generate(width, height, image);

    // And finally free image
    stbi_image_free(image);

    return texture;
}

CubemapTexture2D ResourceManager::load_cubemap_texture_from_files(std::vector<std::string> faces)
{
    CubemapTexture2D texture;

    texture.generate(faces);

    return texture;
}

void ResourceManager::compute_normals(ObjModel* model)
{
    if ( !model->attrib.normals.empty() )
        return;

    // Primeiro computamos as normais para todos os TRI�NGULOS.
    // Segundo, computamos as normais dos V�RTICES atrav�s do m�todo proposto
    // por Gourad, onde a normal de cada v�rtice vai ser a m�dia das normais de
    // todas as faces que compartilham este v�rtice.

    size_t num_vertices = model->attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4  vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
            }

            const glm::vec4  a = vertices[0];
            const glm::vec4  b = vertices[1];
            const glm::vec4  c = vertices[2];

            const glm::vec4  n = matrix::crossproduct(b-a,c-a);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                model->shapes[shape].mesh.indices[3*triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    model->attrib.normals.resize( 3*num_vertices );

    for (size_t i = 0; i < vertex_normals.size(); ++i)
    {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= matrix::norm(n);
        model->attrib.normals[3*i + 0] = n.x;
        model->attrib.normals[3*i + 1] = n.y;
        model->attrib.normals[3*i + 2] = n.z;
    }

    return;
}

void ResourceManager::clear()
{
    // (Properly) delete all shaders
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);

    // (Properly) delete all textures
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.ID);
}
