//----------------------------------------------------------------------------------+
// @file        Actor.cpp
// @brief       �A�N�^�[�N���X (���W�������A�`�悳���I�u�W�F�N�g�S�ʂ̊��N���X)
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "Actor.h"
#include "GameMain.h"
#include "Debugger.h"
#include "ActorPool.h"

int Actor::m_globalActorNo = 0;

Actor::Actor()
	:m_state(ActorEnum::ACTOR_STATE::ACTIVE)
	,m_worldTrans(glm::mat4(1.0f))
	,m_position(glm::vec3(1.0f))
	,m_scale(glm::vec3(1.0f))
	,m_eulerAngles(glm::vec3(1.0f))
	,m_rotationX(glm::quat(glm::vec3(0.0f)))
	,m_rotationY(glm::quat(glm::vec3(0.0f)))
	,m_rotationZ(glm::quat(glm::vec3(0.0f)))
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
	,m_debugObj(nullptr)
{
	// �A�N�^�[�v�[���ɒǉ�����
	ACTOR_POOL->AddObject(this);

	// �Q�[���S�̂̃A�N�^�[�ԍ����C���N�������g
	m_globalActorNo++;

	ComputeWorldTransform();


#ifdef _DEBUG

	m_debugObj = new ActorDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj);

#endif
}

Actor::~Actor()
{

	printf("DELETE::Actor ID = %d\n", m_ID);

	// �R���|�[�l���g�̍폜
	while (!m_components.empty())
	{
		delete m_components.back();
	}

}

void Actor::Update(float _deltaTime)
{
	ComputeWorldTransform();
}

/// <summary>
/// �R���|�[�l���g�̍X�V����
/// </summary>
/// <param name="_deltaTime"></param>
void Actor::UpdateComponents(float _deltaTime)
{
}

/// <summary>
/// �A�N�^�[�ŗL�̍X�V����
/// </summary>
/// <param name="_deltaTime"></param>
void Actor::UpdateActor(float _deltaTime)
{
	
}

/// <summary>
/// ���[���h�ϊ��s��̍X�V����
/// </summary>
void Actor::ComputeWorldTransform()
{
	// ���[���h�ϊ��s��̍Čv�Z���K�v�ȏꍇ�̂ݎ��s
	if (m_recomputeWorldTransform)
	{

		glm::mat4 trans(1.0f);

		// ���s�ړ�
		trans = glm::translate(trans, m_position);
		// ��]
		glm::vec3 radian = glm::radians(m_eulerAngles);
		trans = glm::rotate(trans, radian.x, glm::vec3(1.0, 0.0, 0.0));
		trans = glm::rotate(trans, radian.y, glm::vec3(0.0, 1.0, 0.0));
		trans = glm::rotate(trans, radian.z, glm::vec3(0.0, 0.0, 1.0));
		// �X�P�[�����O
		trans = glm::scale(trans, m_scale);

		m_worldTrans = trans;

		// ��]
		//trans *= m_rotationX * m_rotationY * m_rotationZ;

		// �A�N�^�[�̑S�R���|�[�l���g���X�V
		for (auto comp : m_components)
		{
			comp->OnUpdateTransform();
		}

		m_recomputeWorldTransform = false;
	}
}

