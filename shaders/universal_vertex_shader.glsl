#version 330 core
layout (location = 0) in vec3 position;    // Vertex position
layout (location = 1) in vec3 normal;      // Vertex normal
layout (location = 2) in vec2 texCoords;   // Texture coordinates

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;       // Pass the normal to fragment shader without interpolation
out vec2 fragTexCoords;    // Pass the texture coordinates to fragment shader
out vec3 fragPosition;     // Fragment position in world space

void main() {
    // Calculate world space position
    vec4 worldPosition = model * vec4(position, 1.0);
    fragPosition = worldPosition.xyz;

    // Pass the same normal for flat shading
    fragNormal = normalize(mat3(transpose(inverse(model))) * normal);

    // Pass the texture coordinates
    fragTexCoords = texCoords;

    // Transform the vertex position to clip space
    gl_Position = projection * view * worldPosition;
}
