// ----------------------------------------
//
// デバッグ表示処理の説明[debugproc.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "debugproc.h"

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
LPD3DXFONT CDebugproc::m_pFont = NULL;
char CDebugproc::m_aStr[STRING_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CDebugproc::CDebugproc()
{
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CDebugproc::~CDebugproc()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CDebugproc::Init(LPDIRECT3DDEVICE9 m_pDevice)
{
#ifdef _DEBUG


	// デバッグ表示用フォントを設定
	D3DXCreateFont(m_pDevice, 12, 6, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ ゴシック", &m_pFont);
#endif // _DEBUG
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CDebugproc::Uninit(void)
{
}

// ----------------------------------------
// デバッグ表示処理
// ----------------------------------------
void CDebugproc::Print(char * fmt, ...)
{
#ifdef _DEBUG
	// 変数宣言
	va_list va;	// 可変変数
	int nCntString;

	nCntString = strlen(m_aStr);
	// 可変引数を1個にまとめる
	va_start(va, fmt);
	// まとめて表示
	vsprintf(&m_aStr[nCntString], fmt, va);
	va_end(va);
#endif // _DEBUG
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CDebugproc::Draw(void)
{
#ifdef _DEBUG
	int nCntString;

	nCntString = strlen(m_aStr);
	// 変数宣言
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	// テキスト描画
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	// テキスト描画一回
	m_aStr[0] = '\0';
	if (nCntString >= STRING_MAX)
	{
		CCalculation::Messanger("CDebugproc::デバッグ表示が多すぎます!!\n");
	}
#endif // _DEBUG
}

