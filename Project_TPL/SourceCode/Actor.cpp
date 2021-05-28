//----------------------------------------------------------------------------------+
// @file        Actor.cpp
// @brief       アクタークラス (座標を持ち、描画されるオブジェクト全般の基底クラス)
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 4/23   新規作成
//----------------------------------------------------------------------------------+
#include "Actor.h"
#include "GameMain.h"
#include "Debugger.h"
#include "ActorPool.h"

int Actor::m_globalActorNo = 0;

Actor::Actor(OBJECT_TAG _tag)
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
	// アクタープールに追加する
	ACTOR_POOL->AddObject(this);

	// ゲーム全体のアクター番号をインクリメント
	m_globalActorNo++;

	ComputeWorldTransform();

	// デバッグオブジェクトの生成
#ifdef _DEBUG

	m_debugObj = new ActorDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj, _tag);

#endif
}

Actor::~Actor()
{

	printf("DELETE::Actor ID = %d\n", m_ID);

	// コンポーネントの削除
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
/// コンポーネントの更新処理
/// </summary>
/// <param name="_deltaTime"></param>
void Actor::UpdateComponents(float _deltaTime)
{
}

/// <summary>
/// アクター固有の更新処理
/// </summary>
/// <param name="_deltaTime"></param>
void Actor::UpdateActor(float _deltaTime)
{
	
}

/// <summary>
/// ワールド変換行列の更新処理
/// </summary>
void Actor::ComputeWorldTransform()
{
	// ワールド変換行列の再計算が必要な場合のみ実行
	if (m_recomputeWorldTransform)
	{
		// 変換行列
		glm::mat4 trans(1.0f);

		// 平行移動
		trans = glm::translate(trans, m_position);
		// 回転
		glm::vec3 radian = glm::radians(m_eulerAngles);
		trans = glm::rotate(trans, radian.x, glm::vec3(1.0, 0.0, 0.0));
		trans = glm::rotate(trans, radian.y, glm::vec3(0.0, 1.0, 0.0));
		trans = glm::rotate(trans, radian.z, glm::vec3(0.0, 0.0, 1.0));
		// スケーリング
		trans = glm::scale(trans, m_scale);

		m_worldTrans = trans;

		// アクターの全コンポーネントも更新
		for (auto comp : m_components)
		{
			comp->OnUpdateTransform();
		}

		m_recomputeWorldTransform = false;
	}
}

/// <summary>
/// コンポーネントの追加処理
/// </summary>
/// <param name="_comp"> 追加するコンポーネント </param>
void Actor::AddComponent(Component* _comp)
{
	// コンポーネントをソートして追加
	// 自分のオーダーより大きい挿入点を検索
	int myOrder = _comp->GetUpdateOrder();
	auto iter = m_components.begin();

	for (; iter != m_components.end(); ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// 要素を見つけた手前に挿入する
	m_components.insert(iter, _comp);
}

/// <summary>
/// コンポーネントの削除処理
/// </summary>
/// <param name="_comp"> 削除したいコンポーネント </param>
void Actor::RemoveComponent(Component* _comp)
{
	// 渡されたコンポーネントを検索し、削除する
	auto iter = std::find(m_components.begin(), m_components.end(), _comp);
	if (iter != m_components.end())
	{
		m_components.erase(iter);
	}
}

/// <summary>
/// 座標のセット
/// ワールド変換行列の再計算のフラグを立てる
/// </summary>
/// <param name="_pos"></param>
void Actor::SetPosition(const glm::vec3& _pos)
{
	m_position = _pos;
	m_recomputeWorldTransform = true;
}

/// <summary>
/// スケールのセット
/// ワールド変換行列の再計算のフラグを立てる
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






/// <summary>
/// アクター用デバッグオブジェクト
/// コンストラクタ
/// </summary>
/// <param name="_owner"> オーナー </param>
ActorDebugObject::ActorDebugObject(Actor* _owner)
	:m_owner(_owner)
{
}

ActorDebugObject::~ActorDebugObject()
{
}

/// <summary>
/// ImGuiによるデバッグ更新処理
/// </summary>
/// <param name="_deltaTime"> デルタタイム </param>
void ActorDebugObject::Update(float _deltaTime)
{
	
	// ラベルとID
	std::string id = std::to_string(m_owner->m_ID);
	std::string label = (char)m_owner->m_tag + " ID : " + id;
	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		//------------------------------------------------------------------------+
		// 座標
		ImGui::Text(u8"座標");
		glm::vec3 pos;
		pos.x = m_owner->m_position.x;
		pos.y = m_owner->m_position.y;
		pos.z = m_owner->m_position.z;

		// インプット形式
		label = "Position X (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &pos.x, 1.0f, 0.0f);
		label = "Position Y (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &pos.y, 1.0f, 0.0f);
		label = "Position Z (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &pos.z, 1.0f, 0.0f);
		
		// 更新されたら
		if (pos.x != m_owner->m_position.x || pos.y != m_owner->m_position.y || pos.z != m_owner->m_position.z)
		{
			m_owner->SetPosition(pos);
		}
		//--------------------------------------------------------------------------+

		//--------------------------------------------------------------------------+
		// 回転
		ImGui::Text(u8"回転");
		glm::vec3 rot = m_owner->m_eulerAngles;

		label = "Rotation X (ID : " + id + ")";
		ImGui::SliderFloat(label.c_str(), &rot.x, -360.0f, 360.0f);
		label = "Rotation Y (ID : " + id + ")";
		ImGui::SliderFloat(label.c_str(), &rot.y, -360.0f, 360.0f);
		label = "Rotation Z (ID : " + id + ")";
		ImGui::SliderFloat(label.c_str(), &rot.z, -360.0f, 360.0f);

		// 更新されたら
		if (rot.x != m_owner->m_eulerAngles.x || rot.y != m_owner->m_eulerAngles.y || rot.z != m_owner->m_eulerAngles.z)
		{
			m_owner->SetEulerAngle(rot);
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// スケール
		ImGui::Text(u8"拡大率");
		glm::vec3 scale;
		scale.x = m_owner->m_scale.x;
		scale.y = m_owner->m_scale.y;
		scale.z = m_owner->m_scale.z;

		label = "Scale X (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &scale.x, 0.005f, 1.0f);
		label = "Scale Y (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &scale.y, 0.005f, 1.0f);
		label = "Scale Z (ID : " + id + ")";
		ImGui::InputFloat(label.c_str(), &scale.z, 0.005f, 1.0f);

		// 更新されたら
		if (scale.x != m_owner->m_scale.x || scale.y != m_owner->m_scale.y || scale.z != m_owner->m_scale.z)
		{
			m_owner->SetScale(scale);
		}
		//----------------------------------------------------------------------------+

		ImGui::TreePop();
	}

}

void ActorDebugObject::Render()
{
}
