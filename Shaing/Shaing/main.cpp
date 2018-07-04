//
//  main.cpp
//  Shaing
//
//  Created by xue on 2018/4/11.
//  Copyright © 2018年 xue. All rights reserved.
//

#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "Shader.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// 灯光位置
glm::vec3 lightPos(0.55f, 0.60f, 1.0f);
glm::vec3 viewPos(0.0f, 1.0f, 3.0f);

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
    
    gl3wInit();
    
    //imGUI
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    
    // 激活深度测试
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    
    // 创建和编译着色器
    // ------------------------------------
    Shader GouraudShader("/Users/xue/Desktop/OPENGL/Shaing/Shaing/Gouraud.vs", "/Users/xue/Desktop/OPENGL/Shaing/Shaing/Gouraud.fs");
    Shader PhongShader("/Users/xue/Desktop/OPENGL/Shaing/Shaing/Phong.vs", "/Users/xue/Desktop/OPENGL/Shaing/Shaing/Phong.fs");
    Shader lampShader("/Users/xue/Desktop/OPENGL/Shaing/Shaing/lamp.vs", "/Users/xue/Desktop/OPENGL/Shaing/Shaing/lamp.fs");
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // 立方体着色器和VBO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 法向量属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    // 灯的着色器和物体着色器用的是一个顶点数组
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 注意顶点数组的改变，灯的着色器只需每行的前三个数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    int shading = 0;
    float ambientStrength = 0.1;
    float diffStrength = 1.0;
    float specularStrength = 1.0;
    int Shininess = 32;
    bool Tranalate_lamp1 = false;
    bool Tranalate_lamp2 = false;
    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))      {
        
        //processInput(window);
        
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection;
        glm::mat4 view;
        
        
        {
            ImGui::Text("Choose use Phong Phong Shading or Gouraud Shading");
            ImGui::RadioButton("Phong", &shading, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Gouraud", &shading, 1);
            
            if (shading == 0) {
                ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
                ImGui::SliderFloat("diffStrength", &diffStrength, 0.0f, 1.0f);
                ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
                ImGui::SliderInt("Shininess", &Shininess, 2, 256);
                
                
                if(ImGui::Button("Reset")) {
                    ambientStrength = 0.1;
                    diffStrength = 1.0;
                    specularStrength = 1.0;
                    Shininess = 32;
                    lightPos = glm::vec3(0.55f, 0.60f, 1.0f);
                }
                ImGui::Checkbox("Tranalate lamp", &Tranalate_lamp1);
                if(Tranalate_lamp1) {
                    lightPos.x = 1.0f + sin(glfwGetTime()) * 3.0f;
                    lightPos.y = sin(glfwGetTime() / 3.0f) * 1.0f;
                }
                // 确保先激活着色器程序
                PhongShader.use();
                PhongShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
                PhongShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
                PhongShader.setVec3("lightPos", lightPos);
                
                PhongShader.setFloat("ambientStrength", ambientStrength);
                PhongShader.setFloat("diffStrength", diffStrength);
                PhongShader.setFloat("specularStrength", specularStrength);
                PhongShader.setInt("Shininess", Shininess);
                
                // 观察者位置
                PhongShader.setVec3("viewPos", viewPos);
                
                // view/projection 变换
                projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                view = glm::lookAt(viewPos, viewPos + glm::vec3(0.0f, -1.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                PhongShader.setMat4("projection", projection);
                PhongShader.setMat4("view", view);
                
                // world transformation
                glm::mat4 model;
                PhongShader.setMat4("model", model);
            }
            // 透视投影
            if (shading == 1) {
                ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
                ImGui::SliderFloat("diffStrength", &diffStrength, 0.0f, 1.0f);
                ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f);
                ImGui::SliderInt("Shininess", &Shininess, 2, 256);
                if(ImGui::Button("Reset")) {
                    ambientStrength = 0.1;
                    diffStrength = 1.0;
                    specularStrength = 1.0;
                    Shininess = 32;
                    lightPos = glm::vec3(0.55f, 0.60f, 1.0f);
                }
                ImGui::Checkbox("Tranalate lamp", &Tranalate_lamp2);
                if(Tranalate_lamp2) {
                    lightPos.x = 1.0f + sin(glfwGetTime()) * 3.0f;
                    lightPos.y = sin(glfwGetTime() / 3.0f) * 1.0f;
                }
                // 确保先激活着色器程序
                GouraudShader.use();
                GouraudShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
                GouraudShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
                GouraudShader.setVec3("lightPos", lightPos);
                
                GouraudShader.setFloat("ambientStrength", ambientStrength);
                GouraudShader.setFloat("diffStrength", diffStrength);
                GouraudShader.setFloat("specularStrength", specularStrength);
                GouraudShader.setInt("Shininess", Shininess);
                // 观察者位置
                GouraudShader.setVec3("viewPos", viewPos);
                
                //GouraudShader.setVec3("viewPos", camera.Position);
                // view/projection 变换
                projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
                view = glm::lookAt(viewPos, viewPos + glm::vec3(0.0f, -1.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                //view = camera.GetViewMatrix();
                GouraudShader.setMat4("projection", projection);
                GouraudShader.setMat4("view", view);
                
                glm::mat4 model;
                GouraudShader.setMat4("model", model);
            }
        }
        // 渲染cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        // 渲染灯光
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        glm::mat4 model2;
        model2 = glm::translate(model2, lightPos);
        model2 = glm::scale(model2, glm::vec3(0.2f));
        lampShader.setMat4("model", model2);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

