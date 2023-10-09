#version 330 core

struct PBR
{
    vec3 albedo;
    float metalness;
    float roughness;
    float ao;
};

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec3 o_uv0;

out vec4 FragColor;

uniform PBR params;

void main() {
    FragColor = vec4(params.albedo, 1.0);
}
