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
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Tag.h"
#include "Component.h"


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

	Actor(OBJECT_TAG _tag);
	virtual ~Actor();

	void Update(float _deltaTime);
	void UpdateComponents(float _deltaTime);
	virtual void UpdateActor(float _deltaTime);

	void ComputeWorldTransform();

	// コンポーネントの追加・削除処理
	void AddComponent(class Component* _comp);
	void RemoveComponent(class Component* _comp);

	void SetPosition(const glm::vec3& _pos);
	void SetScale(const glm::vec3& _scale);
	void SetEulerAngle(const glm::vec3& _angle);

	const glm::mat4& GetWorldTransform() const { return m_worldTrans; }
	const glm::vec3& GetPosition() const { return m_position; }

protected:

	ActorEnum::ACTOR_STATE m_state;

	// 座標系
	glm::vec3 m_position;
	glm::mat4 m_worldTrans;
	// 変換
	glm::vec3 m_scale;
	// 回転行列
	glm::vec3 m_eulerAngles;
	glm::quat m_rotationX;
	glm::quat m_rotationY;
	glm::quat m_rotationZ;
	
	bool m_recomputeWorldTransform;


	std::vector<class Component*> m_components;           // アクターが保持するコンポーネントクラス

	OBJECT_TAG m_tag;                                     // アクタータグ
	int m_ID;                                             // アクター識別ID
	static int m_globalActorNo;                           // ゲーム全体におけるアクター番号

	
	class ActorDebugObject* m_debugObj;                   // アクター用デバッグオブジェクト

	friend class ActorDebugObject;
};

