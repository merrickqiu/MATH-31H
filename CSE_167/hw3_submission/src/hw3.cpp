#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"

#include "3rdparty/glm/glm.hpp"
#include "3rdparty/glm/gtc/matrix_transform.hpp"
#include "3rdparty/glm/gtc/type_ptr.hpp"

using namespace hw3;

// globals
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float zFar;

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = zFar * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int useShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
glm::mat4 toGlmMat4(const Matrix4x4f& matrix) {
    glm::mat4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[j][i] = matrix(i, j);
        }
    }
    return result;
}
void hw_3_1(const std::vector<std::string> &params) {
    // HW 3.1: Open a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
const char* vertexShaderSource2 = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 rotationMatrix;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = rotationMatrix*vec4(aPos, 1.0);\n"
    "}\0";
const char* fragmentShaderSource2 = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
void hw_3_2(const std::vector<std::string> &params) {
    // HW 3.2: Render a single 2D triangle
    // Setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    unsigned int shaderProgram = useShader(vertexShaderSource2, fragmentShaderSource2);

    // vertex data
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
       //  1, 2, 3   // second Triangle
    };

    // Vertex Array Object Vertex Buffer Object, 
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // background color
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // set rotation and activate shader
        glm::mat4 trans = glm::mat4(1.0f);
        float angle = glfwGetTime();
        trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));
        int rotationMatrixLocation = glGetUniformLocation(shaderProgram, "rotationMatrix");
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(trans));
        //render shape
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

const char* vertexShaderSource3 = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "uniform mat4 transMatrix;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transMatrix*vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char* fragmentShaderSource3 = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    Camera camera = scene.camera;
    float s = camera.s;
    float a = camera.resolution.x / camera.resolution.y;
    zFar = camera.z_far;
    float zNear = camera.z_near;
    float width = camera.resolution.x;
    float height = camera.resolution.y;
    float alpha = 2 * atan(s);
    Matrix4x4f camMatrix = camera.cam_to_world;
    cameraPos = glm::vec3(camMatrix(0, 3), camMatrix(1, 3), camMatrix(2, 3));
    cameraFront = glm::vec3(-camMatrix(0, 2), -camMatrix(1, 2), -camMatrix(2, 2));
    cameraUp = glm::vec3(camMatrix(0,1), camMatrix(1, 1), camMatrix(2, 1));
    
    std::cout << scene << std::endl;

    // Setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    
    unsigned int shaderProgram = useShader(vertexShaderSource3, fragmentShaderSource3);   

    // Vertex Array Object Vertex Buffer Object, Element buffer object    
    std::vector<unsigned int> VAOs(scene.meshes.size());
    std::vector<unsigned int> VBOs(scene.meshes.size());
    std::vector<unsigned int> EBOs(scene.meshes.size());
    std::vector<int> numIndices(scene.meshes.size());

    glGenVertexArrays(scene.meshes.size(), &VAOs[0]);
    glGenBuffers(scene.meshes.size(), &VBOs[0]);
    glGenBuffers(scene.meshes.size(), &EBOs[0]);

    for (int i = 0; i < scene.meshes.size(); i++) {
        // compile vertex data
        TriangleMesh mesh = scene.meshes[i]; 
        std::vector<Vector3f> meshVertices = mesh.vertices;
        std::vector<Vector3f> colors = mesh.vertex_colors;
        Matrix4x4f modelMatrix = mesh.model_matrix;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        for (int j = 0; j < meshVertices.size(); j++) {
            Vector3f vertex = meshVertices[j];
            std::vector<float> row = { vertex.x, vertex.y, vertex.z, colors[j].x, colors[j].y, colors[j].z };
            vertices.insert(vertices.end(), row.begin(), row.end());
        }

        std::vector<Vector3i> faces = mesh.faces;
        for (Vector3i face : faces) {
            indices.push_back(face.x);
            indices.push_back(face.y);
            indices.push_back(face.z);
        }
        numIndices[i] = indices.size();
        // Bind Buffers
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // unbinding the VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);  
    }

    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // time and input
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // background color
        glClearColor(scene.background.x, scene.background.y, scene.background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render each mesh
        for (int i = 0; i < scene.meshes.size(); i++) {
            unsigned int VAO = VAOs[i];
            // apply matrix transformations      
            glm::mat4 model = toGlmMat4(scene.meshes[i].model_matrix);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(alpha, a, zNear, zFar);
            glm::mat4 trans =  projection * view * model;
            int transLocation = glGetUniformLocation(shaderProgram, "transMatrix");
            glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(trans));

            //render shape
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        // glfw swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
};
const char* objectVS4 =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec3 aNormal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec3 objectColor;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"void main()\n"
"{\n"
"   gl_Position = projection*view*model*vec4(aPos, 1.0);\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   objectColor = aColor;\n"
"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"}\0";

