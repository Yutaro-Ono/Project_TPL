#pragma once
#include "RenderMethodBase.h"


class RenderDeferred : public RenderMethodBase
{
public:

	RenderDeferred(class Renderer* _owner, RENDER_METHOD _method);
	~RenderDeferred();

	bool Load() override;

	bool Draw() override;


private:

	bool CreateGBuffer();
	bool CreateLightBuffer();

	// Gバッファ
	unsigned int m_gBuffer;
	// Gバッファ要素
	unsigned int m_gPos;
	unsigned int m_gNormal;
	unsigned int m_gAlbedoSpec;
	unsigned int m_gEmissive;
	unsigned int m_gAttachments[4];
	unsigned int m_gRBO;

	// ライトバッファ
	unsigned int m_lightBuffer;
	// ライトバッファ要素
	unsigned int m_lightHDR;
	unsigned int m_lightHighBright;
	unsigned int m_lightRBO;
	unsigned int m_lightAttachments[2];

};