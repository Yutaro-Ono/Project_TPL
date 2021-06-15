//----------------------------------------------------------------------------------+
// @file        Renderer.cpp
// @brief       �`�揈��
//              ���b�V���E�e�N�X�`���E�V�F�[�_�[���̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   �V�K�쐬
// 2021/ 5/26   �e��o�b�t�@�̏�����������ǉ� (G�o�b�t�@�E���C�g�EMSAA)
//----------------------------------------------------------------------------------+
#include "Renderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "GameMain.h"
#include "GameSettings.h"
#include "ShaderManager.h"
#include "DrawableObjectManager.h"
#include "Actor.h"
#include "Debugger.h"
#include "Tag.h"
#include "VertexArray.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
	,m_renderMethod(RENDER_METHOD::DEFFERED)
	,m_shaderManager(nullptr)
	,m_drawableObject(nullptr)
	,m_uboMatrices(0)
	,m_uboCamera(0)
	,m_debugObj(nullptr)
	,m_quadVA(nullptr)
	,m_enableBloom(false)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Renderer::~Renderer()
{
	Delete();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="_width"> �X�N���[���̉��� </param>
/// <param name="_height"> �X�N���[���̏c�� </param>
/// <param name="_fullScreen"> �t���X�N���[���ɂ��邩 </param>
/// <returns> �������Ɏ��s�����ꍇfalse��Ԃ� </returns>
bool Renderer::Initialize(int _width, int _height, bool _fullScreen)
{

	//--------------------------------------+
    // GLFW������
    //--------------------------------------+
	if (!glfwInit())
	{
		std::cout << "Error::GLFW Initialize" << std::endl;
		return false;
	}
	// GLFW�\���I�v�V�����̐ݒ�
	// (0�F�I������I�v�V����(�񋓌^), 1�F�I�v�V�����ɐݒ肷��l)
	// ver4.2���g�p�B���o�[�W�������C���X�g�[������Ă��Ȃ��ꍇ�A���삵�Ȃ�
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                   // ���W���[�o�[�W����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                   // �}�C�i�[�o�[�W����
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // �R�A�v���t�@�C�����g�p

	//--------------------------------------+
	// �E�B���h�E�I�u�W�F�N�g��`
	//--------------------------------------+
	// �t���X�N���[����
	if (_fullScreen)
	{
		m_window = glfwCreateWindow(_width, _height, "Project_TPL", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		m_window = glfwCreateWindow(_width, _height, "Project_TPL", NULL, NULL);
	}
	// �E�B���h�E�쐬���s��
	if (m_window == NULL)
	{
		std::cout << "Failed : Create Renderer Window" << std::endl;
		return false;
	}
	// �쐬�����E�B���h�E�����݂̃X���b�h�̃��C���R���e�L�X�g�Ƃ��Đݒ�
	glfwMakeContextCurrent(m_window);

	//---------------------------------------------------+
	// gl3w������ (glfwMakeContextCurrent�֐��̌��)
	//---------------------------------------------------+
	GLenum error = gl3wInit();
	if (GL3W_OK != error)
	{
		std::cout << "Error::GL3W Initialize" << std::endl;
		return false;
	}

	//---------------------------------------+
	// �r���[�|�[�g�̐ݒ�
	//---------------------------------------+
	// �r���[�|�[�g (0x0�̍��W����1920x1080�܂ł�`��͈͂Ƃ��Đݒ�)
	glViewport(0, 0, _width, _height);
	// �E�B���h�E�T�C�Y�ύX���s��ꂽ�ۂɁA�R�[���o�b�N�֐� (����͉�ʃT�C�Y�̍œK���֐�)���Ăяo�����Ƃ��AGLFW�Ɏw��
	glfwSetFramebufferSizeCallback(m_window, FrameBuffer_Size_Callback);

	//---------------------------------------+
	// �s��̏�����
	//---------------------------------------+
	m_viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	m_projMat = glm::perspective(glm::radians(75.0f), (float)_width / (float)_height, 0.1f, 10000.0f);

	//---------------------------------------+
	// uniform�o�b�t�@����
	//---------------------------------------+
	// �r���[�s��E�v���W�F�N�V�����s��UBO
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));
	// �J�������FBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uboCamera, 0, sizeof(glm::vec3::x) + sizeof(glm::vec3::y) + sizeof(glm::vec3::z));

	//---------------------------------------+
	// �ėp���_�z��
	//---------------------------------------+
	// �l�p�`
	m_quadVA = new VertexArray();
	m_quadVA->CreateSimpleQuadVAO();

	//---------------------------------------+
    // �`��o�b�t�@����
    //---------------------------------------+
	CreateGBuffer();
	CreateLightBuffer();
	CreateMSAA();

	// �`����@�̐ݒ�
	if (GAME_CONFIG.GetEnableDeffered())
	{
		m_renderMethod = RENDER_METHOD::DEFFERED;
	}

	// �V�F�[�_�[�}�l�[�W���[
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "Error::ShaderManager CreateShaders()" << std::endl;
		return false;
	}

	// �`��\�I�u�W�F�N�g�Ǘ��N���X
	m_drawableObject = new DrawableObjectManager();



	return true;
}

