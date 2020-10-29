// ------------------------------------------
//
// レンダリング処理の説明[renderer.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------
#include "renderer.h"
#include "scene.h"
#include "scene_load.h"
#include "fade.h"
#include "game.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "keyboard.h"
#include "mouse.h"
#include "connectblock.h"

// テスト
#include "3Dmap.h"

// ------------------------------------------
//
// マクロ関数
//
// ------------------------------------------

// ------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------

// ------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------
#ifdef _DEBUG
CDebugproc * CRenderer::m_debugproc = NULL;
#endif
bool CRenderer::m_bDebug = false;				// デバッグモード

D3DXVECTOR3 g_f = D3DVECTOR3_ZERO;
// ------------------------------------------
// コンストラクタ
// ------------------------------------------
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pDevice = NULL;
}

// ------------------------------------------
// デストラクタ
// ------------------------------------------
CRenderer::~CRenderer()
{
}

// ------------------------------------------
// 初期化処理
// ------------------------------------------
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// 変数宣言
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));								// ワークをゼロクリア
	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;								// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;										// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;								// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;						// デプスバッファとして16bitを使う
	m_d3dpp.Windowed = bWindow;											// ウィンドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート(現在の速度に合わせる)
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// インターバル(VSyncを待って描画)
	// m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// クライアント領域を直ちに更新する

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	// ディスプレイアダプタ
		D3DDEVTYPE_HAL,									// デバイスタイプ
		hWnd,											// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,			// デバイス作成制御の組み合わせ
		&m_d3dpp,											// デバイスのプレゼンテーションパラメータ
		&m_pDevice)))									// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダラーステートの設定
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャステージステートパラメータの設定
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// ImGuiのセットアップ
	ImGui_SetUp(hWnd);
	// デバッグプロシージャー
	m_debugproc = new CDebugproc;
	// デバッグ表示初期化
	m_debugproc->Init(m_pDevice);
#endif
	// シーンの読み込み
	CScene_load::LoadAll();
	// カメラの生成
	m_pCamera = CCamera::Create();
	// ライトの生成
	m_pLight = CLight::Create();
	return S_OK;
}

// ------------------------------------------
// 終了処理
// ------------------------------------------
void CRenderer::Uninit(void)
{
	// シーン親子のリリース処理
	CScene::ReleaseAll();
	// 読み込んだものを破棄する処理
	CScene_load::UnLoadAll();
	// カメラの生成
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	// ライトの生成
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}
	// デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// D3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
#ifdef _DEBUG
	// ImGuiの終了
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デバッグ表示
	if (m_debugproc != NULL)
	{
		m_debugproc->Uninit();
		delete m_debugproc;
		m_debugproc = NULL;
	}

#endif // _DEBUG
}

// ------------------------------------------
// 更新処理
// ------------------------------------------
void CRenderer::Update(void)
{
#ifdef _DEBUG
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("SideWindow");
#endif // _DEBUG
	// カメラの更新
	m_pCamera->Update();
	// ライトの更新
	m_pLight->Update();

#ifdef _DEBUG
	CDebugproc::Print("デバッグモード:F2\n");
	CDebugproc::Print("1フレーム再生:L\n");
	// FPS表示
	DrawFPS();
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_F1))
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	// デバッグモード
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_F2))
	{
		m_bDebug = !m_bDebug;
	}
	// シーン
	if (m_bDebug == false)
	{
		CScene::UpdateAll();
	}
	else
	{
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{
			CScene::UpdateAll();
		}
	}
	// ImGuiの更新
	UpdateImGui();
#else // _RELEASE
	// シーン
	if (m_bDebug == false)
	{
		CScene::UpdateAll();
	}
	else
	{
		if (CManager::GetKeyboard()->GetKeyboardPress(DIK_L))
		{
			CScene::UpdateAll();
		}
	}
#endif // _DEBUG || _RELEASE
}

