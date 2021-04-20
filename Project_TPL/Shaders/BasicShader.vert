//----------------------------------------------------------------------------------+
// @file        BasicShader.vert
// @brief       BasicShader (vertex)
// @note        vertex position & color(rgb)
//              output clipspace to shader
//              output color to shader
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
layout (location = 0) in vec3 a_vertexPos;
layout (location = 1) in vec3 a_color;

out vec3 out_color;

void main()
{

	out_color = a_color;
	gl_Position = vec4(a_vertexPos, 1.0);

}