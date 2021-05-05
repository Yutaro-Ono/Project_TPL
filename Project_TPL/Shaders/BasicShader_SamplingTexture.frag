//----------------------------------------------------------------------------------+
// @file        BasicShader_SamplingTexture.frag
// @brief       BasicShader (fragment)
// @note        output 1 dot color on clip space.
//              texture sampling enabled (only diffuse)
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
out vec4 out_fragColor;     // output color

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
	vec3 fragColor;
}fs_in;

// material information structure (textures and shininess...)
uniform struct MaterialInfo
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D shininess;
}u_mat;


void main()
{
	// result color calculation
	vec4 resultColor = texture(u_mat.diffuseMap, fs_in.fragTexCoords);

	// pass to output color
	out_fragColor = resultColor;
}