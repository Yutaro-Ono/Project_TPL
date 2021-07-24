//----------------------------------------------------------------------------------+
// @file        CameraActor.cpp
// @brief       カメラアクタークラス
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 6/04   新規作成
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
	// ターゲットアクターがいない
	if (m_targetActor == nullptr)
	{
		m_targetExists = false;
	}

	// ターゲットがいる場合
	if (m_targetExists)
	{

	}
	// いない場合
	else
	{

	}

	//m_viewMat = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

/// <summary>
/// カメラにターゲットとなるアクターをセットする
/// カメラのターゲット座標とそれに伴う自身の座標を更新する
/// ビュー行列を更新する
/// レンダラーにビュー行列を登録する
/// </summary>
/// <param name="_target"></param>
void CameraActor::SetTargetActor(Actor* _target)
{
	// ターゲットをセット+座標を更新
	m_targetActor = _target;
	m_position = m_targetActor->GetPosition() + m_length;
	// ビュー行列作成
	CreateViewMatrix();
	// レンダラーに作成したビュー行列をセット
	RENDERER->SetViewMatrix(m_viewMat);
	// ターゲットが存在している
	m_targetExists = true;
}

/// <summary>
/// ビュー行列の生成
/// </summary>
void CameraActor::CreateViewMatrix()
{
}
