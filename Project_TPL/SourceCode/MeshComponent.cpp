//----------------------------------------------------------------------------------+
// @file        MeshComponent.cpp
// @brief       ���b�V���R���|�[�l���g�N���X
//              
// @note        Component�N���X���p��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/14   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "MeshComponent.h"
#include "Mesh.h"
#include "GameMain.h"
#include "Renderer.h"
#include "MeshPool.h"

MeshComponent::MeshComponent(Actor* _owner)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	
}

MeshComponent::MeshComponent(Actor* _owner, const std::string& _filePath)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	Load(_filePath);
}

MeshComponent::~MeshComponent()
{
}

/// <summary>
/// ���b�V���̃��[�h
/// </summary>
/// <param name="_filePath"></param>
void MeshComponent::Load(const std::string& _filePath)
{
	// ���b�V���v�[�����烁�b�V�����擾
	m_mesh = MESH_POOL->GetMesh(_filePath);
}

/// <summary>
/// �[�x�}�b�v�ɉe��`�悷��
/// </summary>
/// <param name="_depthShader"> �[�x�}�b�v�`��ɗp����V�F�[�_�[ </param>
void MeshComponent::DrawShadow(GLSLprogram* _depthShader)
{
}


/// <summary>
/// ���b�V���̕`�揈��
/// </summary>
/// <param name="_shader"> �`��ɗp����V�F�[�_�[�N���X </param>
void MeshComponent::Draw(GLSLprogram* _shader)
{
	if (m_mesh != nullptr && !m_isVisible)
	{

	}
}
