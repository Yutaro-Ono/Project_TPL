//----------------------------------------------------------------------------------+
// @file        Tag.h
// @brief       �^�O�N���X
// @note        �Q�[�����A�N�^�[����ޕ�������
// @author      ���� �N���Y (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 5/28   �V�K�쐬
//----------------------------------------------------------------------------------+
#include "Tag.h"

OBJECT_TAG& operator++(OBJECT_TAG& rhs)
{
    if (rhs == OBJECT_TAG::END)
    {
        throw std::out_of_range("for outof range Tag& operator ++ (Tag&)");
    }
    rhs = OBJECT_TAG(static_cast<std::underlying_type<OBJECT_TAG>::type>(rhs) + 1);

    return rhs;
}
