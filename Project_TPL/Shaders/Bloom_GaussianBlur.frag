//----------------------------------------------------------------------------------+
// @file        Bloom_GaussianBlur.frag
// @brief       prev:down sampling, next:bloom
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

// uniform structure
// gauss parameter
struct GaussBlurParam
{
	int sampleCount;
	vec3 offset[16];
};
uniform GaussBlurParam u_gaussParam;

uniform sampler2D u_blurSource;

void main()
{
	vec4 result = vec4(0.0f);
	for(int i = 0; i < u_gaussParam.sampleCount; i++)
	{
		// Synthesize textures from Gaussian weights
		result += u_gaussParam.offset[i].z * 
		          texture(u_blurSource, fs_in.fragTexCoords + u_gaussParam.offset[i].xy);
	}

	result.w = 1.0f;
	out_fragColor = result;

}