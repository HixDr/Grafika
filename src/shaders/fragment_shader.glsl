#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;    // Sphere's position (passed from CPU)
uniform vec3 lightColor;  // Sphere's light color (e.g., white)
uniform vec3 viewPos;     // Camera position
uniform sampler2D diffuseMap;

// Phong lighting components
vec3 calculateLighting(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor) {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    return (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 lighting = calculateLighting(norm, FragPos, lightPos, lightColor);

    // Apply texture (if exists)
    vec3 texColor = texture(diffuseMap, TexCoord).rgb;
    FragColor = vec4(lighting * texColor, 1.0);
}