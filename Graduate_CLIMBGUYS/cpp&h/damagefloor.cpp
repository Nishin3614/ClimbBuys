// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ダメージ床[damagefloor.cpp]
// Author : FujiwaraMasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "damagefloor.h"
#include "player.h"
#include "basemode.h"
#include "debugproc.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// イニシャライザコンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CDamageFloor::CDamageFloor() : CScene_THREE::CScene_THREE()
{
	m_MoveSpeed = 0.0f;									// 移動量の初期化
	m_AscendUpToTime = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CDamageFloor::~CDamageFloor()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::Init(void)
{
	// オフセットタイプ設定
	CScene_THREE::SetOffsetType(OFFSET_TYPE_SIDE_CENTER);

	// 透明にさせる
	CScene_THREE::SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));

	m_MoveSpeed = 0.5f;
	m_AscendUpToTime = 3600;

	// デバッグ時のみ赤く表示
	CScene_THREE::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// シーン3Dの初期化
	CScene_THREE::Init();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::Uninit(void)
{
	CScene_THREE::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::Update(void)
{
	// ゲームモードだったら
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// タイマーの値を減らしていき0になったら床を上昇させる
		if (m_AscendUpToTime-- <= 0)
		{
			m_AscendUpToTime = 0;
			// ダメージ床の上昇
			Move();
		}
		CDebugproc::Print("タイマー %d\n", m_AscendUpToTime);
	}

	// 高さを比較
	ComparisonHeight();

	CScene_THREE::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::Draw(void)
{
	// 描画
	CScene_THREE::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
CDamageFloor * CDamageFloor::Create()
{
	// 変数宣言
	CDamageFloor * pDamageFloor;		// シーン3Dクラス
										// メモリの生成(初め->基本クラス,後->派生クラス)
	pDamageFloor = new CDamageFloor();
	// シーン管理設定
	pDamageFloor->ManageSetting(CScene::LAYER_FLOOR);
	// 位置設定
	pDamageFloor->SetPos(D3DXVECTOR3(0.0, -500.0f, 0.0f));
	// サイズ設定
	pDamageFloor->SetSize(FIELD_SIZE);
	// 初期化処理
	pDamageFloor->Init();
	// テクスチャの貼り付け
	pDamageFloor->SetTexType(CTexture_manager::TYPE_MAGMA);

	// 生成したオブジェクトを返す
	return pDamageFloor;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CDamageFloor::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// unload処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 高さを比較
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::ComparisonHeight()
{
	//通常アイテムの総数分
	for (int nCnt = 0; nCnt < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCnt++)
	{
		// プレイヤーのポインタ取得
		CPlayer *pPlayer = (CPlayer*)CScene::GetScene(LAYER_CHARACTER, nCnt);
		if (pPlayer != nullptr)
		{
			//高さを比較してプレイヤーが床より下に落ちたら死亡フラグをtrue
			if (this->GetPos().y >= pPlayer->GetPos().y)
			{
				// 記録更新_死亡原因
				pPlayer->GetRecord().DieCause = CPlayer::DIECAUSE::DIECAUSE_FALL;
				// 死亡設定
				pPlayer->SetDie(true);
			}
			pPlayer = nullptr;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// 床の移動
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDamageFloor::Move()
{
	this->GetPos().y += m_MoveSpeed;
}
