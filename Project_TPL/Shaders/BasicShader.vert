//----------------------------------------------------------------------------------+
// @file        BasicShader.vert
// @brief       �x�[�V�b�N�V�F�[�_�[ (���_)
// @note        ���_���W�E�J���[��ǂݍ���
//              �N���b�v���W�����v�Z���A��̃v���Z�X�ɓn��
//              �J���[���͂��̂܂܃t���O�����g�ɏo��
// @author      ���� �N���Y (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#version 420
layout (location = 0) in vec3 a_vertexPos;
layout (location = 1) in vec3 a_color;

out vec3 out_color;

void main()
{

	out_color = a_color;
	gl_Position = vec4(a_vertexPos, 1.0);

}