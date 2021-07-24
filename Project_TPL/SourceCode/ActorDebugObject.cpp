#include "ActorDebugObject.h"
#include "GameMain.h"
#include "Debugger.h"
#include "Actor.h"

/// <summary>
/// �A�N�^�[�p�f�o�b�O�I�u�W�F�N�g
/// �R���X�g���N�^
/// </summary>
/// <param name="_owner"> �I�[�i�[ </param>
ActorDebugObject::ActorDebugObject(Actor* _owner)
	:m_owner(_owner)
{
	m_actorLabel = (char)m_owner->m_tag + " ID : " + std::to_string(m_owner->m_ID);
}

ActorDebugObject::~ActorDebugObject()
{
}

/// <summary>
/// ImGui�ɂ��f�o�b�O�X�V����
/// </summary>
/// <param name="_deltaTime"> �f���^�^�C�� </param>
void ActorDebugObject::Update(float _deltaTime)
{

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode(m_actorLabel.c_str()))
	{
		//------------------------------------------------------------------------+
		// ���W
		ImGui::Text(u8"���W");
		Vector3 pos;
		pos.x = m_owner->m_position.x;
		pos.y = m_owner->m_position.y;
		pos.z = m_owner->m_position.z;

		// �C���v�b�g�`��
		std::string label;
		label = "Position X (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &pos.x, 1.0f, 0.0f);
		label = "Position Y (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &pos.y, 1.0f, 0.0f);
		label = "Position Z (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &pos.z, 1.0f, 0.0f);

		// �X�V���ꂽ��
		if (pos.x != m_owner->m_position.x || pos.y != m_owner->m_position.y || pos.z != m_owner->m_position.z)
		{
			m_owner->SetPosition(pos);
		}
		//--------------------------------------------------------------------------+

		//--------------------------------------------------------------------------+
		// ��]
		ImGui::Text(u8"��]");
		Quaternion rot = m_owner->m_rotation;
		
		label = "Rotation X (" + m_actorLabel + ")";
		ImGui::SliderFloat(label.c_str(), &rot.x, -360.0f, 360.0f);
		label = "Rotation Y (" + m_actorLabel + ")";
		ImGui::SliderFloat(label.c_str(), &rot.y, -360.0f, 360.0f);
		label = "Rotation Z (" + m_actorLabel + ")";
		ImGui::SliderFloat(label.c_str(), &rot.z, -360.0f, 360.0f);

		// �X�V���ꂽ��
		if (rot.x != m_owner->m_rotation.x || rot.y != m_owner->m_rotation.y || rot.z != m_owner->m_rotation.z)
		{
			m_owner->SetRotation(rot);
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// �X�P�[��
		ImGui::Text(u8"�g�嗦");
		Vector3 scale;
		scale.x = m_owner->m_scale.x;
		scale.y = m_owner->m_scale.y;
		scale.z = m_owner->m_scale.z;

		label = "Scale X (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &scale.x, 0.005f, 1.0f);
		label = "Scale Y (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &scale.y, 0.005f, 1.0f);
		label = "Scale Z (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &scale.z, 0.005f, 1.0f);

		// �X�V���ꂽ��
		if (scale.x != m_owner->m_scale.x || scale.y != m_owner->m_scale.y || scale.z != m_owner->m_scale.z)
		{
			m_owner->SetScale(scale);
		}
		//----------------------------------------------------------------------------+
		
		// �f�o�b�O�E�B���h�E�ւ̃��b�V���̕`��
		Render();

		ImGui::TreePop();
	}

}

void ActorDebugObject::Render()
{
	std::string label = "Mesh Viewer : " + m_actorLabel;

	// ��؂��
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		// �f�o�b�O�o�b�t�@�̏������݁E�`��
		glBindFramebuffer(GL_FRAMEBUFFER, DEBUGGER->GetDebugFrameBuffer());
		glViewport(0, 0, DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight());
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);                    // �w�肵���F�l�ŉ�ʂ��N���A
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // ��ʂ̃J���[�E�[�x�o�b�t�@���N���A

		// �`�揈�� ------------------------------------------------------------+
		//
		//
		//----------------------------------------------------------------------+

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ImGui��ɕ\��
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(), 
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()), 
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();

	}	
}
