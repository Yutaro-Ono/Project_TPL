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
#include "RenderBloom.h"
#include "Actor.h"
#include "Debugger.h"
#include "Tag.h"
#include "VertexArray.h"
#include "CubeMap.h"
#include "DirectionalLight.h"
#include "RenderMethodBase.h"
#include "RenderForward.h"
#include "RenderDeferred.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
	,m_renderMethodType(RENDER_METHOD::DEFERRED)
	,m_shaderManager(nullptr)
	,m_drawableObject(nullptr)
	,m_bloomRender(nullptr)
	,m_skyBox(nullptr)
	,m_uboMatrices(0)
	,m_uboCamera(0)
	,m_uboTriggers(0)
	,m_uboDirLights(0)
	,m_debugObj(nullptr)
	,m_quadVA(nullptr)
	,m_enableBloom(GAME_CONFIG.GetEnableBloom())
	,m_visualizeNormal(false)
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


	return true;
}

bool Renderer::Load()
{

	//---------------------------------------+
	// �ėp���_�z��
	//---------------------------------------+
	// �l�p�`
	m_quadVA = new VertexArray();
	m_quadVA->CreateSimpleQuadVAO();

	//---------------------------------------+
	// �`��o�b�t�@����
	//---------------------------------------+
	CreateMSAA();

	// �u���[�����ʃN���X�̐���
	m_bloomRender = new RenderBloom;

	// �`����@�̐ݒ�
	if (GAME_CONFIG.GetEnableDeferred())
	{
		m_renderMethodType = RENDER_METHOD::DEFERRED;
	}

	// �V�F�[�_�[�}�l�[�W���[
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "Error::ShaderManager CreateShaders()" << std::endl;
		return false;
	}

	//---------------------------------------+
    // uniform�o�b�t�@����
    //---------------------------------------+
    // �r���[�s��E�v���W�F�N�V�����s��UBO
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �J�������UBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �g���K�[UBO
	glGenBuffers(1, &m_uboTriggers);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_uboTriggers);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �f�B���N�V���i�����C�gUBO(vec3��vec4�Ƃ��đ��M)
	glGenBuffers(1, &m_uboDirLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	auto byte = ((4 * sizeof(float)) * 4) + (1 * sizeof(float));
	glBufferData(GL_UNIFORM_BUFFER, ((4 * sizeof(float)) * 4) + (1 * sizeof(float)), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_uboDirLights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	

	// �`��\�I�u�W�F�N�g�Ǘ��N���X
	m_drawableObject = new DrawableObjectManager();

	// skybox
	m_skyBox = new CubeMap("Data/Textures/SkyBox/NightCity/");

	// �f�B���N�V���i�����C�g
	m_dirLight = new DirectionalLight;

	// �e��`�惁�\�b�h�̐���
	m_renderMethods[RENDER_METHOD::FORWARD] = new RenderForward(this);
	if (!m_renderMethods[RENDER_METHOD::FORWARD]->Load())
	{
		return false;
	}

	m_renderMethods[RENDER_METHOD::DEFERRED] = new RenderDeferred(this);
	if (!m_renderMethods[RENDER_METHOD::DEFERRED]->Load())
	{
		return false;
	}

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

	delete m_bloomRender;
	delete m_skyBox;

	// �`�惁�\�b�h�������
	for (auto itr : m_renderMethods)
	{
		delete itr.second;
	}
	m_renderMethods.clear();

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

	// ���݂̕`�惁�\�b�h�ɉ������`�揈�����s��
	m_renderMethods[m_renderMethodType]->Draw(m_shaderManager, m_drawableObject);

	// �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
	glfwSwapBuffers(m_window);
}

/// <summary>
/// �u���[�����ʂ̕t�^���`��
/// </summary>
/// <param name="_highBrightBuffer"> ���P�x������ۑ������o�b�t�@ </param>
/// <param name="_colorBuffer"> �J���[�o�b�t�@ </param>
void Renderer::BloomPass(unsigned int _highBrightBuffer, unsigned int _colorBuffer)
{
	// �J���[�E�o�b�t�@���̃N���A
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
	glClear(GL_COLOR_BUFFER_BIT);                 // ��ʂ̃J���[�E�[�x�E�X�e���V���o�b�t�@���N���A

	// �k���o�b�t�@�v�Z
	m_bloomRender->DownSampling(_highBrightBuffer, m_shaderManager->GetShader(GLSLshader::BLOOM_DOWNSAMPLING), m_quadVA);
	// �K�E�X�ڂ�������
	m_bloomRender->GaussBlur(m_shaderManager->GetShader(GLSLshader::BLOOM_GAUSSIAN_BLUR), m_quadVA);
	// �ŏI�g�[���}�b�s���O & �X�N���[���o��
	m_bloomRender->DrawBlendBloom(_colorBuffer, m_shaderManager->GetShader(GLSLshader::BLOOM_TONEMAPPING), m_quadVA);
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
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glm::vec3 viewPos = glm::vec3(m_viewMat[3][0], m_viewMat[3][1], m_viewMat[3][2]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 4, glm::value_ptr(glm::vec4(viewPos, 0.0f)));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �g���K�[FBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	int bloom = static_cast<int>(GAME_CONFIG.GetEnableBloom());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &bloom);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �f�B���N�V���i�����C�g
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	// ���M���̃X�g���C�h(�V�F�[�_�[���ł�vec4�^�Ƃ��Ď󂯎�� ���������ǂݎ�肪���܂������Ȃ�����)
	auto stride = sizeof(float) * 4;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, stride, glm::value_ptr(glm::vec4(m_dirLight->GetDirection(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride, stride, glm::value_ptr(glm::vec4(m_dirLight->GetDiffuse(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 2, stride, glm::value_ptr(glm::vec4(m_dirLight->GetSpecular(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 3, stride, glm::value_ptr(glm::vec4(m_dirLight->GetAmbient(), 0.0f)));
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 4, sizeof(float), &m_dirLight->GetIntensity());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
