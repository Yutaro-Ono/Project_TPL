//----------------------------------------------------------------------------------+
// @file        PBR_Basic.frag
// @brief       Physics Based Rendering
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output color
out vec4 out_fragColor;

// input structure from vertex shader
in VS_OUT
{
	vec3 fragWorldPos;
	vec3 fragNormal;
	vec2 fragTexCoords;
}fs_in;

//----------------------------------------------------+
// uniform buffer block
// camera variables
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};
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

// Lights
uniform vec3 u_lightPos[4];
uniform vec3 u_lightColors[4];

const float PI = 3.14159265359;

// MicroFacet Distribution Func
float DistributionGGX(vec3 _N, vec3 _H, float _roughness)
{
	float a = _roughness * _roughness;
	float a2 = a * a;
	float NdotH = max(dot(_N, _H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001f);
}

// Geometric decay term : Schlick model
float GeometrySchlickGGX(float _NdotV, float _roughness)
{
	float r = (_roughness + 1.0f);
	float k = (r * r) / 8.0f;

	float nom = _NdotV;
	float denom = _NdotV * (1.0f - k) + k;

	return nom / denom;
}

// Geometric decay term : Smith model
float GeometrySmith(vec3 _N, vec3 _V, vec3 _L, float _roughness)
{
	float NdotV = max(dot(_N, _V), 0.0f);
	float NdotL = max(dot(_N, _L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, _roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, _roughness);

	return ggx1 * ggx2;
}

// Fresnel term
vec3 FresnelSchlick(float _cosTheta, vec3 _F0)
{
	return _F0 + (1.0f - _F0) * pow(1.0f - _cosTheta, 5.0f);
}

void main()
{
	// Material Parameter
	// albedoMap is sRGB : convert to linear Space gamma 2.2
	//vec3 albedo = vec3(pow(texture(u_mat.albedo, fs_in.fragTexCoords).rgb, 2.2));
	vec3 albedo = vec3(pow(texture(u_mat.albedo, fs_in.fragTexCoords).rgb, vec3(2.2)));
	vec3 normal = texture(u_mat.normal, fs_in.fragTexCoords).rgb;
	float metallic = texture(u_mat.metallic, fs_in.fragTexCoords).r;
	float roughness = texture(u_mat.roughness, fs_in.fragTexCoords).r;
	float ao = texture(u_mat.AO, fs_in.fragTexCoords).r;

	// Basic lighting
	vec3 N = normalize(fs_in.fragNormal);
	vec3 V = normalize(u_viewPos - fs_in.fragWorldPos);
	
	vec3 F0 = vec3(0.04f);
	F0 = mix(F0, albedo, metallic);
	
	vec3 Lo = vec3(0.0f);
	for(int i = 0; i < 4; ++i)
	{
		// Calculate the Radiance per light
		vec3 L = normalize(u_lightPos[i] - fs_in.fragWorldPos);
		vec3 H = normalize(V + L);
		float dist = length(L);
		float attenuation = 1.0f / (dist * dist);
		vec3 radiance = u_lightColors[i] * attenuation;

		// Trans BRDF
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0f) - kS;
		kD *= 1.0f - metallic;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
		vec3 specular = numerator / max(denominator, 0.001f);

		// Add outgoing Light -> Lo
		float NdotL = max(dot(N, L), 0.0f);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03f) * albedo * ao;
	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0f));
	color = pow(color, vec3(1.0f / 2.2f));

	out_fragColor = vec4(color, 1.0f);
}