// -----------------------
// 描画処理
// --------------------------
void CRenderer::Draw(void)
{
	// バックバッファとZバッファのクリア
	m_pDevice->Clear(0, NULL, (D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// Direct3Dによる描画開始
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// カメラの設定
		m_pCamera->Set();
		// シーン
		CScene::DrawAll();
		// ベースモード描画
		CManager::GetBaseMode()->Draw();
		// カメラの描画
		m_pCamera->Draw();

		CManager::GetFade()->Draw();

#ifdef _DEBUG
		// デバッグ表示の描画
		CDebugproc::Draw();

		// ImGuiの描画
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif
		// Direct3Dによる描画終了
		m_pDevice->EndScene();
	}

	// バックバッファとフロートバッファの入れ替え
	HRESULT result = m_pDevice->Present(NULL, NULL, NULL, NULL);
#ifdef _DEBUG
	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && m_pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
#endif

}

// -----------------------
// 描画処理
// --------------------------
void CRenderer::LoadDraw(void)
{
	// バックバッファとZバッファのクリア
	m_pDevice->Clear(0, NULL, (D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(1, 100, 155, 255), 1.0f, 0);

	// Direct3Dによる描画開始
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// Direct3Dによる描画終了
		m_pDevice->EndScene();
	}

	// バックバッファとフロートバッファの入れ替え
	HRESULT result = m_pDevice->Present(NULL, NULL, NULL, NULL);
}

// ------------------------------------------
// 描画状態処理
// ------------------------------------------
void CRenderer::SetType(
	TYPE type
)
{
	switch (type)
	{
	case TYPE_NORMAL:
		// レンダラーステートの設定
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);	// アルファテスト有効
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);				// アルファ値の基準設定(基準より大きいと描画する)
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)
		break;
		// カリングを通常に戻す
	case TYPE_CULLNORMAL:
		// レンダラーステートの設定
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		break;
	case TYPE_CULLBACK:
		// レンダラーステートの設定
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		break;
	case TYPE_CULLNULL:
		// レンダラーステートの設定
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		break;
	case TYPE_NORMALMIX:
		// レンダーステート(通常ブレンド処理)
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case TYPE_ADDMIX:
		// レンダーステート(加算合成処理)
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case TYPE_DECMIX:
		break;
	case TYPE_RIGHTINGON:
		// ライティングモード有効
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		break;
	case TYPE_RIGHTINGOFF:
		// ライティングモード無効
		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		break;
	case TYPE_ZBUFFON:
		//Zバッファ　有効
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		break;
	case TYPE_ZBUFFOFF:
		//Zバッファ　有効
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		break;
	case TYPE_ZTEST_DEFAULT:
		//Zテスト 通常
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		break;
	case TYPE_ZTEST_OFF:
		//Zテスト 無効
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		break;
	case TYPE_3DEFFECT_ON:
		// ライティングモード無効
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		//Zバッファ　有効　無効
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		// レンダーステート(加算合成処理)
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case TYPE_3DEFFECT_OFF:
		//アルファテスト戻す
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		//Zバッファ　有効
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		// ライティングモード有効
		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		// レンダーステート(通常ブレンド処理)
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
		//フォグON
	case TYPE_FOG_ON:
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		break;
		//フォグOFF
	case TYPE_FOG_OFF:
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		break;
	default:
		break;
	}
}


// ------------------------------------------
// 合成処理
//	blend	: 合成演算
// ------------------------------------------
void CRenderer::SetBlend(
	BLEND const &blend	// 合成演算
)
{
	// 合成演算
	switch (blend)
	{
		// 通常合成
	case BLEND_NORMAL:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		break;
		// 半透明合成
	case BLEND_TRANSLUCENT:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
		// 加算合成
	case BLEND_ADD:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
		// 加算半透明合成
	case BLEND_ADD_TRANSLUCENT:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
		// 減算合成
	case BLEND_SUBTRACTION:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		break;
		// 乗算合成1
	case BLEND_MULTIPUL1:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		break;
		// 乗算合成2
	case BLEND_MULTIPUL2:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		break;
		// スクリーン合成
	case BLEND_SCREEN:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
		// リバース合成
	case BLEND_REVERSE:
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		break;
	default:
		break;
	}
}

// ------------------------------------------
// デバイス取得処理
// ------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pDevice;
}

// ------------------------------------------
// カメラ取得処理
// ------------------------------------------
CCamera * CRenderer::GetCamera(void)
{
	return m_pCamera;
}

#ifdef _DEBUG
// ------------------------------------------
// ImGuiのセットアップ処理
// ------------------------------------------
void CRenderer::ImGui_SetUp(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// ImGuiの基本設定
	io.MousePos;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos;     // Enable Keyboard Controls
	io.FontAllowUserScaling = true;
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("C:\\WINDOWS\\Fonts\\MEIRYO.TTC", 18.0f, &config, glyphRangesJapanese);

	// 背景色の設定
	ImGui::StyleColorsDark();
	// プラットフォームに合わせたセットアップ
	ImGui_ImplWin32_Init(hWnd);
	// レンダラーをバインド
	ImGui_ImplDX9_Init(m_pDevice);
	// カラー設定
	m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}
// ------------------------------------------
// 描画状態処理
// ------------------------------------------
void CRenderer::CleanupDeviceD3D()
{
	if (m_pDevice) { m_pDevice->Release(); m_pDevice = NULL; }
	if (m_pD3D) { m_pD3D->Release(); m_pD3D = NULL; }
}

// ------------------------------------------
// 描画状態処理
// ------------------------------------------
void CRenderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = m_pDevice->Reset(&m_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// ------------------------------------------
// ImGuiの更新処理
// ------------------------------------------
void CRenderer::UpdateImGui(void)
{
	// デバッグ情報
	CCalculation::ImG_DebugInfo();
	// カメラ情報の設定
	m_pCamera->Debug();
	// ライトの情報の設定
	m_pLight->Debug();
	// 結合ブロックの静的なデバッグ処理
	CConnectblock::StaticDebug();
	// ライト情報の設定
	CDebugproc::Print("マウス位置(%.1f,%.1f)\n", (float)CManager::GetMouse()->GetX(), (float)CManager::GetMouse()->GetY());
	CDebugproc::Print("ImGuiのマウス位置(%.1f,%.1f)\n",ImGui::GetMousePos().x,ImGui::GetMousePos().y);

	// テスト 3Dマップの情報設定
	C3DMap::Debug();


	// ImGuiの更新終了
	ImGui::End();
	ImGui::EndFrame();
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(m_clear_color.x*255.0f), (int)(m_clear_color.y*255.0f), (int)(m_clear_color.z*255.0f), (int)(m_clear_color.w*255.0f));
}

// ------------------------------------------
// FPS描画処理
// ------------------------------------------
void CRenderer::DrawFPS(void)
{
	int nCntFPS = GetFPS();

	CDebugproc::Print("FPS:%d\n",nCntFPS);
}
#endif // _DEBUG