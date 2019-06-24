// Headers das bibliotecas OpenGL
#include <glad/glad.h>  // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h> // Criação de janelas do sistema operacional

// Headers da \engine
#include "engine\resource_manager.hpp"
#include "engine\matrices.hpp"
#include "engine\camera.hpp"
#include "engine\game.hpp"

// Headers da \utils
#include "utils\helpers.hpp"
#include "utils.h"

// Header do resource
#include "resources.h"

// Activate NVIDIA off-board GPU, only if needed
// extern "C"
// {
//    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// }

// GLFW function declarations
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// Global Constants
const GLuint SCREEN_WIDTH = 1920;       // Width of the screen
const GLuint SCREEN_HEIGHT = 1080;      // Height of the screen
const GLboolean FULLSCREEN = false;     // Should we render fullscreen or not
const GLboolean ANTIALIASING = true;    // Should we use antialiasing or not
const GLuint ANTIALIASING_QUALITY = 4;  // Antialiasing level, if ANTIALIASING is set to true

using namespace std;

Game MinionsHaveSpawned = Game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pedimos para utilizar mais samplers para termos MSAA, se utilizado
    if (ANTIALIASING)
        glfwWindowHint(GLFW_SAMPLES, ANTIALIASING_QUALITY);

    // Criamos uma janela do sistema operacional, com o tamanho pré-definido acima
    GLFWwindow *window;
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minions Have Spawned - FCG 2019/1", FULLSCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Configure window icon
    GLFWimage images[1];
    images[0].pixels = stbi_load("../../assets/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, key_callback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, mouse_callback);
    // ... ou rolar a "rodinha" do mouse ...
    glfwSetScrollCallback(window, scroll_callback);
    // ... ou clicar os botões do mouse.
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT); // Forçamos a chamada do callback acima, para definir os valores de resolução do jogo.

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: Failed to initialize GLAD.\n");
        std::exit(EXIT_FAILURE);
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);                            // Depth testing
    glEnable(GL_MULTISAMPLE);                           // Antialiasing multisample
    glEnable(GL_BLEND);                                 // Enable color blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Configuring blend function
    glEnable(GL_CULL_FACE);                             // Backface culling
    glCullFace(GL_BACK);                                // Configuring culling to back
    glFrontFace(GL_CCW);                                // Configuring front face with counter-clockwise orientation


    // Imprimimos no terminal informações sobre a GPU do sistema
    utils::print_gpu_info();

    // Initialize game
    MinionsHaveSpawned.init();

    // Game main loop
    while (!glfwWindowShouldClose(window))
    {
        // Tell the game we are in a new frame
        MinionsHaveSpawned.new_frame();

        // Manage user input
        MinionsHaveSpawned.process_input();

        // Update Game state
        MinionsHaveSpawned.update();

        // Call render procedure
        MinionsHaveSpawned.render();

        // Swap the buffers (actually render the image in the window) and fetch window events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete all resources loaded using the resource manager
    ResourceManager::clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // When a user presses TAB, should change camera type (if possible)
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        Game::camera.switch_camera_type();

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            MinionsHaveSpawned.keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            MinionsHaveSpawned.keys[key] = GL_FALSE;
    }

}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula (slides 33-44 do documento "Aula_07_Transformacoes_Geometricas_3D.pdf").
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slide 227 do documento "Aula_09_Projecoes.pdf".
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    MinionsHaveSpawned.height = height;
    MinionsHaveSpawned.width = width;
    MinionsHaveSpawned.screen_ratio = (float)width / height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX = SCREEN_WIDTH / 2.0f;
    static float lastY = SCREEN_HEIGHT / 2.0f;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    MinionsHaveSpawned.camera.process_mouse_movement(xoffset, yoffset);
}

// glfw: whenever the mouse clicks, this callback is called
// --------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
        MinionsHaveSpawned.keys[button] = GL_TRUE;
    } else if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_RELEASE) {
        MinionsHaveSpawned.keys[button] = GL_FALSE;
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    MinionsHaveSpawned.camera.process_mouse_scroll(yoffset);
}
