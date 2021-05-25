#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

class DebugObjectBase
{

public:

	DebugObjectBase();
	virtual~DebugObjectBase();

	virtual void Update(float _deltaTime) = 0;

	virtual void Render() = 0;


protected:



};
