
//
//  main.cpp
//  Triangle
//
//  Created by xue on 2018/3/14.
//  Copyright © 2018年 xue. All rights reserved.
//

#include <iostream>

// 引入GLEW库 定义静态链接
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// 对窗口注册一个回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    std::cout << "click" << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//GLSL顶点着色器的源代码(储存在一个C的字符串中)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//GLSL片段着色器的源代码(储存在一个C的字符串中)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);\n"
"}\n\0";
int main() {
    // 调用glfwInit函数来初始化GLFW
    glfwInit();
    
    // 使用glfwWindowHint函数来配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 创建一个窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL-Triangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 告诉GLFW我们希望每当窗口调整大小的时候调用framebuffer_size_callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // 初始化GLEW 获取OpenGL函数
    glewExperimental = GL_TRUE; // 让glew获取所有拓展函数
    GLenum status = glewInit();
    if (status != GLEW_OK)
    {
        std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
        << " error string:" << glewGetErrorString(status) << std::endl;
        glfwTerminate();
        return -1;
    }
    // 指定三个顶点
    //------------------------
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    
    // 在运行时动态编译顶点着色器的源码
    //-----------------------------
    //创建着色器对象vertexShader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //将着色器源码附加到着色器对象上，然后编译它
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    //检测在调用glCompileShader后编译是否成功
    //----------------------------------
    //定义一个GLint变量success来表示是否成功编译，定义了一个储存错误消息的容器infoLog[。然后用glGetShaderiv检查是否编译成功。
    //如果编译失败，用glGetShaderInfoLog获取错误消息，然后打印它。
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 在运行时动态编译片段着色器的源码
    // ------------------------
    // 创建着色器对象fragmentShader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //检测在调用glCompileShader后编译是否成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // 着色器程序
    //-------------------------------
    // 创建程序对象
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    // 把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检测链接着色器程序是否失败
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    // 激活程序对象
    glUseProgram(shaderProgram);
    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    // 使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    GLuint VBO;
    glGenBuffers(1, &VBO);
    
    // 顶点数组对象，创建一个VAO
    // -----------------------------
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    //使用glBindVertexArray绑定VAO
    glBindVertexArray(VAO);
    // 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 接触VAO绑定
    glBindVertexArray(0);
    
    // 输出一些版本信息
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW library version: " << glfwGetVersionString() << std::endl;
    
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 使用一个自定义的颜色清空屏幕
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 画三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // 检查有没有触发什么事件
        glfwPollEvents();
        
        //交换颜色缓冲
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

