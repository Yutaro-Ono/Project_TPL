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
#include "ActorPool.h"

int Actor::m_globalActorNo = 0;

Actor::Actor()
	:m_state(ActorEnum::ACTOR_STATE::ACTIVE)
	,m_worldTrans(glm::mat4(1.0f))
	,m_position(glm::vec3(1.0f))
	,m_scale(glm::vec3(1.0f))
	,m_rotationX(glm::quat(glm::vec3(0.0f)))
	,m_rotationY(glm::quat(glm::vec3(0.0f)))
	,m_rotationZ(glm::quat(glm::vec3(0.0f)))
	,m_recomputeWorldTransform(true)
	,m_ID(m_globalActorNo)
{
	// アクタープールに追加する
	ACTOR_POOL->AddObject(this);

	// ゲーム全体のアクター番号をインクリメント
	m_globalActorNo++;

	ComputeWorldTransform();
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

		// 平行移動
		m_worldTrans = glm::translate(m_worldTrans, m_position);
		// スケーリング
		m_worldTrans = glm::scale(m_worldTrans, m_scale);

		// 回転
		//trans *= m_rotationX * m_rotationY * m_rotationZ;

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
