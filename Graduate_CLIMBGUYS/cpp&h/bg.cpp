//------------------------------------------------------------------------------
//
// 背景の処理  [bg.cpp]
// Author:Yoshiki Sato
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------------
#include "bg.h"
#include "scene_two.h"
#include "manager.h"

//------------------------------------------------------------------------------
// マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------------
CBg::CBg()
{
	m_pScene2D = nullptr;
}

//------------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------------
CBg::~CBg()
{
	m_pScene2D = nullptr;
}

//------------------------------------------------------------------------------
// 初期化処理
//------------------------------------------------------------------------------
void CBg::Init()
{
	// 背景の生成
	m_pScene2D = CScene_TWO::Create(OFFSET_TYPE_CENTER, SCREEN_CENTER_POS, SCREEN_SIZE, 3, 0.0f, D3DXCOLOR_INI, LAYER::LAYER_BG);

	CScene_TWO::Init();
}

//------------------------------------------------------------------------------
// 終了処理
//------------------------------------------------------------------------------
void CBg::Uninit()
{
	CScene_TWO::Uninit();
}

//------------------------------------------------------------------------------
// 更新処理
//------------------------------------------------------------------------------
void CBg::Update()
{
	CScene_TWO::Update();
}

//------------------------------------------------------------------------------
// 描画処理
//------------------------------------------------------------------------------
void CBg::Draw()
{
	CScene_TWO::Draw();
}

//------------------------------------------------------------------------------
// 背景の生成処理
//------------------------------------------------------------------------------
CBg *CBg::Create()
{
	//メモリ確保
	CBg *pBg = new CBg;

	pBg->Init();

	//リターン
	return pBg;
}