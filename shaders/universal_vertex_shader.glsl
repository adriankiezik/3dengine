#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec2 fragTexCoords;

void main() {
    fragNormal = normal;
    fragTexCoords = texCoords;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