/// <summary>
/// �����_���[�p�f�o�b�O�I�u�W�F�N�g�̐���
/// </summary>
void Renderer::CreateRendererDebugObject()
{
	m_debugObj = new RendererDebugObject(this);
	DEBUGGER->AddDebugObject(m_debugObj, OBJECT_TAG::SYSTEM);
}

/// <summary>
/// �������
/// </summary>
void Renderer::Delete()
{
	delete m_shaderManager;
	delete m_drawableObject;
	delete m_quadVA;

	// window�̔j���EGLFW�̃N���[���A�b�v
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

/// <summary>
/// �`��֐�
/// </summary>
void Renderer::Draw()
{
	// �`�悷��E�B���h�E���Z�b�g
	glfwMakeContextCurrent(m_window);

	// �r���[�|�[�g�̍X�V
	glViewport(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());

	// uniform�o�b�t�@�̃Z�b�g
	SetUniformBuffer();

	//------------------------------------------------+
	// ForwardShading
	//------------------------------------------------+
	if (m_renderMethod == RENDER_METHOD::FORWARD)
	{

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // ��ʂ̃J���[�E�[�x�E�X�e���V���o�b�t�@���N���A

		// ���b�V���̕`��
		glEnable(GL_DEPTH_TEST);
		m_shaderManager->EnableShaderProgram(GLSLshader::BASIC_MESH);
		m_drawableObject->Draw(m_shaderManager, GLSLshader::BASIC_MESH);
	}

	//------------------------------------------------+
	// DefferedShading
	//------------------------------------------------+
	if (m_renderMethod == RENDER_METHOD::DEFFERED)
	{
		// G�o�b�t�@���o�C���h
		glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

		// �J���[�E�o�b�t�@���̃N���A
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // ��ʂ̃J���[�E�[�x�E�X�e���V���o�b�t�@���N���A
		glEnable(GL_DEPTH_TEST);

		m_shaderManager->EnableShaderProgram(GLSLshader::GBUFFER_BASIC_MESH);
		// �W�I���g���p�X
		m_drawableObject->Draw(m_shaderManager, GLSLshader::GBUFFER_BASIC_MESH);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ���C�e�B���O�p�X
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �X�N���[���ɏo��
		// �u���[����������
		if (m_enableBloom)
		{

		}
		// �u���[���������Ȃ�
		else
		{
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shaderManager->EnableShaderProgram(GLSLshader::OUT_SCREEN_ENTIRE);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
		}
		// �X�N���[����`��
		m_quadVA->SetActive();
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	// �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
	glfwSwapBuffers(m_window);
}

/// <summary>
/// G�o�b�t�@�̊e��v�f�̐����E�o�^
/// </summary>
void Renderer::CreateGBuffer()
{
	// G�o�b�t�@�̐����E�o�C���h
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	
	//--------------------------------------------------------------------+
	// G�o�b�t�@�e�v�f�̓o�^
	//--------------------------------------------------------------------+
	// ���W���o�b�t�@ (���������_�o�b�t�@ / 0�Ԗ�)
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);
	// �@�����o�b�t�@ (���������_�o�b�t�@ / 1�Ԗ�)
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// �A���x�h&�X�y�L�������o�b�t�@ (8bit�J���[�o�b�t�@ / 2�Ԗ�)
	glGenTextures(1, &m_gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);
	// ���P�x�o�b�t�@ (�G�~�b�V�u�o�b�t�@ / 3�Ԗ�)
	glGenTextures(1, &m_gEmissive);
	glBindTexture(GL_TEXTURE_2D, m_gEmissive);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gEmissive, 0);

	// ���������e�v�f��G�o�b�t�@�̕`���Ƃ���GL�ɖ�������
	m_gAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_gAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	m_gAttachments[2] = { GL_COLOR_ATTACHMENT2 };
	m_gAttachments[3] = { GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, m_gAttachments);

	// �����_�[�o�b�t�@�̐����E�o�^
	glGenRenderbuffers(1, &m_gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gRBO);

	// �t���[���o�b�t�@�̐������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::GBuffer::Create Failed" << std::endl;
	}

	// G�o�b�t�@�̃o�C���h������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// ���C�g�o�b�t�@�̊e��v�f�̓o�^
/// </summary>
void Renderer::CreateLightBuffer()
{
	glGenFramebuffers(1, &m_lightBuffer);

	// HDR�o�b�t�@
	glGenTextures(1, &m_lightHDR);
	glBindTexture(GL_TEXTURE_2D, m_lightHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightHDR, 0);
	// ���P�x�o�b�t�@�̍쐬
	glGenTextures(1, &m_lightHighBright);
	glBindTexture(GL_TEXTURE_2D, m_lightHighBright);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_lightHighBright, 0);
	// �A�^�b�`�����g�ݒ�
	m_lightAttachments[0] = { GL_COLOR_ATTACHMENT0 };
	m_lightAttachments[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, m_lightAttachments);

	// �����_�[�o�b�t�@���쐬����
	glGenRenderbuffers(1, &m_lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRBO);

	// �t���[���o�b�t�@�̐��������`�F�b�N
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::LightBuffer::Create Failed" << std::endl;
	}
	// �t���[���o�b�t�@�̃o�C���h����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// �}���`�T���v�����O�A���`�G�C���A�V���O�p�o�b�t�@�̐���
