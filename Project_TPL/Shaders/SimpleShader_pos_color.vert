//----------------------------------------------------------------------------------+
// @file        BasicShader.vert
// @brief       BasicShader (vertex)
// @note        vertex position & color(rgb)
//              output clipspace to shader
//              output color to shader
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout(location = 0) in vec3 a_vertexPos;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec3 a_color;
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
	vec3 fragColor;
}vs_out;




void main()
{

	vs_out.fragColor = a_color;
	vs_out.fragTexCoords = a_texCoords;

	// pass to gl_position
	gl_Position = vec4(a_vertexPos, 1.0);

}