#pragma once
#include "DebugObjectBase.h"
#include "Renderer.h"


class RendererDebugObject : public DebugObjectBase
{

public:

	RendererDebugObject(class Renderer* _target);
	~RendererDebugObject();


	void Update(float _deltaTime) override;
	void Render() override;



private:


	class Renderer* m_renderer;




};