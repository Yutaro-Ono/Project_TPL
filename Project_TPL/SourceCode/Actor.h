//----------------------------------------------------------------------------------+
// @file        Actor.h
// @brief       アクタークラス (座標を持ち、描画されるオブジェクト全般の基底クラス)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ActorEnum
{
	enum ACTOR_STATE
	{
		ACTIVE,
		PAUSED,
		DEAD,
	};
}

class Actor
{

public:

	Actor();
	virtual ~Actor();

	void Update(float _deltaTime);
	void UpdateComponents(float _deltaTime);
	virtual void UpdateActor(float _deltaTime);

	void CalcWorldTransform();

private:

	ActorEnum::ACTOR_STATE m_state;

	// 座標系
	glm::vec3 m_position;
	glm::mat4 m_worldTrans;
	// 変換
	glm::vec3 m_scale;
	glm::quat m_quaternion;
	

};