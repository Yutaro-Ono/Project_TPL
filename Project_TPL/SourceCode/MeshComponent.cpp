//----------------------------------------------------------------------------------+
// @file        MeshComponent.cpp
// @brief       ���b�V���R���|�[�l���g�N���X
//              
// @note        Component�N���X���p��
//              �A�N�^�[�Ƀ��b�V������R�t����
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
#include "GLSLprogram.h"
#include "VertexArray.h"
#include "DrawableObjectManager.h"

MeshComponent::MeshComponent(Actor* _owner)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	RENDERER->GetDrawableObjectManager()->AddMeshComp(this);
}

MeshComponent::MeshComponent(Actor* _owner, const std::string& _filePath)
	:Component(_owner)
	,m_mesh(nullptr)
	,m_isVisible(false)
	,m_isDrawShadow(true)
	,m_isDrawHUD(true)
	,m_hudColor(glm::vec3(1.0))
{
	RENDERER->GetDrawableObjectManager()->AddMeshComp(this);
	Load(_filePath);
}

MeshComponent::~MeshComponent()
{
	RENDERER->GetDrawableObjectManager()->DeleteMeshComp(this);
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
		// ���[���h�ϊ��s����V�F�[�_�[�ɑ���
		_shader->SetUniform("u_worldTransform", m_owner->GetWorldTransform());


		// �}�e���A�����̃o�C���h
		SetMaterialToUniform();

		// ���_�z��I�u�W�F�N�g�̃A�N�e�B�u��
		m_mesh->SetActiveVAO();

		// �`��
		glDrawElements(GL_TRIANGLES, m_mesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

/// <summary>
/// �}�e���A��(�e�N�X�`��)���w�菇�Ƀo�C���h
/// </summary>
void MeshComponent::SetMaterialToUniform()
{
	// Albedo �� Normal �� Metallic �� Roughness �� AO �� Depth
	// �w�肵���^�C�v�̃e�N�X�`�����Ȃ��ꍇ�͖������l"0"���Z�b�g
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::ALBEDO));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::NORMAL));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::METALLIC));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::ROUGHNESS));

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::AO));

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::SPECULAR));

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_mesh->GetTextureID(PBR_MATERIAL::EMISSIVE));
}
