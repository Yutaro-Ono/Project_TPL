//----------------------------------------------------------------------------------+
// @file        CubeMap.h
// @brief       �L���[�u�}�b�v�̒�`�E���̕ێ�
//              
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/22   �V�K�쐬
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