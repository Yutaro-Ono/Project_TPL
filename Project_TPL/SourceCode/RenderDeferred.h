#pragma once
#include "RenderMethodBase.h"


class RenderDeferred : public RenderMethodBase
{
public:

	RenderDeferred(class Renderer* _renderer, RENDER_METHOD _method);
	~RenderDeferred();

	bool Load() override;

	void Draw(class DrawableObjectManager* _drawObjects) override;


private:

	bool CreateGBuffer();
	bool CreateLightBuffer();

	// シェーダークラスポインタ(生成・解放はシェーダーマネージャークラスに任せる)
	class GLSLprogram* m_basicMeshShader;    // Gバッファ出力シェーダー(標準)
	class GLSLprogram* m_phongShader;         // Gバッファ出力シェーダー(Phongシェーディング)
	class GLSLprogram* m_normalMapShader;     // Gバッファ出力シェーダー(ノーマルマッピング)
	class GLSLprogram* m_skyBoxShader;         // Gバッファ出力スカイボックスシェーダー
	class GLSLprogram* m_dirLightShader;        // ディレクショナルライトシェーダー
	class GLSLprogram* m_visualizeNormalShader;

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