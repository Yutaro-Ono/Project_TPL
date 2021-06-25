//----------------------------------------------------------------------------------+
// @file        Bloom_ToneMapping.frag
// @brief       Blend the ColorBuffer & DS+GB Buffer
// @note        vertex shader = ToneMap
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output color
out vec4 out_fragColor;

// input structure from vertex shader
in VS_OUT
{
	vec2 fragTexCoords;
}fs_in;

// Texture of Scene
uniform sampler2D u_scene;

// Textures with Down sampling & Gaussian blur
uniform sampler2D u_bloom1;
uniform sampler2D u_bloom2;
uniform sampler2D u_bloom3;
uniform sampler2D u_bloom4;
uniform sampler2D u_bloom5;

uniform float u_exposure;     // 0 -> Dark Screen
uniform float u_gamma = 0.085;        // gamma collection

void main()
{
	vec3 hdrColor = texture(u_scene, fs_in.fragTexCoords).rgb;
	hdrColor += texture(u_bloom1, fs_in.fragTexCoords).rgb * 1.0f;
	hdrColor += texture(u_bloom2, fs_in.fragTexCoords).rgb * 1.5f;
	hdrColor += texture(u_bloom3, fs_in.fragTexCoords).rgb * 2.0f;
	hdrColor += texture(u_bloom4, fs_in.fragTexCoords).rgb * 3.0f;
	hdrColor += texture(u_bloom5, fs_in.fragTexCoords).rgb * 9.0f;

	// tone mapping (add exposure)
	vec3 mapped = vec3(1.0f) - exp(-hdrColor * u_exposure);

	mapped = pow(mapped, vec3(1.0f / u_gamma));
	out_fragColor = vec4(mapped, 1.0f);

}