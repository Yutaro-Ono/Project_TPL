//----------------------------------------------------------------------------------+
// @file        BasicShader.frag
// @brief       BasicShader (fragment)
// @note        output 1 dot color on clip space.
//              not texture sampling
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output
out vec4 out_fragColor;

in vec3 fragColor;

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
	vec3 fragColor;
}fs_in;

void main()
{
	//result color calcuration
	vec4 resultColor = vec4(fs_in.fragColor, 1.0);

	// pass to output color
	out_fragColor = resultColor;
}