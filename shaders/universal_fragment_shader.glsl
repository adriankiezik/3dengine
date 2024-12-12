#version 330 core
out vec4 FragColor;

in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 fragPosition;

uniform sampler2D texture_diffuse;

uniform vec3 lightDirection; // Direction of the sunlight (normalized)
uniform vec3 lightColor;     // Color of the sunlight
uniform vec3 viewPosition;   // Camera position for specular calculation

void main() {
    // Normalize the normal vector
    vec3 normal = normalize(fragNormal);

    // Diffuse lighting
    float diffuseFactor = max(dot(normal, -lightDirection), 0.0); // Dot product of normal and light direction
    vec3 diffuse = diffuseFactor * lightColor;

    // Specular lighting (Phong reflection model)
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(lightDirection, normal); // Reflect light direction around the normal
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), 0.0); // Shininess factor = 32
    vec3 specular = specularFactor * lightColor;

    // Combine results
    vec4 texColor = texture(texture_diffuse, fragTexCoords);
    vec3 lighting = diffuse + specular;
    vec4 finalColor = vec4(lighting, 1.0) * texColor;

    FragColor = finalColor;
}
