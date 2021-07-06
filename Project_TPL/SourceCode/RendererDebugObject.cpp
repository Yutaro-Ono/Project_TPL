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
	// �r���[�|�[�g�ݒ�
	glViewport(0, 0, DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight());
	// �f�o�b�O�o�b�t�@�̏������݁E�`��
	glBindFramebuffer(GL_FRAMEBUFFER, DEBUGGER->GetDebugFrameBuffer());

	// �����_�����O���@�̕ύX
	bool method = (int)m_renderer->m_renderMethodType;
	ImGui::Checkbox("RenderingMode : 0 = Forward, 1 = Deferred", &method);
	if (method == 0)
	{
		m_renderer->SetRenderMethod(RENDER_METHOD::FORWARD);
	}
	else
	{
		m_renderer->SetRenderMethod(RENDER_METHOD::DEFERRED);
	}

	// �u���[�����ʂ̗L���E������
	ImGui::Checkbox("Bloom : 0 = false, 1 = enable", &m_renderer->m_enableBloom);

	// �@���̎��o���̗L����
	ImGui::Checkbox("VisualizeNormal", &m_renderer->m_visualizeNormal);

	if (ImGui::TreeNode("GBuffer : POSITION"))
	{

		// �`�揈�� ------------------------------------------------------------+
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer->m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_renderer->m_gPos);
		//----------------------------------------------------------------------+
		m_renderer->m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui��ɕ\��
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(),
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : NORMAL"))
	{

		// �`�揈�� ------------------------------------------------------------+
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer->m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_renderer->m_gNormal);
		//----------------------------------------------------------------------+
		m_renderer->m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui��ɕ\��
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(),
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GBuffer : ALBEDO"))
	{

		// �`�揈�� ------------------------------------------------------------+
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_renderer->m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_renderer->m_gAlbedoSpec);
		//----------------------------------------------------------------------+
		m_renderer->m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ImGui��ɕ\��
		ImGui::Image((void*)(intptr_t)DEBUGGER->GetDebugColorBuffer(),
			ImVec2(DEBUGGER->GetDebugBufferWidth(), DEBUGGER->GetDebugBufferHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::TreePop();
	}

	// �o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
