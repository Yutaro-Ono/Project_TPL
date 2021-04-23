//----------------------------------------------------------------------------------+
// @file        Renderer.cpp
// @brief       �`�揈��
//              ���b�V���E�e�N�X�`���E�V�F�[�_�[���̕ێ�
// @note        
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "Renderer.h"
#include <iostream>
#include "BasicTriangle.h"
#include "ShaderManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
	,m_shaderManager(nullptr)
	,m_triangle(nullptr)
	,m_uboMatrices(0)
	,m_uboCamera(0)
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
	m_viewMat = glm::lookAt(glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(00, 1.0, 0.0));
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
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3::x) + sizeof(glm::vec3::y) + sizeof(glm::vec3::z), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uboCamera, 0, sizeof(glm::vec3::x) + sizeof(glm::vec3::y) + sizeof(glm::vec3::z));

	// �V�F�[�_�[�}�l�[�W���[
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "Error::ShaderManager CreateShaders()" << std::endl;
		return false;
	}

	// �f�o�b�O�p�O�p�`
	m_triangle = new BasicTriangle();

	return true;
}

/// <summary>
/// �������
/// </summary>
void Renderer::Delete()
{
	delete m_triangle;
	delete m_shaderManager;

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
	glViewport(0, 0, 1920, 1080);

	//------------------------------------------------+
    // �`�揈��
    //------------------------------------------------+
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);              // ��ʂ̃J���[�E�[�x�E�X�e���V���o�b�t�@���N���A

	// (�f�o�b�O�p)�O�p�`�̕`��
	//m_shaderManager->EnableShaderProgram(GLSLshader::SIMPLE_POS_COLOR);
	m_shaderManager->EnableShaderProgram(GLSLshader::SIMPLE_POS_TEXTURE);
	m_triangle->Draw(m_shaderManager->GetShader(GLSLshader::SIMPLE_POS_TEXTURE));

	// �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
	glfwSwapBuffers(m_window);
}

/// <summary>
/// uniform�o�b�t�@�ւ̕ϐ��Z�b�g�֐�
/// </summary>
void Renderer::SetUniformBuffer()
{
	// �s��UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m_viewMat);
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), &m_projMat);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �J����UBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);

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
