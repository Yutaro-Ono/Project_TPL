#pragma once
#include "DebugObjectBase.h"
#include "Renderer.h"


class RenderBloomDebugObject : public DebugObjectBase
{

public:

	RenderBloomDebugObject(class RenderBloom* _target);
	~RenderBloomDebugObject();

	void Update(float _deltaTime) override;
	void Render() override;


private:

	class RenderBloom* m_bloom;

};