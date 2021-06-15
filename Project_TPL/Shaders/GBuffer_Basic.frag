//----------------------------------------------------------------------------------+
// @file        GBuffer_Basic.vert
// @brief       output to GBuffer (basic fragment)
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output gbuffer
layout(location = 0) out vec3 out_gPosition;
layout(location = 1) out vec3 out_gNormal;
layout(location = 2) out vec4 out_gAlbedoSpec;

out vec4 out_fragColor;

// input structure from vertex shader
in VS_OUT
{
	vec3 fragWorldPos;
	vec3 fragNormal;
	vec2 fragTexCoords;
}fs_in;

// material structure
struct Material
{
	sampler2D albedo;
	sampler2D specular;
	sampler2D normal;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D AO;
};
uniform Material u_mat;


void main()
{
	// pass to output gBuffer
	out_gPosition = fs_in.fragWorldPos;
	out_gNormal = normalize(fs_in.fragNormal);
	out_gAlbedoSpec.rgb = texture(u_mat.albedo, fs_in.fragTexCoords).rgb;
	out_gAlbedoSpec.a = texture(u_mat.specular, fs_in.fragTexCoords).r;
}