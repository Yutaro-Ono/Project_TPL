//----------------------------------------------------------------------------------+
// @file        DrawableObjectManager.cpp
// @brief       �`��\�I�u�W�F�N�g�̊Ǘ��N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "DrawableObjectManager.h"
#include "ShaderManager.h"
#include "MeshComponent.h"


DrawableObjectManager::DrawableObjectManager()
{
}

DrawableObjectManager::~DrawableObjectManager()
{
}

void DrawableObjectManager::AddMeshComp(MeshComponent* _meshComp)
{
	m_meshComponents.emplace_back(_meshComp);
}

void DrawableObjectManager::DeleteMeshComp(MeshComponent* _meshComp)
{
	auto iter = std::find(m_meshComponents.begin(), m_meshComponents.end(), _meshComp);

	if (iter != m_meshComponents.end())
	{
		std::iter_swap(iter, m_meshComponents.end() - 1);
		m_meshComponents.pop_back();
	}

}

void DrawableObjectManager::Draw(ShaderManager* _shaderManager)
{
	for (auto mc : m_meshComponents)
	{
		mc->Draw(_shaderManager->GetShader(GLSLshader::BASIC_MESH));
	}
}
