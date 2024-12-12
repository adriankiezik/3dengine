#version 330 core
out vec4 FragColor;

in vec3 fragNormal;        // Normal from the mesh (passed from vertex shader)
in vec2 fragTexCoords;     // Texture coordinates passed from vertex shader
in vec3 fragPosition;      // Fragment position in world space

uniform sampler2D texture_diffuse;  // Diffuse texture

uniform vec3 lightDirection; // Direction of the sunlight (normalized)
uniform vec3 lightColor;     // Color of the sunlight
uniform vec3 viewPosition;   // Camera position for specular calculation

void main() {
    // Use the normal passed from the vertex shader
    vec3 normal = normalize(fragNormal);

    // Diffuse lighting (Lambertian reflection)
    float diffuseFactor = max(dot(normal, -lightDirection), 0.0); // Dot product of normal and light direction
    vec3 diffuse = diffuseFactor * lightColor;

    // Specular lighting (Blinn-Phong reflection model)
    vec3 viewDir = normalize(viewPosition - fragPosition);  // Vector from fragment to camera
    vec3 halfVec = normalize(viewDir + lightDirection);  // Halfway vector between light direction and view direction
    // Specular component can be added if desired (currently commented out)
    //float specularFactor = pow(max(dot(normal, halfVec), 0.0), 1.0); // Blinn-Phong shininess factor
    // vec3 specular = specularFactor * lightColor;

    // Combine results
    vec4 texColor = texture(texture_diffuse, fragTexCoords); // Get the texture color
    vec3 lighting = diffuse + lightColor; // For flat shading, we don't add specular for simplicity
    vec4 finalColor = vec4(lighting, 1.0) * texColor; // Combine lighting with texture color

    FragColor = finalColor; // Final color output
}
