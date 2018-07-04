//
//  FPS.cpp
//  Camera
//
//  Created by xue on 2018/4/9.
//  Copyright © 2018年 xue. All rights reserved.
//


#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// 鼠标移动事件的回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


//GLSL顶点着色器的源代码(储存在一个C的字符串中)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertColor;"
"uniform mat4 view;"
"uniform mat4 model;"
"uniform mat4 projection;"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertColor = color;"
"}\0";
//GLSL片段着色器的源代码(储存在一个C的字符串中）
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertColor;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(vertColor, 1.0f);\n"
"}\n\0";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main() {
    // 调用glfwInit函数来初始化GLFW
    glfwInit();
    // 使用glfwWindowHint函数来配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 创建一个窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cube", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    // 告诉GLFW我们希望每当窗口调整大小的时候调用framebuffer_size_callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 告诉GLFW，隐藏光标，并捕捉(Capture)它
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 用GLFW注册了回调函数之后，鼠标一移动mouse_callback函数就会被调用
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    gl3wInit();
    
    
    // 在运行时动态编译顶点着色器的源码
    //-----------------------------
    //创建着色器对象vertexShader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //将着色器源码附加到着色器对象上，然后编译它
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检测在调用glCompileShader后编译是否成功
    //----------------------------------
    //定义一个GLint变量success来表示是否成功编译，定义了一个储存错误消息的容器infoLog[。然后用glGetShaderiv检查是否编译成功。
    //如果编译失败，用glGetShaderInfoLog获取错误消息，然后打印它。
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    // 在运行时动态编译片段着色器的源码
    // ------------------------
    // 创建着色器对象fragmentShader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //检测在调用glCompileShader后编译是否成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 着色器程序
    //-------------------------------
    // 创建程序对象
    GLint shaderProgram = glCreateProgram();
    // 把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检测链接着色器程序是否失败
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // 激活深度测试
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
        0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
        0.2f,  0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
        0.2f,  0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
        -0.2f,  0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
        -0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 0.0f,
        
        -0.2f, -0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        0.2f, -0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        -0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        -0.2f, -0.2f,  0.2f, 0.0f, 1.0f, 0.0f,
        
        -0.2f,  0.2f,  0.2f, 0.0f, 0.0f, 1.0f,
        -0.2f,  0.2f, -0.2f, 0.0f, 0.0f, 1.0f,
        -0.2f, -0.2f, -0.2f, 0.0f, 0.0f, 1.0f,
        -0.2f, -0.2f, -0.2f, 0.0f, 0.0f, 1.0f,
        -0.2f, -0.2f,  0.2f, 0.0f, 0.0f, 1.0f,
        -0.2f,  0.2f,  0.2f, 0.0f, 0.0f, 1.0f,
        
        0.2f,  0.2f,  0.2f, 1.0f, 0.0f, 1.0f,
        0.2f,  0.2f, -0.2f, 1.0f, 0.0f, 1.0f,
        0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 1.0f,
        0.2f, -0.2f, -0.2f, 1.0f, 0.0f, 1.0f,
        0.2f, -0.2f,  0.2f, 1.0f, 0.0f, 1.0f,
        0.2f,  0.2f,  0.2f, 1.0f, 0.0f, 1.0f,
        
        -0.2f, -0.2f, -0.2f, 1.0f, 1.0f, 0.0f,
        0.2f, -0.2f, -0.2f, 1.0f, 1.0f, 0.0f,
        0.2f, -0.2f,  0.2f, 1.0f, 1.0f, 0.0f,
        0.2f, -0.2f,  0.2f, 1.0f, 1.0f, 0.0f,
        -0.2f, -0.2f,  0.2f, 1.0f, 1.0f, 0.0f,
        -0.2f, -0.2f, -0.2f, 1.0f, 1.0f, 0.0f,
        
        -0.2f,  0.2f, -0.2f, 0.0f, 1.0f, 1.0f,
        0.2f,  0.2f, -0.2f, 0.0f, 1.0f, 1.0f,
        0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 1.0f,
        0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 1.0f,
        -0.2f,  0.2f,  0.2f, 0.0f, 1.0f, 1.0f,
        -0.2f,  0.2f, -0.2f, 0.0f, 1.0f, 1.0f
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    // 解析顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    // 启用顶点位置属性
    glEnableVertexAttribArray(0);
    // 解析顶点颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
    // 启用顶点颜色属性
    glEnableVertexAttribArray(1);
    
    
    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))      {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
        
        
        
        // Rendering
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        // 调用glBufferData函数，把修改后的顶点数据复制到缓冲的内存中
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}


