#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalMap;

void main() {
    FragColor = color*0.5;
    //FragColor = texture(colorTexture, o_uv0) ;//* color;
    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    //FragColor = vec4(0.5 * o_normalWorld*texture(normalMap, o_uv0).xyz + vec3(0.5) , 1.0);
    // DEBUG: normalMap
    //FragColor = vec4(0.5 * texture(normalMap, o_uv0).xyz + vec3(0.5) , 1.0);

    // DEBUG: uv0
    // FragColor = vec4(o_uv0,1.0, 1.0);
}
