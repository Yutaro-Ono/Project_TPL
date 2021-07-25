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
#include <gtc/type_ptr.hpp>
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
    // SDL�ɂ��GL�����ݒ�
    //--------------------------------------+
	// OpenGL �A�g���r���[�g�̃Z�b�g
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// GL version 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	// 8Bit RGBA �`�����l��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// �_�u���o�b�t�@�����O�L����
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// �n�[�h�E�F�A�A�N�Z�����[�V��������������
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);



	//--------------------------------------+
	// �E�B���h�E�I�u�W�F�N�g��`
	//--------------------------------------+
	// Window�̍쐬
	m_window = SDL_CreateWindow
	(
		"Project_TPL",
		0,
		0,
		_width, 
		_height, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (!m_window)
	{
		printf("Window�̍쐬�Ɏ��s: %s", SDL_GetError());
		return false;
	}
	if (_fullScreen)
	{
		if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) �T�C�Y�̃t���X�N���[�����Ɏ��s\n", _width, _height);
			return false;
		}
		glViewport(0, 0, _width, _height);
	}

	// OpenGLContext�̍쐬
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);

	//SDLRenderer�̍쐬
	m_sdlRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_sdlRenderer)
	{
		printf("SDLRenderer�̍쐬�Ɏ��s : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNG�̏������Ɏ��s : %s", SDL_GetError());
		return false;
	}


	//--------------------------------------+
	// GLEW������
	//--------------------------------------+
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error::GLEW Initialize" << std::endl;
		return false;
	}
	// ����̃v���b�g�z�[���ł́AGLEW�����Q�ȃG���[�R�[�h��f���̂ŃN���A���Ă���
	glGetError();


	//---------------------------------------+
	// �r���[�|�[�g�̐ݒ�
	//---------------------------------------+
	// �r���[�|�[�g (0x0�̍��W����1920x1080�܂ł�`��͈͂Ƃ��Đݒ�)
	glViewport(0, 0, _width, _height);
	// �J�����O�̐ݒ�
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//---------------------------------------+
    // �s��̏�����
    //---------------------------------------+
	m_viewMat = Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, -20.0f), Vector3::Zero, Vector3::UnitZ);
	m_projMat = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(_width),
		static_cast<float>(_height),
		1.0f, 5000.0f);

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
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Matrix4::mat), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �J�������UBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z), NULL, GL_DYNAMIC_DRAW);
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
	glBufferData(GL_UNIFORM_BUFFER, (4 * (sizeof(Vector3::x) + sizeof(Vector3::y) + sizeof(Vector3::z))) + (1 * sizeof(float)), NULL, GL_DYNAMIC_DRAW);
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

	// SDL�̃N���[���A�b�v
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

/// <summary>
/// �`��֐�
/// </summary>
void Renderer::Draw()
{
	// �R���e�L�X�g�����C����ʂƍă����N
	SDL_GL_MakeCurrent(m_window, m_context);
	
	// �r���[�|�[�g�̍X�V
	glViewport(0, 0, GAME_CONFIG.GetScreenSizeW(), GAME_CONFIG.GetScreenSizeH());

	// uniform�o�b�t�@�̃Z�b�g
	SetUniformBuffer();

	// ���݂̕`�惁�\�b�h�ɉ������`�揈�����s��
	m_renderMethods[m_renderMethodType]->Draw(m_shaderManager, m_drawableObject);

	// ��ʂ̃X���b�v
	SDL_GL_SwapWindow(m_window);
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
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4::mat), m_viewMat.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(Matrix4::mat), sizeof(Matrix4::mat), m_projMat.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �J����UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	Vector3 viewPos = m_viewMat.GetTranslation();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 4, viewPos.GetAsFloatPtr());
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
	glBufferSubData(GL_UNIFORM_BUFFER, 0, stride, m_dirLight->GetDirection().GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, stride, stride, m_dirLight->GetDiffuse().GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 2, stride, m_dirLight->GetSpecular().GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, stride * 3, stride, m_dirLight->GetAmbient().GetAsFloatPtr());
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
void Renderer::FrameBuffer_Size_Callback(SDL_Window* _window, int _width, int _height)
{
	// �r���[�|�[�g�̃��T�C�Y
	glViewport(0, 0, _width, _height);
}
