#pragma once
#include "RenderMethodBase.h"


class RenderForward : public RenderMethodBase
{

public:

	RenderForward(class Renderer* _renderer);
	~RenderForward();

	bool Load() override;

	void Draw(class ShaderManager* _shaderManager, class DrawableObjectManager* _drawObjects) override;


private:

	bool CreateColorBuffer();


	// シェーダークラスポインタ(生成・解放はシェーダーマネージャークラスに任せる)
	class GLSLprogram* m_basicMeshShader;        // Gバッファ出力シェーダー(標準)
	class GLSLprogram* m_phongShader;            // Gバッファ出力シェーダー(Phongシェーディング)
	class GLSLprogram* m_normalMapShader;        // Gバッファ出力シェーダー(ノーマルマッピング)
	class GLSLprogram* m_skyBoxShader;           // Gバッファ出力スカイボックスシェーダー
	class GLSLprogram* m_visualizeNormalShader;  // 法線を視覚化するシェーダー
	class GLSLprogram* m_outScreenShader;        // 画面出力用シェーダー

	// フォワードレンダリング出力用バッファ
	unsigned int m_colorFBO;
	unsigned int m_albedoSpec;
	unsigned int m_emissive;
	unsigned int m_attachments[2];
	unsigned int m_rbo;

};