/// <summary>
/// �R���|�[�l���g�̒ǉ�����
/// </summary>
/// <param name="_comp"> �ǉ�����R���|�[�l���g </param>
void Actor::AddComponent(Component* _comp)
{
	// �R���|�[�l���g���\�[�g���Ēǉ�
	// �����̃I�[�_�[���傫���}���_������
	int myOrder = _comp->GetUpdateOrder();
	auto iter = m_components.begin();

	for (; iter != m_components.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// �v�f����������O�ɑ}������
	m_components.insert(iter, _comp);
}

/// <summary>
/// �R���|�[�l���g�̍폜����
/// </summary>
/// <param name="_comp"> �폜�������R���|�[�l���g </param>
void Actor::RemoveComponent(Component* _comp)
{
	// �n���ꂽ�R���|�[�l���g���������A�폜����
	auto iter = std::find(m_components.begin(), m_components.end(), _comp);
	if (iter != m_components.end())
	{
		m_components.erase(iter);
	}
}

/// <summary>
/// ���W�̃Z�b�g
/// ���[���h�ϊ��s��̍Čv�Z�̃t���O�𗧂Ă�
/// </summary>
/// <param name="_pos"></param>
void Actor::SetPosition(const glm::vec3& _pos)
{
	m_position = _pos;
	m_recomputeWorldTransform = true;
}

/// <summary>
/// �X�P�[���̃Z�b�g
/// ���[���h�ϊ��s��̍Čv�Z�̃t���O�𗧂Ă�
/// </summary>
/// <param name="_scale"></param>
void Actor::SetScale(const glm::vec3& _scale)
{
	m_scale = _scale;
	m_recomputeWorldTransform = true;
}

void Actor::SetEulerAngle(const glm::vec3& _angle)
{
	m_eulerAngles = _angle;
	m_recomputeWorldTransform = true;
}







ActorDebugObject::ActorDebugObject(Actor* _owner)
	:m_owner(_owner)
	,m_isShowDebug(true)
{
}

ActorDebugObject::~ActorDebugObject()
{
}

void ActorDebugObject::Update(float _deltaTime)
{

	// ���x����ID
	std::string label;
	std::string id = std::to_string(m_owner->m_ID);

	label = "Actor ID : " + id;

	// ��؂��
	ImGui::Separator();

	// �f�o�b�O��ʂ�\�����邩�ǂ���
	ImGui::Checkbox(label.c_str(), &m_isShowDebug);

	if (m_isShowDebug)
	{
		// ���W
		ImGui::Text(u8"���W");
		glm::vec3 pos;
		pos.x = m_owner->m_position.x;
		pos.y = m_owner->m_position.y;
		pos.z = m_owner->m_position.z;

		label = "x (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &pos.x, 0.1f, 0.0f);
		label = "y (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &pos.y, 0.1f, 0.0f);
		label = "z (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &pos.z, 0.1f, 0.0f);

		if (pos.x != m_owner->m_position.x || pos.y != m_owner->m_position.y || pos.z != m_owner->m_position.z)
		{
			m_owner->SetPosition(pos);
		}

		// ��]
		ImGui::Text(u8"��]");
		glm::vec3 rot = m_owner->m_eulerAngles;

		label = "rx (ID : " + id + ")";
		ImGui::SliderFloat(label.c_str(), &rot.x, -360.0f, 360.0f);
		label = "ry (ID : " + id + ")";
		ImGui::SliderFloat(label.c_str(), &rot.y, -360.0f, 360.0f);
		label = "rz (ID : " + id + ")";
		ImGui::SliderFloat(label.c_str(), &rot.z, -360.0f, 360.0f);

		if (rot.x != m_owner->m_eulerAngles.x || rot.y != m_owner->m_eulerAngles.y || rot.z != m_owner->m_eulerAngles.z)
		{
			m_owner->SetEulerAngle(rot);
		}


		// �X�P�[��
		ImGui::Text(u8"�g�嗦");
		glm::vec3 scale;
		scale.x = m_owner->m_scale.x;
		scale.y = m_owner->m_scale.y;
		scale.z = m_owner->m_scale.z;

		label = "sx (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &scale.x, 0.05f, 1.0f);
		label = "sy (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &scale.y, 0.05f, 1.0f);
		label = "sz (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &scale.z, 0.05f, 1.0f);

		if (scale.x != m_owner->m_scale.x || scale.y != m_owner->m_scale.y || scale.z != m_owner->m_scale.z)
		{
			m_owner->SetScale(scale);
		}
	}

}

void ActorDebugObject::Render()
{
}
