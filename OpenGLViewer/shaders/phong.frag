#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// ── Material ──────────────────────────────────────────────────────────────────
struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
    vec3  color;      // base tint
};
uniform Material material;

// ── Point light ───────────────────────────────────────────────────────────────
struct PointLight {
    vec3  position;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight light;

uniform vec3 viewPos;
uniform bool uSelected; // highlight selected object

void main() {
    vec3 norm    = normalize(Normal);
    vec3 lightDir= normalize(light.position - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Attenuation
    float dist  = length(light.position - FragPos);
    float atten = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // Ambient
    vec3 ambient = light.ambient * material.ambient * material.color;

    // Diffuse (Lambert)
    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse * material.color;

    // Specular (Phong)
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular   = light.specular * spec * material.specular;

    vec3 result = (ambient + diffuse + specular) * atten;

    // Selection highlight: slight emissive yellow rim
    if (uSelected) {
        float rimFactor = 1.0 - max(dot(viewDir, norm), 0.0);
        rimFactor = pow(rimFactor, 3.0);
        result += vec3(0.6, 0.6, 0.0) * rimFactor * 0.5;
    }

    FragColor = vec4(result, 1.0);
}
