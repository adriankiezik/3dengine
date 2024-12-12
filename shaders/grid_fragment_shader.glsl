#version 330 core

out vec4 FragColor;

uniform vec3 color;
uniform vec3 cameraPosition;
uniform float distanceThreshold;
uniform float fadeFactor;

in vec3 fragPosition; // Position of the fragment (vertex position in world space)

void main()
{
    // Calculate the distance between the camera and the current fragment
    float distance = length(cameraPosition - fragPosition);

    float alpha = 1.0;
    if (distance > distanceThreshold) {
        // Apply exponential decay for transparency after the threshold
        alpha = exp(-fadeFactor * (distance - distanceThreshold));  // Exponential decay
    }

    // Set the fragment color with the calculated alpha value
    FragColor = vec4(color, alpha);
}
