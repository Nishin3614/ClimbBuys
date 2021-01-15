// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 演出ブロック処理 [stagingobj.cpp]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "stagingobj.h"
#include "debugproc.h"

#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"

#include "basemode.h"
#include "opening.h"
#include "openingmanager.h"

#include "3dparticle.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingObj::CStagingObj() : CScene_X::CScene_X()
{
	m_bStopMove = false;
	m_nAngle = 0;
	m_Vector = D3DVECTOR3_ZERO;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingObj::~CStagingObj()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Init()
{
	// 速さの初期値
	m_fSpeed = 10.0f;
	m_fBurstSpeed = 10.0f;
	// 吹っ飛びベクトルをランダムに設定
	SetBurstVector(CCalculation::RandomDirectionVector(NULL, true));
	// シーンxの初期化
	CScene_X::Init();
	m_Vector.y = -1.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Update(void)
{
	// 移動量の更新
	MoveUpdate();

	// 座標の更新
	GetPos() += m_move;

	CScene_X::Update();

	// 画面範囲外に行ったらメモリ解放
	if (GetPos().y <= -200.0f || GetPos().y >= 1000.0f || GetPos().x <= -800.0f || GetPos().x >= 800.0f)
	{
		Release();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 通常処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Normal()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 空中浮遊処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Levitating()
{
	float fSpeed = (float)CCalculation::GetRandomRange(10, 30);

	m_move.y = sinf(m_nAngle++ * D3DX_PI / 50) * (fSpeed *= 0.1f);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 上昇処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Ascent()
{
	m_move.y = 2.0f;
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 下降処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Descent()
{
	m_move.y = -2.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 種類ごとの移動量の更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::MoveUpdate()
{
	switch (m_ObjType)
	{
	case CStagingObj::STAGING_OBJTYPE::NORMAL:
		break;
	case CStagingObj::STAGING_OBJTYPE::LEVITATION:
		Levitating();
		break;
	case CStagingObj::STAGING_OBJTYPE::ASCENT:
		Ascent();
		break;
	case CStagingObj::STAGING_OBJTYPE::DESCENT:
		break;
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ベースブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CStagingObj::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// メモリの生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingObj * CStagingObj::Create(
	D3DXVECTOR3 const &pos,
	D3DXVECTOR3 const &rot,
	D3DXVECTOR3 const &size,
	D3DXCOLOR color,
	int const &nModelId,
	bool const &bShadowMap,
	CStagingObj::STAGING_OBJTYPE type)
{
	// 変数宣言
	CStagingObj * pStagingBlock = new CStagingObj;

	// シーン管理設定
	pStagingBlock->ManageSetting(CScene::LAYER_3DOBJECT);

	pStagingBlock->SetPos(pos);
	pStagingBlock->SetRot(rot);
	pStagingBlock->SetSize(size);
	//pStagingBlock->SetModelColor(color);
	pStagingBlock->SetModelId(nModelId);
	pStagingBlock->SetShadowMap(bShadowMap);
	pStagingBlock->m_ObjType = type;

	// 初期化処理
	pStagingBlock->Init();

	// 生成したオブジェクトを返す
	return pStagingBlock;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 純粋仮想関数 必要ない関数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj::Scene_MyCollision(int const & nObjType, CScene * pScene){}
void CStagingObj::Scene_OpponentCollision(int const & nObjType, CScene * pScene){}
void CStagingObj::Scene_NoMyCollision(int const & nObjType, CScene * pScene){}
void CStagingObj::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene){}
D3DXVECTOR3 * CStagingObj::Scene_GetPPos(void){return nullptr;}
D3DXVECTOR3 * CStagingObj::Scene_GetPPosold(void){return nullptr;}
D3DXVECTOR3 * CStagingObj::Scene_GetPMove(void){return nullptr;}
