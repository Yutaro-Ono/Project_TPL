#include "ActorDebugObject.h"
#include "GameMain.h"
#include "Debugger.h"
#include "Actor.h"

/// <summary>
/// アクター用デバッグオブジェクト
/// コンストラクタ
/// </summary>
/// <param name="_owner"> オーナー </param>
ActorDebugObject::ActorDebugObject(Actor* _owner)
	:m_owner(_owner)
{
	m_actorLabel = (char)m_owner->m_tag + " ID : " + std::to_string(m_owner->m_ID);
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

	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(m_actorLabel.c_str()))
	{
		//------------------------------------------------------------------------+
		// 座標
		ImGui::Text(u8"座標");
		Vector3 pos;
		pos.x = m_owner->m_position.x;
		pos.y = m_owner->m_position.y;
		pos.z = m_owner->m_position.z;

		// インプット形式
		std::string label;
		label = "Position X (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &pos.x, 1.0f, 0.0f);
		label = "Position Y (" + m_actorLabel + ")";
		ImGui::InputFloat(label.c_str(), &pos.y, 1.0f, 0.0f);
		label = "Position Z (" + m_actorLabel + ")";
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
		Quaternion rot = m_owner->m_rotation;
		
		label = "Rotation X (" + m_actorLabel + ")";
		ImGui::SliderFloat(label.c_str(), &rot.x, -360.0f, 360.0f);
		label = "Rotation Y (" + m_actorLabel + ")";
		ImGui::SliderFloat(label.c_str(), &rot.y, -360.0f, 360.0f);
		label = "Rotation Z (" + m_actorLabel + ")";
		ImGui::SliderFloat(label.c_str(), &rot.z, -360.0f, 360.0f);

		// 更新されたら
		if (rot.x != m_owner->m_rotation.x || rot.y != m_owner->m_rotation.y || rot.z != m_owner->m_rotation.z)
		{
			m_owner->SetRotation(rot);
		}
		//---------------------------------------------------------------------------+

		//---------------------------------------------------------------------------+
		// スケール
		ImGui::Text(u8"拡大率");
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

		// 更新されたら
		if (scale.x != m_owner->m_scale.x || scale.y != m_owner->m_scale.y || scale.z != m_owner->m_scale.z)
		{
			m_owner->SetScale(scale);
		}
		//----------------------------------------------------------------------------+
		
		// デバッグウィンドウへのメッシュの描画
		Render();

		ImGui::TreePop();
	}

}

void ActorDebugObject::Render()
{
	std::string label = "Mesh Viewer : " + m_actorLabel;

	// 区切り線
	ImGui::Separator();

	if (ImGui::TreeNode(label.c_str()))
	{
		// デバッグバッファの書き込み・描画
		glBindFramebuffer(GL_FRAMEBUFFER, DEBUGGER->GetDebugFrameBuffer());
		glViewport(0, 0, DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight());
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);                    // 指定した色値で画面をクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // 画面のカラー・深度バッファをクリア

		// 描画処理 ------------------------------------------------------------+
		//
		//
		//----------------------------------------------------------------------+

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ImGui上に表示
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(), 
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()), 
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();

	}	
}
