//----------------------------------------------------------------------------------+
// @file        GBuffer_SkyBox.vert
// @brief       output to GBuffer (basic vertex)
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_vertexPos;
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
	vec3 fragLocalPos;
}vs_out;

uniform mat4 u_removeTransView;

void main()
{

	vs_out.fragLocalPos = a_vertexPos;
	
	vec4 clipPos = u_projection * u_removeTransView * vec4(a_vertexPos, 1.0f);

	gl_Position = clipPos.xyww;
	
}