//----------------------------------------------------------------------------------+
// @file        Normal_Visualization.frag
// @brief       Visualize Normal
// @note        
// @author      Yutaro Ono, @2021
//----------------------------------------------------------------------------------+
#version 420
// output gbuffer albedo
layout(location = 2) out vec4 out_gAlbedoSpec;

void main()
{
	out_gAlbedoSpec = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}