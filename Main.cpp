#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader_m.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cubo com Camera", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader(,);
    
    float cuboSize = 0.2;
    
    float vertices[] = {
        -cuboSize, -cuboSize, -cuboSize,
        cuboSize, -cuboSize, -cuboSize,
        cuboSize,  cuboSize, -cuboSize,
        cuboSize,  cuboSize, -cuboSize,
        -cuboSize,  cuboSize, -cuboSize,
        -cuboSize, -cuboSize, -cuboSize,

        -cuboSize, -cuboSize,  cuboSize,
        cuboSize, -cuboSize,  cuboSize,
        cuboSize,  cuboSize,  cuboSize,
        cuboSize,  cuboSize,  cuboSize,
        -cuboSize,  cuboSize,  cuboSize,
        -cuboSize, -cuboSize,  cuboSize,

        -cuboSize,  cuboSize,  cuboSize,
        -cuboSize,  cuboSize, -cuboSize,
        -cuboSize, -cuboSize, -cuboSize,
        -cuboSize, -cuboSize, -cuboSize,
        -cuboSize, -cuboSize,  cuboSize,
        -cuboSize,  cuboSize,  cuboSize,

        cuboSize,  cuboSize,  cuboSize,
        cuboSize,  cuboSize, -cuboSize,
        cuboSize, -cuboSize, -cuboSize,
        cuboSize, -cuboSize, -cuboSize,
        cuboSize, -cuboSize,  cuboSize,
        cuboSize,  cuboSize,  cuboSize,

        -cuboSize, -cuboSize, -cuboSize,
        cuboSize, -cuboSize, -cuboSize,
        cuboSize, -cuboSize,  cuboSize,
        cuboSize, -cuboSize,  cuboSize,
        -cuboSize, -cuboSize,  cuboSize,
        -cuboSize, -cuboSize, -cuboSize,

        -cuboSize,  cuboSize, -cuboSize,
        cuboSize,  cuboSize, -cuboSize,
        cuboSize,  cuboSize,  cuboSize,
        cuboSize,  cuboSize,  cuboSize,
        -cuboSize,  cuboSize,  cuboSize,
        -cuboSize,  cuboSize, -cuboSize
    };
    
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Shader shaderChao(,);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float chaoSize = 1.0f;
    
    float chaoVertices[] = {
        // positions
        chaoSize,  chaoSize, 0.0f,
        chaoSize, -chaoSize, 0.0f,
        -chaoSize, -chaoSize, 0.0f,
        -chaoSize,  chaoSize, 0.0f
    };
    unsigned int chaoIndices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int chaoVBO, chaoVAO, chaoEBO;
    glGenVertexArrays(1, &chaoVAO);
    glGenBuffers(1, &chaoVBO);
    glGenBuffers(1, &chaoEBO);

    glBindVertexArray(chaoVAO);

    glBindBuffer(GL_ARRAY_BUFFER, chaoVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chaoVertices), chaoVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chaoEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(chaoIndices), chaoIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // -------- end of chão -----------

    while (!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //------------Render cube------------------------------------
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        ourShader.setMat4("view", view);

        glBindVertexArray(VAO);
        
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[0]);
        float angle = 20.0f * 0;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        //-----------------------------------------------------------
        
        //------Render chão----------------------
        shaderChao.use();
      
        // create transformations
        glm::mat4 chaoModel         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 chaoView          = glm::mat4(1.0f);
        glm::mat4 chaoProjection    = glm::mat4(1.0f);
        chaoModel = glm::rotate(chaoModel, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        chaoView  = glm::translate(chaoView, glm::vec3(0.0f, 0.0f, -3.0f));
        chaoProjection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int chaoModelLoc = glGetUniformLocation(shaderChao.ID, "model");
        unsigned int chaoViewLoc  = glGetUniformLocation(shaderChao.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(chaoModelLoc, 1, GL_FALSE, glm::value_ptr(chaoModel));
        glUniformMatrix4fv(chaoViewLoc, 1, GL_FALSE, &chaoView[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", chaoProjection);

        glBindVertexArray(chaoVAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //-------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //frente
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        cameraPos = glm::vec3(0.0f, 0.0f, -3.0f); //costas
        cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        cameraPos = glm::vec3(0.0f, 3.0f, 0.0f); //cima
        cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
        cameraUp = glm::vec3(-1.0f, 0.0f, 0.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        cameraPos = glm::vec3(0.0f, -3.0f, 0.0f); //baixo
        cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraUp = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        cameraPos = glm::vec3(3.0f, 0.0f, 0.0f); //direita
        cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        cameraPos = glm::vec3(-3.0f, 0.0f, 0.0f); //esquerda
        cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
