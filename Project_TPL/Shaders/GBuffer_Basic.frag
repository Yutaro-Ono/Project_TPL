//----------------------------------------------------------------------------------+
// @file        GBuffer_Basic.frag
// @brief       output to GBuffer (basic fragment)
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output gbuffer
layout(location = 0) out vec3 out_gPosition;
layout(location = 1) out vec3 out_gNormal;
layout(location = 2) out vec4 out_gAlbedoSpec;
layout(location = 3) out vec4 out_gEmissive;

// input structure from vertex shader
in VS_OUT
{
	vec3 fragWorldPos;
	vec3 fragNormal;
	vec2 fragTexCoords;
}fs_in;

// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

// material structure
struct Material
{
	sampler2D albedo;
	sampler2D specular;
	sampler2D normal;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D AO;
	sampler2D emissive;
};
uniform Material u_mat;


void main()
{
	// pass to output gBuffer
	out_gPosition = fs_in.fragWorldPos;
	out_gNormal = normalize(fs_in.fragNormal);
	out_gAlbedoSpec.rgb = texture(u_mat.albedo, fs_in.fragTexCoords).rgb;
	out_gAlbedoSpec.a = texture(u_mat.specular, fs_in.fragTexCoords).r;

	// bloom
	if(u_enableBloom == 1)
	{
		out_gEmissive = texture(u_mat.emissive, fs_in.fragTexCoords);
	}

}