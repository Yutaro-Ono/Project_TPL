#include "RendererDebugObject.h"
#include "GameMain.h"
#include "Debugger.h"
#include "ShaderManager.h"
#include "VertexArray.h"

RendererDebugObject::RendererDebugObject(Renderer* _target)
	:m_renderer(_target)
{
}

RendererDebugObject::~RendererDebugObject()
{
}

void RendererDebugObject::Update(float _deltaTime)
{

	if (ImGui::TreeNode("Renderer"))
	{
		Render();

		ImGui::TreePop();
	}


}

void RendererDebugObject::Render()
{
	// ビューポート設定
	glViewport(0, 0, DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight());
	// デバッグバッファの書き込み・描画
	glBindFramebuffer(GL_FRAMEBUFFER, DEBUGGER->GetDebugFrameBuffer());




	if (ImGui::TreeNode("GBuffer : POSITION"))
	{

		// 描画処理 ------------------------------------------------------------+
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer->m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_renderer->m_gPos);
		//----------------------------------------------------------------------+
		m_renderer->m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui上に表示
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(),
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : NORMAL"))
	{

		// 描画処理 ------------------------------------------------------------+
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer->m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_renderer->m_gNormal);
		//----------------------------------------------------------------------+
		m_renderer->m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui上に表示
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(),
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : ALBEDO"))
	{

		// 描画処理 ------------------------------------------------------------+
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer->m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_renderer->m_gAlbedoSpec);
		//----------------------------------------------------------------------+
		m_renderer->m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui上に表示
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(),
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	// バインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
