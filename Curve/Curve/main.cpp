//
//  main.cpp
//  Curve
//
//  Created by xue on 2018/5/17.
//  Copyright © 2018年 xue. All rights reserved.
//

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 960;

//GLSL顶点着色器的源代码
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"   gl_PointSize = 5; "
"}\0";
//GLSL片段着色器的源代码
const char* fragmentShaderSource = "#version 330 core\n"
"uniform vec4 Color;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = Color;\n"
"}\n\0";

//这是一个点的类，用于存储其中点的坐标
class Point {
public:
    double x, y;
    void setxy(double _x, double _y) {
        x = _x;
        y = _y;
    }
};
// timing
float deltaTime = 0.0f;
float last = 0.0f;
//点的数量
static int POINTSNUM = 0;
//用于存储鼠标选择的点的集合
static Point MouseSelectPoints[4];
//获得line上的点数组
float points[100000];
int points_index = 0;
int reset_index = -1;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
}
float normalize(double input) {
    return float(input)/480;
}

void mouse_buttonCallback(GLFWwindow* window, int button, int action, int mods) {
    //鼠标左键画点
    if (action == GLFW_PRESS && button == 0 && POINTSNUM < 4) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        // 鼠标的坐标系和窗口的坐标系不一致
        std::cout << reset_index << std::endl;
        if (reset_index == -1) {
            MouseSelectPoints[POINTSNUM].x = xpos-480;
            MouseSelectPoints[POINTSNUM].y = -ypos+480;
            points[points_index++] = normalize(MouseSelectPoints[POINTSNUM].x);
            points[points_index++] = normalize(MouseSelectPoints[POINTSNUM].y);
        } else {
            MouseSelectPoints[reset_index].x = xpos-480;
            MouseSelectPoints[reset_index].y = -ypos+480;
            points[2*reset_index] = normalize(MouseSelectPoints[reset_index].x);
            points[2*reset_index+1] = normalize(MouseSelectPoints[reset_index].y);
            reset_index = -1;
        }
        POINTSNUM++;
    } else if (action == GLFW_PRESS && button == 1 && POINTSNUM < 4) { //鼠标右键消除前一个点

        POINTSNUM--;
        points_index--;
        points_index--;
    
    } else if (action == GLFW_PRESS && button == 1 && POINTSNUM == 4) {
        std::cout << "double click" << std::endl;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        Point mouse;
        mouse.x = xpos-480;
        mouse.y = -ypos+480;
        for (int i = 0; i < 4; i++) {
            double deltax = MouseSelectPoints[i].x - mouse.x;
            double deltay = MouseSelectPoints[i].y - mouse.y;
            //std::cout << deltax << " " << deltay << std::endl;
            if (deltax <= 10 && deltay <= 10 && deltax >= -10 && deltay >= -10) {
                std::cout << i << std::endl;
                POINTSNUM--;
                reset_index = i;
                //std::cout << POINTSNUM << std::endl;
            }
        }
        
    }
    if (POINTSNUM == 4)
        for (int i = 0; i < 4; i++)
            std::cout << MouseSelectPoints[i].x << "," << MouseSelectPoints[i].y  << std::endl;
}


// 绘制贝塞尔曲线
void setBezier(Point* result, Point p1, Point p2, Point p3, Point p4, double t) {
    double a1 = pow((1 - t), 3);
    double a2 = pow((1 - t), 2) * 3 * t;
    double a3 = 3 * t*t*(1 - t);
    double a4 = t*t*t;
    result->x = a1*p1.x + a2*p2.x + a3*p3.x + a4*p4.x;
    result->y = a1*p1.y + a2*p2.y + a3*p3.y + a4*p4.y;
}

void Get_Points_of_Curve() {
    
    //J从8开始，前四个坐标表示的是鼠标点击的点
    int j = 8;
    if (POINTSNUM == 4) {
        //绘制贝塞尔曲线
        for (double t = 0.0; t <= 1.0; t += 0.0005) {
            Point point;
            setBezier(&point, MouseSelectPoints[0], MouseSelectPoints[1], MouseSelectPoints[2], MouseSelectPoints[3], t);
            points[j++] = normalize(point.x);
            points[j++] = normalize(point.y);
            points_index = j;
        }
    }
    
}


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
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Curve", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    // 告诉GLFW我们希望每当窗口调整大小的时候调用framebuffer_size_callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    gl3wInit();
    
    //imGUI
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();*/
    
    //告诉GLFW我们希望每当鼠标button被按下时的时候调用mouse_buttonCallback函数
    glfwSetMouseButtonCallback(window, mouse_buttonCallback);
    
    
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 顶点数组对象，创建一个VAO
    // -----------------------------
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    //使用glBindVertexArray绑定VAO
    glBindVertexArray(VAO);
    // 把顶点数组复制到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    // 设置顶点位置属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 启用修改点大小
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    bool setting = true;
    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0);
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        float current = glfwGetTime();
        deltaTime = current - last;
        last = current;
        // input
        // -----
        processInput(window);

        ImGui_ImplGlfwGL3_NewFrame();
        // 使用一个自定义的颜色清空屏幕
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        Get_Points_of_Curve();
        // 改变曲线颜色
        if (setting) {
            ImGui::Begin("Setting", &setting);
            ImGui::ColorEdit3("color", (float*)&color);
            glUniform4fv(glGetUniformLocation(shaderProgram, "Color"), 1, &color[0]);
            ImGui::End();
        }
        
        // 画曲线
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // 调用glBufferData函数，把修改后的顶点数据复制到缓冲的内存中
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        
        //std::cout << points_index << std::endl;
        glDrawArrays(GL_POINTS, 0, points_index/2);
        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        //交换颜色缓冲
        glfwSwapBuffers(window);
        // 检查有没有触发什么事件
        glfwPollEvents();
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    // 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;

}



