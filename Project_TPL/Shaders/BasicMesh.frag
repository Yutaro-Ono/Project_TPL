//----------------------------------------------------------------------------------+
// @file        BasicMesh.frag
// @brief       BasicMesh Shader (fragment)
// @note        only albedo
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

// material structure
struct Material
{
	sampler2D albedo;
	sampler2D normal;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D AO;
};
uniform Material u_mat;


void main()
{
	// pass to output color
	out_fragColor = texture(u_mat.albedo, fs_in.fragTexCoords);
}