//----------------------------------------------------------------------------------+
// @file        Renderer.h
// @brief       描画処理
//              メッシュ・テクスチャ・シェーダー情報の保持
// @note        
// @author      小野 湧太郎 (Yutaro Ono, @2021)
//
// @changelog
// 2021/ 3/22   新規作成
// 2021/ 5/26   各種バッファを登録 (Gバッファ・ライト・MSAA)
//----------------------------------------------------------------------------------+
#pragma once
// インクルードファイル
// ※gl3w→glfwの順番に記述
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Effekseer.h>
#include <EffekseerRendererGL.h>
#include "Math.h"
#include <unordered_map>
#include "RendererDebugObject.h"

// 描画メソッド
enum class RENDER_METHOD
{
	FORWARD = 0,
	DEFERRED,
	PBR,
	BLOOM
};


class Renderer
{
public:

	Renderer();
	~Renderer();

	bool Initialize(int _width, int _height, bool _fullScreen);
	bool Load();
	void CreateRendererDebugObject();
	void Delete();

	void Draw();


	SDL_Window* GetMainWindow() { return m_window; }    // ウィンドウのゲッター
	SDL_GLContext GetContext() { return m_context; }

	class DrawableObjectManager* GetDrawableObjectManager() { return m_drawableObject; }

	void SetRenderMethod(RENDER_METHOD _method) { m_renderMethodType = _method; }

	void BloomPass(unsigned int _highBrightBuffer, unsigned int _colorBuffer);

	const Matrix4& GetViewMatrix() const { return m_viewMat; }
	const Matrix4& GetProjectionMatrix() const { return m_projMat; }

	// Getter / Setter
	class RenderBloom* GetBloom() { return m_bloomRender; }
	class ShaderManager* GetShaderManager() { return m_shaderManager; }
	class DirectionalLight* GetDirectionalLight() { return m_dirLight; }
	class CubeMap* GetSkyBox() { return m_skyBox; }
	class VertexArray* GetQuadVertex() { return m_quadVA; }
	bool GetIsEnableVisualizeNormal() { return m_visualizeNormal; }
	bool GetIsEnableBloom() { return m_enableBloom; }

	void SetViewMatrix(const Matrix4& _viewMat) { m_viewMat = _viewMat; }

private:

	void CreateMSAA();

	void SetUniformBuffer();

	static void FrameBuffer_Size_Callback(SDL_Window* _window, int _width, int _height);


	SDL_Window* m_window;                            // メインウィンドウ
	SDL_GLContext m_context;                         // コンテキスト
	SDL_Renderer* m_sdlRenderer;                     // SDLレンダラー


	RENDER_METHOD m_renderMethodType;                // 描画方法

	class ShaderManager* m_shaderManager;            // シェーダーマネージャークラス
	class DrawableObjectManager* m_drawableObject;   // 描画可能オブジェクト管理クラス

	class RenderBloom* m_bloomRender;                // ブルーム効果を付与するクラス
	class DirectionalLight* m_dirLight;              // アクティブなディレクショナルライト
	class CubeMap* m_skyBox;                         // skybox

	bool m_enableBloom;                              // ブルーム処理するかしないか
	bool m_visualizeNormal;                          // 法線を視覚化するかしないか

	// 各種描画メソッドクラス
	std::unordered_map<RENDER_METHOD, class RenderMethodBase*> m_renderMethods;


	// 描画用の行列関連
	Matrix4 m_viewMat;                             // ビュー行列
	Matrix4 m_projMat;                             // プロジェクション行列

	// uniformバッファ
	unsigned int m_uboMatrices;                      // ビュー・プロジェクション行列用UBO
	unsigned int m_uboCamera;                        // カメラ情報
	unsigned int m_uboTriggers;                      // トリガー類(ブルーム効果のオンオフ)
	unsigned int m_uboDirLights;                     // ディレクショナルライトUBO

	// vertexArrayクラス
	class VertexArray* m_quadVA;                     // 四角形 (スクリーンに使用)

	// MSAAバッファ
	unsigned int m_msaaBuffer;
	unsigned int m_msaaColor;
	unsigned int m_msaaRBO;


	// デバッグ用クラス
	class RendererDebugObject* m_debugObj;
	friend class RendererDebugObject;
};
