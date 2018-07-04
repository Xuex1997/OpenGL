//
//  main.cpp
//  imGUI Triangle
//
//  Created by xue on 2018/3/14.
//  Copyright © 2018年 xue. All rights reserved.
//
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <iostream>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>

// 对窗口注册一个回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
//GLSL顶点着色器的源代码(储存在一个C的字符串中)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 color;\n"
"out vec4 vertColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertColor = color;"
"}\0";
//GLSL片段着色器的源代码(储存在一个C的字符串中)，输出颜色为白色
const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 vertColor;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vertColor;\n"
"}\n\0";

int main(int, char**) {
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    gl3wInit();
    //imGUI
    ImGui::CreateContext();
    
    ImGui_ImplGlfwGL3_Init(window, true);
    
    
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();*/
    
    
    //------------------------
    GLfloat vertices[] = {
        // 顶点坐标   顶点颜色
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f
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
    //VBO的缓冲类型是GL_ARRAY_BUFFER，使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 顶点数组对象，创建一个VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    //使用glBindVertexArray绑定VAO
    glBindVertexArray(VAO);
    // 调用glBufferData函数，把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 链接顶点属性
    // --------------------------------
    // 解析顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    // 启用顶点位置属性
    glEnableVertexAttribArray(0);
    // 解析顶点颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    // 启用顶点颜色属性
    glEnableVertexAttribArray(1);
    
    bool setting = true;
    bool select_Triangle_color = false;
    bool select_Vertices_color = false;
    ImVec4 Vertices_color1  = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
    ImVec4 Vertices_color2 = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
    ImVec4 Vertices_color3 = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
    ImVec4 Triangle_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        
        if (setting) {
            ImGui::Begin("Setting", &setting);
            ImGui::Checkbox("select Triangle color", &select_Triangle_color);
            ImGui::Checkbox("select Vertices color", &select_Vertices_color);
            if (select_Triangle_color) {
                ImGui::Text("Select the color of triangle");
                
                ImGui::ColorEdit3("Triangle color", (float*)&Triangle_color);
                vertices[3]=Triangle_color.x;
                vertices[4]=Triangle_color.y;
                vertices[5]=Triangle_color.z;
                
                vertices[9]=Triangle_color.x;
                vertices[10]=Triangle_color.y;
                vertices[11]=Triangle_color.z;
                
                vertices[15]=Triangle_color.x;
                vertices[16]=Triangle_color.y;
                vertices[17]=Triangle_color.z;
                
                select_Vertices_color = false;
            }
            if (select_Vertices_color) {
                ImGui::Text("Select the color of Vertices");
                
                ImGui::ColorEdit3("X1", (float*)&Vertices_color1);
                ImGui::ColorEdit3("X2", (float*)&Vertices_color2);
                ImGui::ColorEdit3("X3", (float*)&Vertices_color3);
                
                vertices[3]=Vertices_color1.x;
                vertices[4]=Vertices_color1.y;
                vertices[5]=Vertices_color1.z;
                
                vertices[9]=Vertices_color2.x;
                vertices[10]=Vertices_color2.y;
                vertices[11]=Vertices_color2.z;
                
                vertices[15]=Vertices_color3.x;
                vertices[16]=Vertices_color3.y;
                vertices[17]=Vertices_color3.z;
                
                select_Triangle_color = false;
            }
            if (!select_Vertices_color&!select_Triangle_color) {
                vertices[3]=1.0f;
                vertices[4]=1.0f;
                vertices[5]=1.0f;
                
                vertices[9]=1.0f;
                vertices[10]=1.0f;
                vertices[11]=1.0f;
                
                vertices[15]=1.0f;
                vertices[16]=1.0f;
                vertices[17]=1.0f;
                
            }
            
            ImGui::End();
            
        }
        
        
        // Rendering
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        // 调用glBufferData函数，把修改后的顶点数据复制到缓冲的内存中
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    
    return 0;
}

