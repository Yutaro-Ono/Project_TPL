#include "RenderMethodBase.h"

RenderMethodBase::RenderMethodBase(class Renderer* _owner, RENDER_METHOD _method)
	:m_owner(_owner)
	,m_method(_method)
{
}
