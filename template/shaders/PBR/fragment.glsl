#version 330 core
#define M_PI 3.1415926535897932384626433832795
in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalMap;
uniform sampler2D rougnessMap;
uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform float specular =0.5;
uniform vec4 shadowColor = vec4(0.01,0.04,0.2,1); 
void main() {
    vec3 normal = normalize(o_normalWorld*texture(normalMap, o_uv0).xyz);
    float metallic=texture(colorTexture, o_uv0).z;
    float rougness=texture(colorTexture, o_uv0).y;  
    //rougness = 0.1;
    vec3 camtoPoint = normalize(o_positionWorld - cameraPos);
    vec3 V =-camtoPoint;
    vec3 l = reflect(camtoPoint, normal); // l dans le cours
    vec3 h = normalize(camtoPoint+l); // peut être faux 
    vec3 reflectColor = textureLod(skybox,l,rougness*4).xyz;
//rajout d'un peu d'ombre
    float shadows = dot(vec3(0,-1,0),normal)/2+0.5;

    vec3 colorTemp = reflectColor*specular/2 +   texture(colorTexture, o_uv0).xyz/(1+specular);//apply specular
    colorTemp = colorTemp*(1-metallic) +metallic*colorTemp*reflectColor;
    colorTemp= colorTemp*(1-shadows)+ colorTemp*shadowColor.xyz*shadows;
    FragColor = vec4(colorTemp,1.0) ;
}
