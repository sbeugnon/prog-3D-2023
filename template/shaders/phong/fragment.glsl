#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec3 UV;
in vec3 LightPos;

uniform vec3 lightColor;
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;
uniform sampler2D bumpTexture;

void main()
{
    // bump map calculation
    ivec2 texSize  = textureSize(bumpTexture, 0);
    vec2 texOffset = 1.0 / vec2(texSize.xy);
    float color_right = texture(bumpTexture, UV.st + texOffset * vec2(1.0, 0.0)).r;
    float color_left = texture(bumpTexture, UV.st + texOffset * vec2(-1.0, 0.0)).r;
    float color_up = texture(bumpTexture, UV.st + texOffset * vec2(0.0, 1.0)).r;
    float color_down = texture(bumpTexture, UV.st + texOffset * vec2(0.0, -1.0)).r;
    float du = 0.5*(color_right-color_left);
    float dv = 0.5*(color_up-color_down);
    vec3 diru = vec3(1, 0, du);
    vec3 dirv = vec3(0, 1, dv);
    vec3 m = normalize(diru*dirv);

    vec3 ambient = lightColor*material.ambient;
    vec3 norm = normalize(Normal);
    //vec3 norm = m;
    vec3 lightDir = normalize(LightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * material.diffuse;

    vec3 viewDir = normalize(-Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * spec * material.specular;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}