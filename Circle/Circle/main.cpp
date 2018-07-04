
//
//  main.cpp
//  Circle
//
//  Created by xue on 2018/3/25.
//  Copyright © 2018年 xue. All rights reserved.
//

// GLFW
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
using namespace std;
// 对窗口注册一个回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
//GLSL顶点着色器的源代码(储存在一个C的字符串中)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"}\0";
//GLSL片段着色器的源代码(储存在一个C的字符串中)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);\n"
"}\n\0";

float normalize(int input) {
    return float(input)/500;
}
void swap(int *a,int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void bresenham(int array[], int p, int i, int length, int dx, int dy) {
    if(i == length-1)
        return;
    int next_p;
    if(p <= 0) {
        array[i+1] = array[i];
        next_p = p + 2*dy;
    } else {
        array[i+1] = array[i] + 1;
        next_p = p + 2*dy - 2*dx;
    }
    bresenham(array, next_p, i+1, length, dx, dy);
}
void Get_Points_of_Line(int xarray[], int yarray[], int* length, int x0, int x1, int y0, int y1) {
    if(x0 == x1) { //斜率不存在是，x不变，y递增
        if (y0 > y1) {
            swap(&y0, &y1);
        }
        *length = y1 - y0 + 1;
        for (int i = 0; i < *length; i++) {
            xarray[i] = x0;
            yarray[i] = y0 + i;
        }
    } else {  //斜率存在
        float m = float(y1-y0)/float(x1-x0);
        //|m|<=1,让点1在点0的右边；|m|>1, 让点1在点0的上方
        if((fabs(m) <= 1 && x0 > x1)||(fabs(m) > 1 && y0 > y1)) {
            swap(&x0, &x1);
            swap(&y0, &y1);
        }
        
        int dx=x1-x0;
        int dy=y1-y0;
        m = float(dy)/float(dx);
        
        if(fabs(m)<=1) {
            *length = x1-x0+1;
            //当|m|<=1时，x的变化是向右递增的，y的变化由bresenham算法决定
            for (int i = 0; i < *length; i++)
                xarray[i] = x0 + i;
            
            if(dy>=0) { //当 0<m<=1
                yarray[0] = y0;
                yarray[*length-1] = y1;
                int p0 = 2*dy - dx;
                bresenham(yarray, p0, 0, *length, dx, dy);
            } else {
                //-1<=m<0
                yarray[0] = -y0;
                yarray[*length-1] = -y1;
                int p0 = 2*(-dy) - dx;
                bresenham(yarray, p0, 0, *length, dx, -dy);
                for(int i = 0;i < *length; i++)
                    yarray[i] = -yarray[i];
            }
        } else {
            *length=y1-y0+1;
            //当|m|>1时，y的变化是向上递增的，x的变化由bresenham算法决定
            for (int i = 0; i < *length; i++)
                yarray[i] = y0 + i;
            if( dx >= 0) {
                //m>1
                xarray[0] = x0;
                xarray[*length-1] = x1;
                int p0 = 2*dx - dy;
                bresenham(xarray, p0, 0, *length, dy,dx);
            } else {
                //m<-1
                xarray[0] = -x0;
                xarray[*length-1] = -x1;
                int p0 = 2*(-dx) - dy;
                bresenham(xarray, p0, 0, *length, dy, -dx);
                for(int i = 0; i < *length; i++)
                    xarray[i] = -xarray[i];
            }
        }
    }
}
void Gat_Points_of_Triangle(float points[], int*length) {
    int x0, x1, x2, y0, y1, y2;
    cout << "Please gives the x and y value of three points" << endl;
    cout << "with one space between them (like: 'x y')" << endl;
    cout << "Notice: the x and y value should be integers between -500 and 500" << endl << endl;
    cout << "the x and y value of the first point?" << endl;
    cin >> x0 >> y0;
    cout << "the coordinate of the first point: (" << x0 << ", " << y0 <<")." << endl;
    cout << "the x and y value of the second point?" << endl;
    cin >> x1 >> y1;
    cout << "the coordinate of the second point: (" << x1 << ", " << y1 <<")." << endl;
    cout << "the x and y value of the first point?" << endl;
    cin >> x2 >> y2;
    cout << "the coordinate of the Third point: (" << x2 << ", " << y2 <<")." << endl;

    
    int length1, length2, length3;
    int xarray1[1001], yarray1[1001],xarray2[1001], yarray2[1001],xarray3[1001], yarray3[1001];
    
    Get_Points_of_Line(xarray1, yarray1, &length1, x0, x1, y0, y1);
    Get_Points_of_Line(xarray2, yarray2, &length2, x0, x2, y0, y2);
    Get_Points_of_Line(xarray3, yarray3, &length3, x1, x2, y1, y2);
    
    float fxarray1[1001], fxarray2[1001], fxarray3[1001];
    float fyarray1[1001], fyarray2[1001], fyarray3[1001];
    int j=0;
    for(int i=0; i < length1; i++) {
        fxarray1[i] = normalize(xarray1[i]);
        fyarray1[i] = normalize(yarray1[i]);
        points[j++] = fxarray1[i];
        points[j++] = fyarray1[i];
    }
    for(int i=0; i<length2; i++) {
        fxarray2[i] = normalize(xarray2[i]);
        fyarray2[i] = normalize(yarray2[i]);
        points[j++] = fxarray2[i];
        points[j++] = fyarray2[i];
    }
    for(int i=0; i<length3; i++) {
        fxarray3[i] = normalize(xarray3[i]);
        fyarray3[i] = normalize(yarray3[i]);
        points[j++] = fxarray3[i];
        points[j++] = fyarray3[i];
    }
    *length = 2*(length1+length2+length3);
}
void Get_Points_Circle(float points[], int* index) {
    int r, cx,cy;
    cout << "Please give the coordinate of the center of the circle? like 'x y'" << endl
    << "Notice: x and y should be integers between 0 and 500" << endl;
    cin >> cx >> cy;
    cout << "Please give the radius r of the circle" << endl
    << "Notice: r should be an integer between 0 and 500" << endl;
    cin >> r;
    float fcx=normalize(cx);
    float fcy=normalize(cy);
    // 计算圆上点的坐标
    //------------------------
    int x = 0, y = r;
    GLint yy =(int)(r*1.0/(sqrt(2)));
    int d = 1.25-r;
    while(yy >= x) {
        float fx = normalize(x); //0
        float fy = normalize(y); //r
        //(x,y) (-x,-y) (-x,y) (x,-y)
        points[++(*index)] = fcx+fx;
        points[++(*index)] = fcy+fy;
        points[++(*index)] = fcx-fx;
        points[++(*index)] = fcy-fy;
        points[++(*index)] = fcx-fx;
        points[++(*index)] = fcy+fy;
        points[++(*index)] = fcx+fx;
        points[++(*index)] = fcy-fy;
        //(y,x) (-y,-x) (-y,x) (y,-x)
        points[++(*index)] = fcx+fy;
        points[++(*index)] = fcy+fx;
        points[++(*index)] = fcx-fy;
        points[++(*index)] = fcy-fx;
        points[++(*index)] = fcx-fy;
        points[++(*index)] = fcy+fx;
        points[++(*index)] = fcx+fy;
        points[++(*index)] = fcy-fx;
        
        if(d<0) {
            d = d + 2*x + 3;
        } else {
            d = d + 2*(x-y) + 5;
            y--;
        }
        x++;
    }
    
}
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
    GLFWwindow* window = glfwCreateWindow(600, 600, "OpenGL-Triangle", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 告诉GLFW我们希望每当窗口调整大小的时候调用framebuffer_size_callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
    //获得line上的点数组
    float points[100000];
    int index = -1;
    Get_Points_Circle(points, &index);
    
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
    // 设置顶点属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // 输出一些版本信息
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW library version: " << glfwGetVersionString() << std::endl;
    
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 检查有没有触发什么事件
        glfwPollEvents();
        // 使用一个自定义的颜色清空屏幕
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // 画三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // 调用glBufferData函数，把修改后的顶点数据复制到缓冲的内存中
        //glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        glDrawArrays(GL_POINTS, 0, index);
        

        //交换颜色缓冲
        glfwSwapBuffers(window);
    }
    // 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}


