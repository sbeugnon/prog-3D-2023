#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aUV;

out vec3 Normal;
out vec3 Position;
out vec3 UV;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light;

void main()
{
    Position = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * vec4(Position, 1.0);

    mat3 normalMat = mat3(transpose(inverse(model)));
    UV = aUV;
    Normal = normalMat * aNormal;
    LightPos = vec3(view * vec4(light, 1.0));
}