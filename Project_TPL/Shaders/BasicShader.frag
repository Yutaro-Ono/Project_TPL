//----------------------------------------------------------------------------------+
// @file        BasicShader.frag
// @brief       �x�[�V�b�N�V�F�[�_�[ (�t���O�����g)
// @note        �N���b�v���W���1�s�N�Z���̃J���[���v�Z���A�o�͂���
// @author      ���� �N���Y (Yutaro Ono, @2021)
//----------------------------------------------------------------------------------+
#version 420
out vec4 out_fragColor;

in vec3 in_color;

void main()
{
	out_fragColor = vec4(in_color, 1.0);
}