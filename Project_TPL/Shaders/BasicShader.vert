//----------------------------------------------------------------------------------+
// @file        BasicShader.vert
// @brief       ベーシックシェーダー (頂点)
// @note        頂点座標・カラーを読み込む
//              クリップ座標情報を計算し、後のプロセスに渡す
//              カラー情報はそのままフラグメントに出力
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#version 420
layout (location = 0) in vec3 a_vertexPos;
layout (location = 1) in vec3 a_color;

out vec3 out_color;

void main()
{

	out_color = a_color;
	gl_Position = vec4(a_vertexPos, 1.0);

}