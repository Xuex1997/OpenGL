#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = vec3(model * vec4(aNormal, 1.0));
    //Normal = mat3(transpose(inverse(model))) * aNormal;//进行了不等比缩放
    gl_Position = projection * view * model * vec4(FragPos, 1.0);
}
