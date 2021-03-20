// �C���N���[�h�t�@�C��
// ��glad�̌��glfw���L�q
// ��gl3w��glfw����ɋL�q
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"


// �֐��v���g�^�C�v�錾
void FrameBuffer_Size_Callback(GLFWwindow* in_window, int in_windth, int in_height);
void ProcessInput(GLFWwindow* in_window);
void UpdateImGui();
void RenderImGui(GLFWwindow* in_window);



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
	glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);


	//----------------------------------------+
    // imgui�Z�b�g�A�b�v
    //----------------------------------------+
    // imgui�R���e�L�X�g�쐬
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// imgui�̃J���[�X�^�C����ݒ�
	ImGui::StyleColorsDark();         // �_�[�N�X�^�C��
	//ImGui::StyleColorsClassic();    // �N���V�b�N�X�^�C��
	// ���{��t�H���g�ɐݒ�
	io.Fonts->AddFontFromFileTTF("Data/Fonts/Debug/mplus-1c-medium.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	// GL��GLSL�Ƀo�C���h
	// ver420���g�p���ł��A410 core �Ƃ��ăZ�b�g
	const char* glsl_version = "#version 410 core";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	// ���C�����[�v
	// (GLFW�����炩�̑���ŕ���܂Ń��[�v)
	while (!glfwWindowShouldClose(window))
	{

		//------------------------------------------------+
        // ���͂ɑ΂��鏈��
        //------------------------------------------------+
		ProcessInput(window);      // ���͂ɂ�鏈��


		//------------------------------------------------+
		// �`�揈��
		//------------------------------------------------+
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);      // �w�肵���F�l�ŉ�ʂ��N���A
		glClear(GL_COLOR_BUFFER_BIT);              // ��ʂ̃J���[�o�b�t�@���N���A
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glClear(GL_STENCIL_BUFFER_BIT);


#ifdef _DEBUG

		//------------------------------------------------+
		// imgui�X�V�E�`�揈��
		//------------------------------------------------+
		UpdateImGui();
		RenderImGui(window);


#elif _RELEASE


#endif







		//------------------------------------------------+
		// GL�C�x���g�̍X�V�ƃo�b�t�@�̌���
		//------------------------------------------------+
		glfwPollEvents();          // �L�[�{�[�h�E�}�E�X�̃g���K�[���m�F
		glfwSwapBuffers(window);   // �V�����J���[�o�b�t�@���Â��o�b�t�@�ƌ������A��ʂɕ\��




	}
	
	// imgui�̃N���[���A�b�v
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// GLFW�̃N���[���A�b�v
	glfwDestroyWindow(window);
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

/// <summary>
/// ���̓v���Z�X
/// </summary>
/// <param name="in_window"> �E�B���h�E�I�u�W�F�N�g�̃|�C���^ </param>
void ProcessInput(GLFWwindow* in_window)
{
	// ESCAPE�������ꂽ��
	// �E�B���h�E�I���t���O��ON����
	if (glfwGetKey(in_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(in_window, true);
	}

}

/// <summary>
/// imgui�̍X�V����
/// </summary>
void UpdateImGui()
{
	// imgui�̐V�K�t���[�����J�n����
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// �E�B���h�E�^�C�g��
	// �����{��t�H���g��u8���e�����Ő������\���ł���
	ImGui::Begin(u8"�f�o�b�O���");

	// ����
	ImGui::Text("index");



	ImGui::End();
}

/// <summary>
/// imgui�̕`�揈��
/// </summary>
/// <param name="in_window"> �`�悷��E�B���h�E </param>
void RenderImGui(GLFWwindow* in_window)
{
	ImGui::Render();

	int w = 1920;
	int h = 1080;
	// �t���[���o�b�t�@�T�C�Y�̎擾
	glfwGetFramebufferSize(in_window, &w, &h);
	// �r���[�|�[�g�̐ݒ�
	glViewport(0, 0, w, h);

	// �t���[���o�b�t�@��ImGui�̕`�挋�ʂ������o��
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}