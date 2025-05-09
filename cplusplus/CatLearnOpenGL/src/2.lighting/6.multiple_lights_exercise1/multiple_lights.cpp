﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <vector>

enum EXERCISE_MODE {
    EXERCISE_DESERT,
    EXERCISE_FACTORY,
    EXERCISE_HORROR,
    EXERCISE_BIOCHEMICAL_LAB
};

EXERCISE_MODE exercise_mode = EXERCISE_BIOCHEMICAL_LAB;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // 当前帧和上一帧之间的时间
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CatLearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 告诉GLFW捕获我们的鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: 加载所有 OpenGL 函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 构建并编译着色器z程序
    // ------------------------------------
    Shader lightingShader("6.multiple_lights.vs", "6.multiple_lights.fs");
    Shader lightCubeShader("6.light_cube.vs", "6.light_cube.fs");

        // 设置顶点数据（和缓冲区）并配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // 放置所有集装箱
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    glm::vec3 pointLightColors[4];

    switch (exercise_mode) {
    case EXERCISE_DESERT:
        pointLightColors[0] = glm::vec3(1.0f, 0.6f, 0.0f);
        pointLightColors[1] = glm::vec3(1.0f, 0.0f, 0.0f);
        pointLightColors[2] = glm::vec3(1.0f, 1.0, 0.0);
        pointLightColors[3] = glm::vec3(0.2f, 0.2f, 1.0f);
        break;
    case EXERCISE_FACTORY:
        pointLightColors[0] = glm::vec3(0.2f, 0.2f, 0.6f);
        pointLightColors[1] = glm::vec3(0.3f, 0.3f, 0.7f);
        pointLightColors[2] = glm::vec3(0.0f, 0.0f, 0.3f);
        pointLightColors[3] = glm::vec3(0.4f, 0.4f, 0.4f);
        break;
    case EXERCISE_HORROR:
        pointLightColors[0] = glm::vec3(0.1f, 0.1f, 0.1f);
        pointLightColors[1] = glm::vec3(0.1f, 0.1f, 0.1f);
        pointLightColors[2] = glm::vec3(0.1f, 0.1f, 0.1f);
        pointLightColors[3] = glm::vec3(0.3f, 0.1f, 0.1f);
        break;
    case EXERCISE_BIOCHEMICAL_LAB:
        pointLightColors[0] = glm::vec3(0.4f, 0.7f, 0.1f);
        pointLightColors[1] = glm::vec3(0.4f, 0.7f, 0.1f);
        pointLightColors[2] = glm::vec3(0.4f, 0.7f, 0.1f);
        pointLightColors[3] = glm::vec3(0.4f, 0.7f, 0.1f);
        break;
    default:
        break;
    }

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindVertexArray(cubeVAO);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
        // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 第二，配置灯光的VAO（VBO保持不变；对于同样是3D立方体的灯光对象，顶点是相同的）
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 请注意，我们更新了灯的位置属性的步幅，以反映更新的缓冲区数据

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
    unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
    
    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

        // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // render
        // ------
        switch (exercise_mode)
        {
        case EXERCISE_DESERT:
            glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
            break;
        case EXERCISE_FACTORY:
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            break;
        case EXERCISE_HORROR:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case EXERCISE_BIOCHEMICAL_LAB:
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
            break;
        default:
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            break;
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        switch (exercise_mode)
        {
        case EXERCISE_DESERT:
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.03f, 0.24f, 0.14f);
            lightingShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
            lightingShader.setVec3("pointLights[0].diffuse",pointLightColors[0]);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0]);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1] * 0.1f);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1]);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1]);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
            // point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2] * 0.1f);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2]);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2]);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
            // point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3] * 0.1f);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3]);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3]);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
            // spotLight
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.0f);
            lightingShader.setVec3("spotLight.specular", 0.8f, 0.8f, 0.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));    

            break;
        case EXERCISE_FACTORY:
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.1f);
            lightingShader.setVec3("dirLight.diffuse", 0.2f, 0.2f, 0.7f);
            lightingShader.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);

            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
            lightingShader.setVec3("pointLights[0].diffuse",pointLightColors[0]);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0]);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1] * 0.1f);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1]);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1]);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
            // point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2] * 0.1f);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2]);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2]);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
            // point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3] * 0.1f);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3]);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3]);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
            // spotLight
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.009f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));    
            break;
        case EXERCISE_HORROR:
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05);
            lightingShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
            lightingShader.setVec3("pointLights[0].diffuse",pointLightColors[0]);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0]);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.14f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.07f);

            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1] * 0.1f);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1]);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1]);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.14f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.07f);
            // point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2] * 0.1f);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2]);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2]);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.22f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.20f);
            // point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3] * 0.1f);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3]);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3]);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.14f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.07f);
            // spotLight
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.009f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
            break;
        case EXERCISE_BIOCHEMICAL_LAB:
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
            lightingShader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
            lightingShader.setVec3("pointLights[0].diffuse",pointLightColors[0]);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0]);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.07f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.017f);

            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1] * 0.1f);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1]);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1]);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.07f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.017f);
            // point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2] * 0.1f);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2]);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2]);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.07f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.017f);
            // point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3] * 0.1f);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3]);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3]);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.07f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.017f);
            // spotLight
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.07f);
            lightingShader.setFloat("spotLight.quadratic", 0.017f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
            break;
        default:
            // directional light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // point light 1
            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
            // point light 2
            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
            // point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
            // point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
            // spotLight
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));     

            break;
        }


        

        // 将投影矩阵传递给着色器(因为投影矩阵很少改变，所以不需要每帧都这样做)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

       // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);



        glBindVertexArray(cubeVAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
         lightCubeShader.use();
         lightCubeShader.setMat4("projection", projection);
         lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
         }
        
        // 函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    /*
        这里我们检查用户是否按下了返回键(Esc)
        （如果没有按下，glfwGetKey将会返回GLFW_RELEASE。如果用户的确按下了返回键，
        我们将通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为
        true的方法关闭GLFW。
    */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
} 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //确保视口匹配新的窗口尺寸;注意宽度和
    //高度将明显大于视网膜显示器上的指定。
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    // 这个bool变量初始化时是设定为true的，解决窗口第一次获取焦点的时候摄像机会突然跳一下。
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // 计算当前帧和上一帧鼠标位置的偏移量
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// 从文件加载二维纹理的实用函数
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if(data)
    {
        GLenum format;
        if(nrComponents == 1)
        {
            format = GL_RED;
        } else if(nrComponents == 3)
        {
            format = GL_RGB;
        } else if(nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
