//----------------------------------------------------------------------------------+
// @file        OutScreen.vert
// @brief       uniform texture output to screen entire
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec2 a_vertexPos;
layout(location = 1) in vec2 a_texCoords;

// out structure (Output to Fragment)
out VS_OUT
{
	vec2 fragTexCoords;
}vs_out;

void main()
{
	vs_out.fragTexCoords = a_texCoords;
	gl_Position = vec4(a_vertexPos.x, a_vertexPos.y, 0.0f, 1.0f);
}