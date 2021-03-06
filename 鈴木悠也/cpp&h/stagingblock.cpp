// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 演出ブロック処理 [stagingblock.cpp]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "stagingblock.h"
#include "debugproc.h"

#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"

#include "basemode.h"
#include "opening.h"
#include "openingmanager.h"

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
CStagingBlock::CStagingBlock() : CScene_X::CScene_X()
{
	m_bStopMove = false;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingBlock::~CStagingBlock()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::Init()
{
	// 速さの初期値
	m_fSpeed = 100.0f;
	m_fBurstSpeed = 10.0f;
	// 吹っ飛びベクトルをランダムに設定
	SetBurstVector(CCalculation::RandomDirectionVector(NULL, true));
	// シーンxの初期化
	CScene_X::Init();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::Update(void)
{
	CScene_X::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ベースブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CStagingBlock::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// メモリの生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingBlock * CStagingBlock::Create(D3DXVECTOR3 const &pos,D3DXVECTOR3 const &rot,D3DXVECTOR3 const &size,D3DXCOLOR color,int const &nModelId,bool const &bShadowMap)
{
	// 変数宣言
	CStagingBlock * pStagingBlock = new CStagingBlock;

	// シーン管理設定
	pStagingBlock->ManageSetting(CScene::LAYER_3DOBJECT);

	pStagingBlock->SetPos(pos);
	pStagingBlock->SetRot(rot);
	pStagingBlock->SetSize(size);
	pStagingBlock->SetModelColor(color);
	pStagingBlock->SetModelId(nModelId);
	pStagingBlock->SetShadowMap(bShadowMap);

	// 初期化処理
	pStagingBlock->Init();

	// 生成したオブジェクトを返す
	return pStagingBlock;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 純粋仮想関数 必要ない関数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::Scene_MyCollision(int const & nObjType, CScene * pScene){}
void CStagingBlock::Scene_OpponentCollision(int const & nObjType, CScene * pScene){}
void CStagingBlock::Scene_NoMyCollision(int const & nObjType, CScene * pScene){}
void CStagingBlock::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene){}
D3DXVECTOR3 * CStagingBlock::Scene_GetPPos(void){return nullptr;}
D3DXVECTOR3 * CStagingBlock::Scene_GetPPosold(void){return nullptr;}
D3DXVECTOR3 * CStagingBlock::Scene_GetPMove(void){return nullptr;}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの移動
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::BlockFall()
{
	if (!m_bStopMove)
	{
		D3DXVECTOR3 vector = COpeningManager::GetTargetpos(this->m_nBlockNun) - this->GetPos();
		D3DXVec3Normalize(&vector, &vector);

		D3DXVECTOR3 temppos = this->GetPos() + vector * m_fSpeed;

		if (temppos.y <= COpeningManager::GetTargetpos(this->m_nBlockNun).y)
		{
			this->SetPos(COpeningManager::GetTargetpos(this->m_nBlockNun));
		}
		else
		{
			this->SetPos(this->GetPos() + vector * m_fSpeed);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの破裂
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingBlock::BlockBurst()
{
	if (this->m_nBlockNun == 4)
	{
		m_Vector = CManager::GetRenderer()->GetCamera()->GetPosV() - D3DXVECTOR3(this->GetPos().x, this->GetPos().y + 80, this->GetPos().z);

		if (CCalculation::Range_Absolute(CManager::GetRenderer()->GetCamera()->GetPosV().z, this->GetPos().z) <= 50)
		{
			// タイトルへ遷移
			// フェード状態が何も起こっていない状態なら
			if (CManager::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
		}
		else
		{
			D3DXVec3Normalize(&m_Vector, &m_Vector);
			this->SetPos(this->GetPos() + m_Vector * m_fBurstSpeed);
		}
	}
	else
	{
		this->SetPos(this->GetPos() + m_Vector * m_fBurstSpeed);
	}
}
