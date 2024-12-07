#version 330 core
out vec4 FragColor;

in vec3 fragNormal;
in vec2 fragTexCoords;

uniform sampler2D texture_diffuse1;

void main() {
    vec4 texColor = texture(texture_diffuse1, fragTexCoords);
    FragColor = texColor;
}
