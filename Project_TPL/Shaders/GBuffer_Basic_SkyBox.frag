//----------------------------------------------------------------------------------+
// @file        GBuffer_SkyBox.frag
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
	vec3 fragLocalPos;
}fs_in;

// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

uniform samplerCube u_cubeMap;    // sample CubeMap Texture


void main()
{
	//output to Albedo
	out_gAlbedoSpec = texture(u_cubeMap, fs_in.fragLocalPos);

	// bloom
	if(u_enableBloom == 1)
	{
		out_gEmissive = texture(u_cubeMap, fs_in.fragLocalPos);
	}

}