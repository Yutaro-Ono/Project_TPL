//----------------------------------------------------------------------------------+
// @file        Bloom_DownSampling.frag
// @brief       Down Sampling the Texture of Scene
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

void main()
{
	// Get Screen Size
	ivec2 size = textureSize(u_scene, 0);

	vec2 texOffset = vec2(1.0f / size);

	vec3 c1 = vec3(texture(u_scene, fs_in.fragTexCoords + texOffset * vec2(1.0f, 1.0f)));
	vec3 c2 = vec3(texture(u_scene, fs_in.fragTexCoords + texOffset * vec2(-1.0f, 1.0f)));
	vec3 c3 = vec3(texture(u_scene, fs_in.fragTexCoords + texOffset * vec2(1.0f, -1.0f)));
	vec3 c4 = vec3(texture(u_scene, fs_in.fragTexCoords + texOffset * vec2(-1.0f, -1.0f)));
	vec3 c5 = vec3(texture(u_scene, fs_in.fragTexCoords + texOffset * vec2(0.0f, 0.0f)));

	out_fragColor = vec4((c1 + c2 + c3 + c4 + c5) * 0.2f, 1.0f);
}