const char* objectFS4 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"uniform float u_time;\n"
"in vec3 objectColor;\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"void main()\n"
"{\n"
"   float ambientStrength = 0.1;\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   vec3 norm = normalize(Normal);\n"
"   float x = cos(u_time);\n"
"   float y = 1;\n"
"   float z = sin(u_time);\n"
"   vec3 lightDir = normalize(vec3(x, y, z));\n"
"   float diff = max(dot(norm, lightDir), 0.0);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   float specularStrength = 0.5;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, norm);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   vec3 result = (ambient + diffuse + specular) * objectColor;\n"
"   FragColor = vec4(result, 1.0f);\n"
"}\n\0";

const char* lightVS4 =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection*view*model*vec4(aPos, 1.0);\n"
"}\0";

const char* lightFS4 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0);\n"
"}\n\0";

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    Camera camera = scene.camera;
    float s = camera.s;
    float a = camera.resolution.x / camera.resolution.y;
    zFar = camera.z_far;
    float zNear = camera.z_near;
    float width = camera.resolution.x;
    float height = camera.resolution.y;
    float alpha = 2 * atan(s);
    Matrix4x4f camMatrix = camera.cam_to_world;
    cameraPos = glm::vec3(camMatrix(0, 3), camMatrix(1, 3), camMatrix(2, 3));
    cameraFront = glm::vec3(-camMatrix(0, 2), -camMatrix(1, 2), -camMatrix(2, 2));
    cameraUp = glm::vec3(camMatrix(0, 1), camMatrix(1, 1), camMatrix(2, 1));

    std::cout << scene << std::endl;

    // Setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    unsigned int objectShader = useShader(objectVS4, objectFS4);
    unsigned int lightShader = useShader(lightVS4, lightFS4);

    // Vertex Array Object Vertex Buffer Object, Element buffer object
    std::vector<unsigned int> VAOs(scene.meshes.size());
    std::vector<unsigned int> VBOs(scene.meshes.size());
    std::vector<unsigned int> EBOs(scene.meshes.size());
    std::vector<int> numIndices(scene.meshes.size());

    
    glGenVertexArrays(scene.meshes.size(), &VAOs[0]);
    glGenBuffers(scene.meshes.size(), &VBOs[0]);
    glGenBuffers(scene.meshes.size(), &EBOs[0]);
    // lighting buffers
    /*unsigned int lightVAO;
    unsigned int lightVBO;

    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindVertexArray(lightVAO);*/

    //position attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // mesh buffers
    for (int i = 0; i < scene.meshes.size(); i++) {
        // compile vertex data
        TriangleMesh mesh = scene.meshes[i];
        std::vector<Vector3f> meshVertices = mesh.vertices;
        std::vector<Vector3f> meshNormals = mesh.vertex_normals;
        std::vector<Vector3f> colors = mesh.vertex_colors;
        Matrix4x4f modelMatrix = mesh.model_matrix;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        for (int j = 0; j < meshVertices.size(); j++) {
            Vector3f vertex = meshVertices[j];
            Vector3f color = colors[j];
            Vector3f normal = meshNormals[j];
            std::vector<float> row = { vertex.x, vertex.y, vertex.z, color.x, color.y, color.z, normal.x, normal.y, normal.z};
            vertices.insert(vertices.end(), row.begin(), row.end());
        }

        std::vector<Vector3i> faces = mesh.faces;
        for (Vector3i face : faces) {
            indices.push_back(face.x);
            indices.push_back(face.y);
            indices.push_back(face.z);
        }
        numIndices[i] = indices.size();
        // Bind Buffers
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // normal vector attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // unbinding the VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // time and input
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        // background color
        glClearColor(scene.background.x, scene.background.y, scene.background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render light cube 
        /*glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(alpha, a, zNear, zFar);
        
        glUseProgram(lightShader);
        glUniformMatrix4fv(glGetUniformLocation(lightShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/

        // Render each mesh
        for (int i = 0; i < scene.meshes.size(); i++) {            
            unsigned int VAO = VAOs[i];
            // apply matrix transformations      
            glm::mat4 model = toGlmMat4(scene.meshes[i].model_matrix);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(alpha, a, zNear, zFar);
            glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
            
            glUseProgram(objectShader);
            glUniformMatrix4fv(glGetUniformLocation(objectShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(objectShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(objectShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3f(glGetUniformLocation(objectShader, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
            glUniform3f(glGetUniformLocation(objectShader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
            glUniform1f(glGetUniformLocation(objectShader, "u_time"), currentFrame);
            //render shape  
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        // glfw swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
