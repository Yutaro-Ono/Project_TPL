//----------------------------------------------------------------------------------+
// @file        GBuffer_NormalMap.vert
// @brief       output to GBuffer (basic vertex)
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_vertexPos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in vec3 a_tangent;
//----------------------------------------------------+
// uniform buffer block
// matrices
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};
// camera variables
layout(std140, binding = 1) uniform CameraVariable
{
	vec3 u_viewPos;
};


// out structure (Output to Fragment)
out VS_OUT
{
	vec3 fragWorldPos;
	vec3 fragNormal;
	vec2 fragTexCoords;

	// tangent space
	vec3 fragTangentLightPos;
	vec3 fragTangentViewPos;
	vec3 fragTangentFragPos;

}vs_out;

uniform mat4 u_worldTransform;     // world space
uniform vec3 u_lightPos;


void main()
{
	gl_Position = u_projection * u_view * u_worldTransform * vec4(a_vertexPos, 1.0);

	vs_out.fragWorldPos = vec3(u_worldTransform * vec4(a_vertexPos, 1.0f));
	vs_out.fragNormal = mat3(transpose(inverse(u_worldTransform))) * a_normal;
	vs_out.fragTexCoords = a_texCoords;


	// Tangent Calc
	vec3 T, B, N;     // TBN mat
	N = a_normal;
	T = a_tangent;

	T = normalize(T - dot(T, N) * N);
	B = cross(N, T);

	T = normalize(vec3(vec4(T, 0.0f) * u_worldTransform));
	B = normalize(vec3(vec4(B, 0.0f) * u_worldTransform));
	N = normalize(vec3(vec4(N, 0.0f) * u_worldTransform));

	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.fragTangentLightPos = u_lightPos.xyz * TBN;
	vs_out.fragTangentViewPos = u_viewPos.xyz * TBN;
	vs_out.fragTangentFragPos = vec3(vec4(a_vertexPos, 0.0f) * u_worldTransform) * TBN;
}