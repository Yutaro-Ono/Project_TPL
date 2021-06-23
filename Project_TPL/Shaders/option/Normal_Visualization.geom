//----------------------------------------------------------------------------------+
// @file        Normal_Visualization.geom
// @brief       Visualize Normal
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// out structure (Output to Fra, mgment)
in VS_OUT
{
	vec3 normal;
}geom_in[];

//----------------------------------------------------+
// uniform buffer block
// matrices
layout(std140, binding = 0) uniform Matrices
{
	mat4 u_view;
	mat4 u_projection;
};

const float MAGNITUDE = 0.2f;

void GenerateLine(int _idx)
{
	gl_Position = u_projection * gl_in[_idx].gl_Position;
	EmitVertex();
	gl_Position = u_projection * (gl_in[_idx].gl_Position + vec4(geom_in[_idx].normal, 0.0f) * MAGNITUDE);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}