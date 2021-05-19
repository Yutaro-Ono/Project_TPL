//----------------------------------------------------------------------------------+
// @file        BasicMesh.vert
// @brief       BasicMesh Shader (vertex)
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_vertexPos;
layout(location = 1) in vec3 a_normalPos;
layout(location = 2) in vec2 a_texCoords;
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
	vec2 fragTexCoords;
}vs_out;

uniform mat4 u_worldTransform;     // world space

void main()
{
	vs_out.fragTexCoords = a_texCoords;
	gl_Position = u_worldTransform * vec4(a_vertexPos, 1.0);
}