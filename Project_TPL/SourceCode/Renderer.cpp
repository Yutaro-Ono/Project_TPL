#include "Renderer.h"
#include <iostream>

/// <summary>
/// �R���X�g���N�^
/// </summary>
Renderer::Renderer()
	:m_window(NULL)
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
/// <returns> ���������s����false��Ԃ� </returns>
bool Renderer::Initialize()
{

	//--------------------------------------+
    // GLFW������
    //--------------------------------------+
	glfwInit();
	// GLFW�\���I�v�V�����̐ݒ�
	// (0�F�I������I�v�V����(�񋓌^), 1�F�I�v�V�����ɐݒ肷��l)
	// ver4.2���g�p�B���o�[�W�������C���X�g�[������Ă��Ȃ��ꍇ�A���삵�Ȃ�
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                   // ���W���[�o�[�W����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                   // �}�C�i�[�o�[�W����
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // �R�A�v���t�@�C�����g�p

	//--------------------------------------+
	// �E�B���h�E�I�u�W�F�N�g��`
	//--------------------------------------+
	// �E�B���h�E�쐬 (1920x1080)
	m_window = glfwCreateWindow(1920, 1080, "Project_TPL", NULL, NULL);
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
	gl3wInit();

	//---------------------------------------+
	// �r���[�|�[�g�̐ݒ�
	//---------------------------------------+
	// �r���[�|�[�g (0x0�̍��W����1920x1080�܂ł�`��͈͂Ƃ��Đݒ�)
	glViewport(0, 0, 1920, 1080);
	// �E�B���h�E�T�C�Y�ύX���s��ꂽ�ۂɁA�R�[���o�b�N�֐� (����͉�ʃT�C�Y�̍œK���֐�)���Ăяo�����Ƃ��AGLFW�Ɏw��
	glfwSetFramebufferSizeCallback(m_window, FrameBuffer_Size_Callback);

	return true;
}

/// <summary>
/// �������
/// </summary>
void Renderer::Delete()
{
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
	glClear(GL_COLOR_BUFFER_BIT);              // ��ʂ̃J���[�o�b�t�@���N���A
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_STENCIL_BUFFER_BIT);

	// �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
	glfwSwapBuffers(m_window);
}

/// <summary>
/// ��ʂ̃��T�C�Y���s���֐� (�R�[���o�b�N�֐��Ƃ��ă��C�����[�v�Őݒ�)
/// (��ʃT�C�Y�̕ύX���s��ꂽ�ۂ�GLFW�ɂ�莩���I�ɌĂяo�����)
/// </summary>
/// <param name="in_window"> �E�B���h�E�I�u�W�F�N�g�̃|�C���^ </param>
/// <param name="in_windth"> ��ʂ̉��� </param>
/// <param name="in_height"> ��ʂ̏c�� </param>
void Renderer::FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height)
{
	// �r���[�|�[�g�̃��T�C�Y
	glViewport(0, 0, in_windth, in_height);
}
