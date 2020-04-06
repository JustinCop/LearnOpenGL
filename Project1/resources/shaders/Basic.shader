#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_MVP;

out vec2 v_texCoord;

void main()
{
    gl_Position = u_MVP * position;
    v_texCoord = texCoord;
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    color = u_Color * texture(u_Texture, v_texCoord);
}