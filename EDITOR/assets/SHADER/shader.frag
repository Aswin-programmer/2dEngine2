#version 430

in vec2 fragUVs;
in vec4 fragColor;

out vec4 color;

uniform sampler2D uTexture;

void main()
{
    vec4 texColor = texture(uTexture, fragUVs);
    color = texColor * fragColor;
}