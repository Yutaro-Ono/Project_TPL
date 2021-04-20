//----------------------------------------------------------------------------------+
// @file        BasicShader.frag
// @brief       BasicShader (fragment)
// @note        output 1 dot color on clip space.
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
out vec4 out_fragColor;

in vec3 in_color;

void main()
{
	out_fragColor = vec4(in_color, 1.0);
}