/// </summary>
void Renderer::CreateMSAA()
{
	// MSAA�o�b�t�@�̐����E�o�^
	glGenFramebuffers(1, &m_msaaBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_msaaBuffer);
	// �}���`�T���v���p�J���[�o�b�t�@�𐶐�
	glGenTextures(1, &m_msaaColor);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msaaColor);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH(), GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_msaaColor, 0);
	// MSAA�p�����_�[�o�b�t�@�̐��� (�[�x�E�X�e���V���Ƃ��ēo�^)
	glGenRenderbuffers(1, &m_msaaRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_msaaRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_msaaRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::MSAA BUFFER::Create Failed" << std::endl;
	}
	
	// MSAA�o�b�t�@�̃o�C���h������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/// <summary>
/// uniform�o�b�t�@�ւ̕ϐ��Z�b�g�֐�
/// </summary>
void Renderer::SetUniformBuffer()
{
	// �s��UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_viewMat));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_projMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �J����UBO
	//glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);

}

/// <summary>
/// ��ʂ̃��T�C�Y���s���֐� (�R�[���o�b�N�֐��Ƃ��ă��C�����[�v�Őݒ�)
/// (��ʃT�C�Y�̕ύX���s��ꂽ�ۂ�GLFW�ɂ�莩���I�ɌĂяo�����)
/// </summary>
/// <param name="in_window"> �E�B���h�E�I�u�W�F�N�g�̃|�C���^ </param>
/// <param name="in_windth"> ��ʂ̉��� </param>
/// <param name="in_height"> ��ʂ̏c�� </param>
void Renderer::FrameBuffer_Size_Callback(GLFWwindow* _window, int _width, int _height)
{
	// �r���[�|�[�g�̃��T�C�Y
	glViewport(0, 0, _width, _height);
}
