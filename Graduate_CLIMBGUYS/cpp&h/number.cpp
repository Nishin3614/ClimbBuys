// ----------------------------------------
//
// 番号処理の説明[number.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "number.h"
#include "keyconfig.h"

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
CNumber::CNumber() : CScene_TWO::CScene_TWO()
{
	m_nNum = 0;
	m_nRandamTime = 0;
	m_nFram = 0;
	m_bRand = false;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CNumber::~CNumber()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CNumber::Init(void)
{	
	CScene_TWO::Init();
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CNumber::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CNumber::Update(void)
{
	// ランダムタイムが0超過だったら
	// ->アニメーション開始
	if (m_bRand == true)
	{
		// フレームがランダムタイムを同じになったら
		// ->保存した番号にする
		if (m_nFram == m_nRandamTime)
		{
			SetNum(m_nNum);
			m_bRand = false;
			return;	
		}
		// 決定ボタンを押すと
		// ->保存した番号にする
		if (CManager::GetKeyConfig()->GetKeyConfigTrigger(CKeyConfig::CONFIG_DECISION))
		{
			SetNum(m_nNum);
			m_bRand = false;
			return;
		}
		// 数字をランダムで設定
		SetNum(rand() % 10);
		// フレームタイムアップ
		m_nFram++;
	}
	// 更新
	CScene_TWO::Update();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CNumber::Draw(void)
{
	// 初期化
	CScene_TWO::Draw();
}

// ----------------------------------------
// 番号設定処理
// ----------------------------------------
void CNumber::SetNum(int const &nNum)
{
	// 変数宣言
	float fTex;			// テクスチャー範囲
	// テクスチャー範囲の計算
	fTex = (float)nNum / 10.0f;
	// テクスチャー設定
	CScene_TWO::Set_Vtx_Tex(
		D3DXVECTOR2(fTex, 0.0f),
		D3DXVECTOR2(fTex + 0.1f, 1.0f)
	);
}

// ----------------------------------------
// 強制アニメ終了
// ----------------------------------------
void CNumber::Complusion(void)
{
	SetNum(m_nNum);
	m_bRand = false;
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CNumber::Load(void)
{
	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CNumber::UnLoad(void)
{
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CNumber * CNumber::Create(
	int						const & nScore,
	D3DXVECTOR3				const & pos,
	CTexture_manager::TYPE	const & tex,
	D3DXVECTOR2				const & size,
	D3DXCOLOR				const & col
)
{
	// 変数宣言
	CNumber * pNumber;
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pNumber = new CNumber();
	// 位置設定
	pNumber->SetPosition(pos);
	//サイズ設定
	pNumber->SetSize(size);
	// テクスチャータイプ設定
	pNumber->BindTexture(tex);
	// マネージャー管理
	pNumber->ManageSetting(CScene::LAYER_UI);
	// 初期化処理
	pNumber->Init();
	// スコア設定
	pNumber->SetNum(nScore);
	// 数字保存
	pNumber->m_nNum = nScore;
	// 生成したオブジェクトを返す
	return pNumber;
}