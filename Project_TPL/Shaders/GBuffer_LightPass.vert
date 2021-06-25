//----------------------------------------------------------------------------------+
// @file        GBuffer_LightPass.vert
// @brief       Lighting Pass of Deffered Rendering
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec2 a_pos;

// out structure (Output to Fragment)
out VS_OUT
{
	vec2 fragTexCoords;
}vs_out;


void main()
{
	vec4 pos = vec4(a_pos, 0.0f, 1.0f);

	gl_Position = pos;

	// conversion : Normalized device coordinates -> Texture coordinates of gBuffer
	// x / w, y / w, -> [0.0 ~ 1.0]
	vs_out.fragTexCoords.x = pos.x / pos.w * 0.5f + 0.5f;
	vs_out.fragTexCoords.y = pos.y / pos.w * 0.5f + 0.5f;
}