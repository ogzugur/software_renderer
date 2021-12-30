#version 330 core

layout(location = 0) out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D frameTexture;

void main() 
{
    vec4 textureColor = texture(frameTexture,texCoord); 
    FragColor = vec4(textureColor);
}

