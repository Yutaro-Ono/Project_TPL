#include "RenderBloomDebugObject.h"
#include "RenderBloom.h"
#include "GameMain.h"
#include "Debugger.h"


RenderBloomDebugObject::RenderBloomDebugObject(RenderBloom* _target)
	:m_bloom(_target)
{
}

RenderBloomDebugObject::~RenderBloomDebugObject()
{
}

void RenderBloomDebugObject::Update(float _deltaTime)
{
	if (ImGui::TreeNode("Bloom Config"))
	{
		Render();

		ImGui::TreePop();
	}
}

void RenderBloomDebugObject::Render()
{
	// ビューポート設定
	glViewport(0, 0, DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight());
	// デバッグバッファの書き込み・描画
	glBindFramebuffer(GL_FRAMEBUFFER, DEBUGGER->GetDebugFrameBuffer());

	// 露出の調整
	float exposureRate = m_bloom->m_exposure;
	ImGui::SliderFloat("Exposure Rate:", &exposureRate, 0.0f, 10.0f);
	if (exposureRate != m_bloom->m_exposure)
	{
		m_bloom->m_exposure = exposureRate;
	}

	// ガンマ調整
	float gammaRate = m_bloom->m_gamma;
	ImGui::SliderFloat("Gamma Rate:", &gammaRate, 0.0f, 10.0f);
	if (gammaRate != m_bloom->m_gamma)
	{
		m_bloom->m_gamma = gammaRate;
	}

	// バインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}