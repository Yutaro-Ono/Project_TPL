//----------------------------------------------------------------------------------+
// @file        CameraActor.cpp
// @brief       �J�����A�N�^�[�N���X
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "CameraActor.h"
#include "GameMain.h"
#include "Renderer.h"

const Vector3 CAMERA_LENGTH = Vector3(2000.0f, 0.0f, 500.0f);
const float CAMERA_ROTATE_ANGLE = -Math::PiOver2;
const float CAMERA_UP_ANGLE = Math::PiOver2 * 0.5f;
const float CAMERA_ROTATE_SPEED = 40.0f;

CameraActor::CameraActor()
	:Actor(OBJECT_TAG::CAMERA)
	,m_targetExists(false)
	,m_targetActor(nullptr)
	,m_length(CAMERA_LENGTH)
	,m_targetPos(Vector3::Zero)
	,m_rotateAngle(1.0f)
	,m_upAngle(1.0f)
{
	//m_viewMat = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

CameraActor::~CameraActor()
{
}


void CameraActor::UpdateActor(float _deltaTime)
{
	// �^�[�Q�b�g�A�N�^�[�����Ȃ�
	if (m_targetActor == nullptr)
	{
		m_targetExists = false;
	}

	// �^�[�Q�b�g������ꍇ
	if (m_targetExists)
	{

	}
	// ���Ȃ��ꍇ
	else
	{

	}

	//m_viewMat = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

/// <summary>
/// �J�����Ƀ^�[�Q�b�g�ƂȂ�A�N�^�[���Z�b�g����
/// �J�����̃^�[�Q�b�g���W�Ƃ���ɔ������g�̍��W���X�V����
/// �r���[�s����X�V����
/// �����_���[�Ƀr���[�s���o�^����
/// </summary>
/// <param name="_target"></param>
void CameraActor::SetTargetActor(Actor* _target)
{
	// �^�[�Q�b�g���Z�b�g+���W���X�V
	m_targetActor = _target;
	m_position = m_targetActor->GetPosition() + m_length;
	// �r���[�s��쐬
	CreateViewMatrix();
	// �����_���[�ɍ쐬�����r���[�s����Z�b�g
	RENDERER->SetViewMatrix(m_viewMat);
	// �^�[�Q�b�g�����݂��Ă���
	m_targetExists = true;
}

/// <summary>
/// �r���[�s��̐���
/// </summary>
void CameraActor::CreateViewMatrix()
{
}
