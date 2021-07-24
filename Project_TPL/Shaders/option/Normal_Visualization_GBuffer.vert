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
	vec4 normal;
}vs_out;

uniform mat4 u_worldTransform;     // world space

void main()
{
	gl_Position = u_projection * u_view * u_worldTransform * vec4(a_vertexPos, 1.0f);
	// calculate inverse&transpose matrix
	mat4 normalTransform = transpose(inverse(u_view * u_worldTransform));
	vec4 normalViewSpace = normalTransform * vec4(a_normal, 0.0f);
	vs_out.normal = normalize(u_projection * normalViewSpace);
}