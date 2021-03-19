// �C���N���[�h�t�@�C��
// ��glad�̌��glfw���L�q
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height);

int main()
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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Project_TPL", NULL, NULL);
	// �E�B���h�E�쐬���s��
	if (window == NULL)
	{
		std::cout << "Failed : Create Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// �쐬�����E�B���h�E�����݂̃X���b�h�̃��C���R���e�L�X�g�Ƃ��Đݒ�
	glfwMakeContextCurrent(window);

	//---------------------------------------+
	// GLAD������
	//---------------------------------------+
	// OS�ŗL��OpenGL�֐��|�C���^�̃A�h���X�����[�h����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed : Initialize GLAD" << std::endl;
		return -1;
	}

	//---------------------------------------+
	// �r���[�|�[�g�̐ݒ�
	//---------------------------------------+
	// �r���[�|�[�g (0x0�̍��W����1920x1080�܂ł�`��͈͂Ƃ��Đݒ�)
	glViewport(0, 0, 1920, 1080);
	// �E�B���h�E�T�C�Y�ύX���s��ꂽ�ۂɁA�R�[���o�b�N�֐� (����͉�ʃT�C�Y�̍œK���֐�)���Ăяo�����Ƃ��AGLFW�Ɏw��
	glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);

	// ���C�����[�v
	// (GLFW�����炩�̑���ŕ���܂Ń��[�v)
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);   // �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��
		glfwPollEvents();          // �L�[�{�[�h�E�}�E�X�̃g���K�[���m�F
	}
	
	// GLFW�̃N���[���A�b�v
	glfwTerminate();
	
	return 0;
}

/// <summary>
/// ��ʃT�C�Y�̍œK���֐� (�R�[���o�b�N�֐��Ƃ��ă��C�����[�v�Őݒ�)
/// (��ʃT�C�Y�̕ύX���s��ꂽ�ۂ�GLFW�ɂ�莩���I�ɌĂяo�����)
/// </summary>
/// <param name="in_window"> �E�B���h�E�I�u�W�F�N�g�̃|�C���^ </param>
/// <param name="in_windth"> ��ʂ̉��� </param>
/// <param name="in_height"> ��ʂ̏c�� </param>
void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height)
{
	glViewport(0, 0, in_windth, in_height);
}