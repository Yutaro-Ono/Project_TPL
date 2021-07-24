//----------------------------------------------------------------------------------+
// @file        GBuffer_NormalMap.frag
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

	// tangent space
	vec3 fragTangentLightPos;
	vec3 fragTangentViewPos;
	vec3 fragTangentFragPos;
}fs_in;

// triggers
layout(std140, binding = 2) uniform Triggers
{
	int u_enableBloom;
};

// Directional Light
layout(std140, binding = 3) uniform DirLight
{
	vec3 u_dLightDir;
	vec3 u_dLightDiffuse;
	vec3 u_dLightSpecular;
	vec3 u_dLightAmbient;
	float u_dLightIntensity;
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
	vec3 normal = texture(u_mat.normal, fs_in.fragTexCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	out_gNormal = normal;

	// diffuse sampling
	vec3 color = texture(u_mat.albedo, fs_in.fragTexCoords).rgb;

	// ambient
	vec3 ambient = u_dLightAmbient * color;

	// diffuse color
	vec3 lightDir = normalize(fs_in.fragTangentLightPos - fs_in.fragTangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = u_dLightDiffuse * diff * color;

	// スペキュラ計算
	vec3 viewDir = normalize(fs_in.fragTangentViewPos - fs_in.fragTangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = u_dLightSpecular * spec * texture(u_mat.specular, fs_in.fragTexCoords).rgb;


	// pass to output gBuffer
	out_gPosition = fs_in.fragWorldPos;

	vec3 resultColor = ambient + diffuse;
	out_gAlbedoSpec.rgb = resultColor;

	float resultAlpha = texture(u_mat.emissive, fs_in.fragTexCoords).a + specular.r;
	out_gAlbedoSpec.a = texture(u_mat.specular, fs_in.fragTexCoords).r;

	// bloom
	if(u_enableBloom == 1)
	{
		vec4 emissiveColor = texture(u_mat.emissive, fs_in.fragTexCoords);
		out_gEmissive = emissiveColor * 500000.0f;
	}

}