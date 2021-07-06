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

	// G�o�b�t�@
	unsigned int m_gBuffer;
	// G�o�b�t�@�v�f
	unsigned int m_gPos;
	unsigned int m_gNormal;
	unsigned int m_gAlbedoSpec;
	unsigned int m_gEmissive;
	unsigned int m_gAttachments[4];
	unsigned int m_gRBO;

	// ���C�g�o�b�t�@
	unsigned int m_lightBuffer;
	// ���C�g�o�b�t�@�v�f
	unsigned int m_lightHDR;
	unsigned int m_lightHighBright;
	unsigned int m_lightRBO;
	unsigned int m_lightAttachments[2];

};