//----------------------------------------------------------------------------------+
// @file        GBuffer_DirectionalLight.frag
// @brief       Directional Light Pass
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output buffer
layout(location = 0) out vec4 out_colorBuffer;
layout(location = 1) out vec4 out_brightBuffer;

// input structure from vertex shader
in VS_OUT
{
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


// GBuffer structure
struct GBuffer
{
	sampler2D position;
	sampler2D normal;
	sampler2D albedoSpec;
	sampler2D emissive;
};
uniform GBuffer u_gBuffer;

uniform float u_highBrightLine = 1.2f;

void main()
{
	// Sampling GBuffer
	vec3 gPos = texture(u_gBuffer.position, fs_in.fragTexCoords).xyz;
	vec3 gNormal = normalize(texture(u_gBuffer.normal, fs_in.fragTexCoords).xyz);
	vec4 gAlbedoSpec = texture(u_gBuffer.albedoSpec, fs_in.fragTexCoords);
	vec3 gAlbedo = gAlbedoSpec.rgb;
	float gSpec = gAlbedoSpec.a;

	// ambient
	vec3 ambient = u_ambientColor * gAlbedo;
	vec3 lightDir = normalize(-u_direction);
	float diff = max(dot(gNormal, lightDir), 0.0f);

	// diffuse
	vec3 diffuse = u_diffuseColor * u_intensity * gAlbedo * diff;

	// specular
	vec3 viewDir = normalize(u_viewPos - gPos);
	vec3 halfVec = normalize(lightDir + viewDir);
	float spec = pow(max(dot(gNormal, halfVec), 0.0f), 32.0f);
	vec3 specular = u_specularColor *u_intensity * spec * gSpec;

	// output to color buffer
	vec3 result = ambient + diffuse + specular + texture(u_gBuffer.emissive, fs_in.fragTexCoords).rgb;
	out_colorBuffer = vec4(result, 1.0f);

	// High Bright
	if(u_enableBloom)
	{
		vec3 brightColor = ambient + diffuse + texture(u_gBuffer.emissive, fs_in.fragTexCoords).rgb;
		float brightness = dot(result, vec3(0.1326f, 0.1352f, 0.642f));

		if(brightness > u_highBrightLine)
		{
			out_brightBuffer = vec4(brightColor, 0.0f);
		}
		else
		{
			out_brightBuffer = vec4(0.0f);
		}
	}
}