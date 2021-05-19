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
	// ���b�V���R���|�[�l���g�������
	while (!m_meshComponents.empty())
	{
		delete m_meshComponents.back();
	}
}

/// <summary>
/// ���b�V���R���|�[�l���g�N���X��z��ɒǉ�����
/// </summary>
/// <param name="_meshComp"> �z��ɒǉ����郁�b�V���R���|�[�l���g�N���X </param>
void DrawableObjectManager::AddMeshComp(MeshComponent* _meshComp)
{
	m_meshComponents.emplace_back(_meshComp);
}

/// <summary>
/// ���b�V���R���|�[�l���g�N���X�̍폜
/// </summary>
/// <param name="_meshComp"> �폜���������b�V���R���|�[�l���g�N���X </param>
void DrawableObjectManager::DeleteMeshComp(MeshComponent* _meshComp)
{
	auto iter = std::find(m_meshComponents.begin(), m_meshComponents.end(), _meshComp);

	if (iter != m_meshComponents.end())
	{
		std::iter_swap(iter, m_meshComponents.end() - 1);
		m_meshComponents.pop_back();
	}

}

/// <summary>
/// ���b�V���R���|�\�l���g�̕`�揈��
/// </summary>
/// <param name="_shaderManager"> �`��ɗp����V�F�[�_�[�N���X </param>
void DrawableObjectManager::Draw(ShaderManager* _shaderManager)
{
	for (auto mc : m_meshComponents)
	{
		mc->Draw(_shaderManager->GetShader(GLSLshader::BASIC_MESH));
	}
}
