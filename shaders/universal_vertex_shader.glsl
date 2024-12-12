#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 fragPosition;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0); // World-space position
    fragNormal = mat3(transpose(inverse(model))) * normal; // Transform normal to world space
    fragTexCoords = texCoords;
    fragPosition = worldPosition.xyz;
    gl_Position = projection * view * worldPosition;
}
