// ------------------------------------------
//
// レンダリング処理の説明[renderer.h]
// Author : Koki Nishiyama
//
// ------------------------------------------

#ifndef _RENDERER_H_
#define _RENDERER_H_	 // ファイル名を基準を決める

// ------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------
#if ERROW_ACTION

#define IMGUI_DEBUG	(1)	// ImGuiのデバッグ処理

#endif // ERROW_ACTION

// ------------------------------------------
//
// 前方宣言
//
// ------------------------------------------
class CDebugproc;
class CCamera;
class CLight;

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CRenderer
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_NORMAL = 0,
		// カリングノーマル
		TYPE_CULLNORMAL,
		// カリング(裏)
		TYPE_CULLBACK,
		// カリングなし
		TYPE_CULLNULL,
		// 通常合成
		TYPE_NORMALMIX,
		// 加算合成
		TYPE_ADDMIX,
		// 減算合成
		TYPE_DECMIX,
		// ライティングオン
		TYPE_RIGHTINGON,
		// ライティングオフ
		TYPE_RIGHTINGOFF,
		// Zバッファオン
		TYPE_ZBUFFON,
		// Zバッファオフ
		TYPE_ZBUFFOFF,
		// Zテスト通常
		TYPE_ZTEST_DEFAULT,
		// Zテスト無効
		TYPE_ZTEST_OFF,
		// 3Dエフェクト用オン
		TYPE_3DEFFECT_ON,
		// 3Dエフェクト用オフ
		TYPE_3DEFFECT_OFF,
		// フォグON
		TYPE_FOG_ON,
		// フォグOFF
		TYPE_FOG_OFF,
		TYPE_MAX
	} TYPE;

	// 合成演算
	typedef enum
	{
		BLEND_NORMAL = 0,		// 通常
		BLEND_TRANSLUCENT,		// 半透明
		BLEND_ADD,				// 加算合成
		BLEND_ADD_TRANSLUCENT,	// 加算半透明
		BLEND_SUBTRACTION,		// 減算
		BLEND_MULTIPUL1,		// 乗算1
		BLEND_MULTIPUL2,		// 乗算2
		BLEND_SCREEN,			// スクリーン
		BLEND_REVERSE,			// リバース
		BLEND_MAX				// 最大数
	} BLEND;

	/* 関数 */
	CRenderer();
	~CRenderer();
	HRESULT  Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// ロード中の描画処理
	void LoadDraw(void);
#if IMGUI_DEBUG

	void CleanupDeviceD3D();
	void ResetDevice();

#endif // IMGUI_DEBUG

	// 描画状態
	void SetType(TYPE type = TYPE_NORMAL);
	// 合成処理
	//	blend	: 合成演算
	void SetBlend(
		BLEND const &blend = BLEND_TRANSLUCENT	// 合成演算
	);
	// 情報取得
	LPDIRECT3DDEVICE9 GetDevice(void);	// デバイス
	CCamera * GetCamera(void);			// カメラ
	CLight * GetLight(void) { return m_pLight; }			// ライト
protected:

private:
	/* 変数 */
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトのポインタ
	LPDIRECT3DDEVICE9 m_pDevice;		// Direct3Dデバイスのポインタ

	CCamera * m_pCamera;				// カメラ情報
	CLight * m_pLight;					// ライト情報
	static bool m_bDebug;				// デバッグモード
	D3DPRESENT_PARAMETERS    m_d3dpp;
#if IMGUI_DEBUG

	// ImGuiのセットアップ
	void ImGui_SetUp(HWND hWnd);
	// ImGuiの更新
	void UpdateImGui(void);
	ImVec4 m_clear_color;

#endif // IMGUI_DEBUG
#ifdef _DEBUG

	void DrawFPS(void);					// FPS関数
#endif // _DEBUG

	// 変数宣言 //
	static CDebugproc * m_debugproc;
};

#endif // !_RENDERER_H_