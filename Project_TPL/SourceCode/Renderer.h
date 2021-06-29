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
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <vector>
#include "RendererDebugObject.h"

// 描画メソッド
enum class RENDER_METHOD
{
	FORWARD = 0,
	DEFFERED
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


	GLFWwindow* GetMainWindow() { return m_window; }    // ウィンドウコンテキストのゲッター

	class DrawableObjectManager* GetDrawableObjectManager() { return m_drawableObject; }

	void SetRenderMethod(RENDER_METHOD _method) { m_renderMethod = _method; }

	const glm::mat4 GetViewMatrix() { return m_viewMat; }
	const glm::mat4 GetProjectionMatrix() { return m_projMat; }

	class DirectionalLight* GetDirectionalLight() { return m_dirLight; }

	void SetViewMatrix(const glm::mat4& _viewMat) { m_viewMat = _viewMat; }

private:

	void CreateGBuffer();
	void CreateLightBuffer();
	void CreateMSAA();

	void SetUniformBuffer();

	static void FrameBuffer_Size_Callback(GLFWwindow* _window, int _width, int _height);


	GLFWwindow* m_window;                            // メインウィンドウ

	RENDER_METHOD m_renderMethod;                    // 描画方法

	class ShaderManager* m_shaderManager;            // シェーダーマネージャークラス
	class DrawableObjectManager* m_drawableObject;   // 描画可能オブジェクト管理クラス

	class RenderBloom* m_bloomRender;                // ブルーム効果を付与するクラス
	class DirectionalLight* m_dirLight;              // アクティブなディレクショナルライト
	class CubeMap* m_skyBox;                         // skybox

	bool m_enableBloom;                              // ブルーム処理するかしないか
	bool m_visualizeNormal;                          // 法線を視覚化するかしないか

	// 描画用の行列関連
	glm::mat4 m_viewMat;                             // ビュー行列
	glm::mat4 m_projMat;                             // プロジェクション行列

	// uniformバッファ
	unsigned int m_uboMatrices;                      // ビュー・プロジェクション行列用UBO
	unsigned int m_uboCamera;                        // カメラ情報
	unsigned int m_uboTriggers;                      // トリガー類(ブルーム効果のオンオフ)
	unsigned int m_uboDirLights;                     // ディレクショナルライトUBO

	// vertexArrayクラス
	class VertexArray* m_quadVA;                     // 四角形 (スクリーンに使用)

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

	// MSAAバッファ
	unsigned int m_msaaBuffer;
	unsigned int m_msaaColor;
	unsigned int m_msaaRBO;


	// デバッグ用クラス
	class RendererDebugObject* m_debugObj;
	friend class RendererDebugObject;
};
