//----------------------------------------------------------------------------------+
// @file        Normal_Visualization.geom
// @brief       Visualize Normal
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// attribute
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

// out structure (Output to Fra, mgment)
in VS_OUT
{
	vec4 normal;
}geom_in[];

const float SCALE_DOWN = 0.2f;

void GenerateLine(int _idx)
{
	gl_Position = gl_in[_idx].gl_Position;
	EmitVertex();
	gl_Position = gl_in[_idx].gl_Position + geom_in[_idx].normal * SCALE_DOWN;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}