//----------------------------------------------------------------------------------+
// @file        DrawableObjectManager.h
// @brief       �`��\�I�u�W�F�N�g�̊Ǘ��N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#pragma once
#include <vector>
#include "ShaderManager.h"

class DrawableObjectManager
{

public:

	DrawableObjectManager();
	~DrawableObjectManager();



	void AddMeshComp(class MeshComponent* _meshComp);
	void DeleteMeshComp(class MeshComponent* _meshComp);

	void Draw(class GLSLprogram* _shader);





private:

	std::vector<class MeshComponent*> m_meshComponents;

};