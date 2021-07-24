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
#include "ActorDebugObject.h"

int Actor::m_globalActorNo = 0;

Actor::Actor(OBJECT_TAG _tag)
	:m_state(ActorEnum::ACTOR_STATE::ACTIVE)
	,m_position(Vector3::Zero)
	,m_scale(Vector3(1.0f, 1.0f, 1.0f))
	,m_rotation(Quaternion::Identity)
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
	,m_debugObj(nullptr)
{
	// �A�N�^�[�v�[���ɒǉ�����
	ACTOR_POOL->AddObject(this);

	// �Q�[���S�̂̃A�N�^�[�ԍ����C���N�������g
	m_globalActorNo++;

	ComputeWorldTransform();

	// �f�o�b�O�I�u�W�F�N�g�̐���
#ifdef _DEBUG

	m_debugObj = new ActorDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj, _tag);

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
	// �A�N�^�[���A�N�e�B�u�Ȃ�
	if (m_state == ActorEnum::ACTOR_STATE::ACTIVE)
	{
		// ���[���h�ϊ��s����v�Z
		ComputeWorldTransform();

		// �R���|�[�l���g�̍X�V����
		UpdateComponents(_deltaTime);
		// �A�N�^�[�ŗL�̍X�V����
		UpdateActor(_deltaTime);

		// ���[���h�ϊ��s��̍Čv�Z
		ComputeWorldTransform();
	}
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
		// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
		m_worldTrans = Matrix4::CreateScale(m_scale);
		m_worldTrans *= Matrix4::CreateFromQuaternion(m_rotation);
		m_worldTrans *= Matrix4::CreateTranslation(m_position);

		// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ����s��
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
void Actor::SetPosition(const Vector3& _pos)
{
	m_position = _pos;
	m_recomputeWorldTransform = true;
}

/// <summary>
/// �X�P�[���̃Z�b�g
/// ���[���h�ϊ��s��̍Čv�Z�̃t���O�𗧂Ă�
/// </summary>
/// <param name="_scale"></param>
void Actor::SetScale(const Vector3& _scale)
{
	m_scale = _scale;
	m_recomputeWorldTransform = true;
}

void Actor::RotateToNewForward(const Vector3& in_forward)
{
	// X���x�N�g��(1, 0, 0)��forward�Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, in_forward);
	float angle = Math::Acos(dot);

	// �������������ꍇ
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// ���x�N�g����forward�Ƃ̊O�ς����]�������߂ĉ�]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, in_forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

void Actor::SetRotation(const Quaternion& _rot)
{
	m_rotation = _rot;
	m_recomputeWorldTransform = true;
}

