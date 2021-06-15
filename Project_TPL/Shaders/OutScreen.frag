//----------------------------------------------------------------------------------+
// @file        OutScreen.frag
// @brief       uniform texture output to screen entire
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output
out vec4 out_fragColor;

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
}fs_in;

uniform sampler2D u_screenTexture;

void main()
{
	// pass to output color
	out_fragColor = vec4(texture(u_screenTexture, fs_in.fragTexCoords).rgb, 1.0f);
}