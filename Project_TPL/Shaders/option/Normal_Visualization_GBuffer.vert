//----------------------------------------------------------------------------------+
// @file        Normal_Visualization.vert
// @brief       Visualize Normal
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_vertexPos;
layout(location = 1) in vec3 a_normal;
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
	vec3 normal;
}vs_out;

uniform mat4 u_worldTransform;     // world space

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(u_view * u_worldTransform)));
	vs_out.normal = vec3(vec4(normalMatrix * a_normal, 0.0f));
	gl_Position = u_view * u_worldTransform * vec4(a_vertexPos, 1.0f);
}