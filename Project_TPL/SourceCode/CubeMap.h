//----------------------------------------------------------------------------------+
// @file        CubeMap.h
// @brief       キューブマップの定義・情報の保持
//              
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/22   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <string>



class CubeMap
{

public:

	CubeMap(const std::string& _filePath);
	~CubeMap();

	void Draw(class GLSLprogram* _shader);

private:

	class VertexArray* m_cubeVerts;

	unsigned int m_cubeMapID;

};