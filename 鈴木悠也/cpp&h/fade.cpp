// ----------------------------------------
//
// 画面遷移処理の説明[fade.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "fade.h"
#include "manager.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CFade::CFade() : C2DPresents::C2DPresents()
{
	m_fade = FADE_NONE;					// フェード状態
	m_modeNext = CManager::MODE_GAME;	// 次の画面（モード）
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CFade::~CFade()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CFade::Init(void)
{	
	// 変数宣言
	m_fade = FADE_NONE;
	// 位置設定
	C2DPresents::SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	// サイズ設定
	C2DPresents::SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	// サイズ設定
	C2DPresents::SetCol(D3DXCOLOR_CA(0.0f, 0.0f));
	// プレゼン初期化
	C2DPresents::Init();
	// テクスチャー
	C2DPresents::BindTexture(0);
	// フェードイン
	C2DPresents::SetFadeIn(
		D3DXCOLOR_CA(0.0f, 0.0f),
		D3DXCOLOR_CA(0.0f, 1.0f),
		30
	);
	// フェードアウト
	C2DPresents::SetFadeOut(
		D3DXCOLOR_CA(0.0f, 1.0f),
		D3DXCOLOR_CA(0.0f, 0.0f),
		30
	);
	// モード変更
	CManager::SetMode(m_modeNext);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CFade::Uninit(void)
{
	// プレゼン終了
	C2DPresents::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_OUT)
		{
			// フェードインが不使用だったら
			if (C2DPresents::GetFadeOut_Bool() == false)
			{
				// フェード終了
				m_fade = FADE_NONE;
				// フェードアウト強制終了
				C2DPresents::Compulsion_FadeOut();
			}
		}
		else if (m_fade == FADE_IN)
		{
			// フェードアウトが不使用だったら
			if (C2DPresents::GetFadeIn_Bool() == false)
			{
				// フェード終了
				m_fade = FADE_OUT;
				// モード変更
				CManager::SetMode(m_modeNext);
				// フェードアウト開始
				C2DPresents::Start_FadeOut();
			}
		}
	}
	// プレゼン更新
	C2DPresents::Update();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CFade::Draw(void)
{
	// プレゼン描画
	C2DPresents::Draw();
}

// ----------------------------------------
// 画面遷移設定処理
// ----------------------------------------
void CFade::SetFade(CManager::MODE const & Mode)
{
	// フェードイン
	m_fade = FADE_IN;
	// モード代入
	m_modeNext = Mode;
	// フェードイン開始
	C2DPresents::Start_FadeIn();
}

// ----------------------------------------
// 画面遷移取得処理
// ----------------------------------------
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CFade * CFade::Create(CManager::MODE const & mode)
{
	// 変数宣言
	CFade * pFade;
	// メモリ確保
	pFade = new CFade;
	// 設定
	pFade->m_modeNext = mode;
	// 初期化
	pFade->Init();
	// 返す
	return pFade;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CFade::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CFade::UnLoad(void)
{
}