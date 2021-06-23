//----------------------------------------------------------------------------------+
// @file        GBuffer_Phong.frag
// @brief       output to GBuffer (Phong Lighting)
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
	bool u_enableBloom;
};
// Directional Light
layout(std140, binding = 3) uniform DirLight
{
	vec3 u_direction;
	vec3 u_diffuseColor;
	vec3 u_specularColor;
	vec3 u_ambientColor;
	float u_intensity;
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
	sampler2D emissiveMap;
};
uniform Material u_mat;

uniform float u_specularPower = 1.0f;

void main()
{
	// PhongLighting
	vec3 N = normalize(fs_in.fragNormal);                    // Polygon Surface normal
	vec3 L = normalize(-u_direction);                        // Vector from : Neg Light Direction
	vec3 V = normalize(u_viewPos - fs_in.fragWorldPos);      // Vector from : Polygon Pos -> Camera Pos
	vec3 R = normalize(reflect(-L, N));                      // Reflect Vector from : Light Dir -> Polygon Surface
	// Phong Reflection Calculation
	vec3 Phong = u_ambientColor;
	float NdotL = dot(N, L);

	vec3 diffuse = u_diffuseColor * max(NdotL, 0.0f);
	vec3 specular = u_specularColor * pow(max(0.0f, dot(R, V)), u_specularPower) * u_intensity;

	// pass to output gBuffer
	out_gPosition = fs_in.fragWorldPos;
	out_gNormal = normalize(fs_in.fragNormal);
	out_gAlbedoSpec.rgb = texture(u_mat.albedo, fs_in.fragTexCoords).rgb * vec3(diffuse + u_ambientColor) + specular;
	out_gAlbedoSpec.a = texture(u_mat.specular, fs_in.fragTexCoords).r;

	// bloom
	if(u_enableBloom)
	{
		out_gEmissive = texture(u_mat.emissiveMap, fs_in.fragTexCoords);
	}

}