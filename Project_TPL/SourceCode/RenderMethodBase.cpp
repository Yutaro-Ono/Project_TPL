#include "RenderMethodBase.h"

RenderMethodBase::RenderMethodBase(class Renderer* _renderer, RENDER_METHOD _method)
	:m_renderer(_renderer)
	,m_method(_method)
	,m_shader(m_renderer->GetShaderManager())
{
}
