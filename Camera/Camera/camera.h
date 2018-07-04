//
//  camera.h
//  Camera
//
//  Created by xue on 2018/4/8.
//  Copyright © 2018年 xue. All rights reserved.
//

#ifndef camera_h
#define camera_h
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// const常量
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;


class Camera {
    public:
        // 相机属性
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
    
        // 欧拉角
        float Yaw;
        float Pitch;
    
        // 相机操作
        float MovementSpeed;
        float MouseSensitivity;
    
        // 构造函数
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) {
            Position = position;
            Front = glm::vec3(0.0f, 0.0f, -1.0f);
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            MovementSpeed = SPEED;
            MouseSensitivity = SENSITIVITY;
            updateCameraVectors();
        }
    
        // 返回View矩阵
        glm::mat4 GetViewMatrix() {
            return glm::lookAt(Position, Position + Front, Up);
        }
    
        // 处理键盘输入
        void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
            float velocity = MovementSpeed * deltaTime;
            if (direction == FORWARD)
                Position += Front * velocity;
            if (direction == BACKWARD)
                Position -= Front * velocity;
            if (direction == LEFT)
                Position -= Right * velocity;
            if (direction == RIGHT)
                Position += Right * velocity;
        }
    
        // 处理鼠标的输入
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;
        
            Yaw += xoffset;
            Pitch += yoffset;
        
            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch) {
                if (Pitch > 89.0f)
                    Pitch = 89.0f;
                if (Pitch < -89.0f)
                    Pitch = -89.0f;
            }
        
            // 更新 Front, Right 和 Up向量
            updateCameraVectors();
        }
    
    
    private:
        // 通过更新的欧拉角重新计算Front，Right以及Up向量
        void updateCameraVectors()  {
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
                        Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));
        }
};

#endif /* camera_h */
