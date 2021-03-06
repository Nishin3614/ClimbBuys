// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 演出ブロック処理 [stagingobj_info.cpp]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "stagingobj_info.h"
//#include "stagingobj.h"

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
bool CStagingObj_Info::m_bEnd = false;
int CStagingObj_Info::m_nFrame = 0;
CStagingObj_Info::Block_Condition CStagingObj_Info::m_Condition = {};
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingObj_Info::CStagingObj_Info() : CScene::CScene()
{
	m_bStopMove = false;
	m_nAngle = 0;
	m_Vector = D3DVECTOR3_ZERO;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingObj_Info::~CStagingObj_Info()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Init()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Update(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Draw(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ベースブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CStagingObj_Info::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::UnLoad(void)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 純粋仮想関数 必要ない関数
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Scene_MyCollision(int const & nObjType, CScene * pScene) {}
void CStagingObj_Info::Scene_OpponentCollision(int const & nObjType, CScene * pScene) {}
void CStagingObj_Info::Scene_NoMyCollision(int const & nObjType, CScene * pScene) {}
void CStagingObj_Info::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene) {}
D3DXVECTOR3 * CStagingObj_Info::Scene_GetPPos(void) { return nullptr; }
D3DXVECTOR3 * CStagingObj_Info::Scene_GetPPosold(void) { return nullptr; }
D3DXVECTOR3 * CStagingObj_Info::Scene_GetPMove(void) { return nullptr; }


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 条件の設定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::SetCondition(const int PerFrame, const int FrameBetween, const int ToOnes, CStagingObj::STAGING_OBJTYPE type)
{
	m_Condition.nPerFrame[static_cast<int>(type)] = PerFrame;
	m_Condition.nFrameBetween[static_cast<int>(type)] = FrameBetween;
	m_Condition.nToOnes[static_cast<int>(type)] = ToOnes;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// メモリの生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStagingObj_Info * CStagingObj_Info::Create_StagingObj_Info()
{
	// 変数宣言
	CStagingObj_Info * pStagingBlock = new CStagingObj_Info;

	// シーン管理設定
	pStagingBlock->ManageSetting(CScene::LAYER_3DOBJECT);

	// 初期化処理
	pStagingBlock->Init();

	// 生成したオブジェクトを返す
	return pStagingBlock;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 一定時間毎に演出ブロックを生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Create_StagingObj(D3DXVECTOR3 Originpos, D3DXVECTOR3 Range, int const & nModelId, CStagingObj::STAGING_OBJTYPE type, bool loop)
{
	m_nFrame++;

	if (m_nFrame % m_Condition.nPerFrame[static_cast<int>(type)] == 0 && m_Condition.nFrameBetween[static_cast<int>(type)] > m_nFrame)
	{
		D3DXVECTOR3 CreatePos;
		D3DXVECTOR3 RandomRot;

		RandomRot.x = CCalculation::Random_PI();
		RandomRot.y = CCalculation::Random_PI();
		RandomRot.z = CCalculation::Random_PI();

		// ランダムな座標を求める
		if (Range != NULL)
		{
			CreatePos.x = CCalculation::Random(Originpos.x + Range.x);
			CreatePos.y = Originpos.y;
			CreatePos.z = Originpos.z;
		}
		else
		{
			CreatePos = Originpos;
		}

		// オブジェクトの生成
		CStagingObj::Create(
			CreatePos,
			RandomRot,
			D3DXVECTOR3(3.0f, 3.0f, 3.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			nModelId,
			false,
			type
		);
	}
	else
	{
		return;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 一定時間毎に演出ブロックを生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Create_StagingObj_RangeSelect(D3DXVECTOR3 Originpos, D3DXVECTOR3 Range, int const & nModelIdfirst, int const & nModelIdend, CStagingObj::STAGING_OBJTYPE type, bool loop)
{
	m_nFrame++;

	if (loop)
	{
		if (m_nFrame % m_Condition.nPerFrame[static_cast<int>(type)] == 0)
		{
			D3DXVECTOR3 CreatePos;
			D3DXVECTOR3 RandomRot;

			RandomRot.x = CCalculation::Random_PI();
			RandomRot.y = CCalculation::Random_PI();
			RandomRot.z = CCalculation::Random_PI();

			// ランダムな座標を求める
			if (Range != NULL)
			{
				CreatePos.x = CCalculation::Random(Originpos.x + Range.x);
				CreatePos.y = Originpos.y;
				CreatePos.z = Originpos.z;
			}
			else
			{
				CreatePos = Originpos;
			}

			// プレイヤーモデルをランダムに選択
			int nRandomModel = (int)CCalculation::GetRandomRange(nModelIdfirst, nModelIdend);

			// オブジェクトの生成
			CStagingObj::Create(
				CreatePos,
				RandomRot,
				D3DXVECTOR3(3.0f, 3.0f, 3.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				nRandomModel,
				false,
				type
			);
		}
		else
		{
			return;
		}
	}
	else
	{
		if (m_nFrame % m_Condition.nPerFrame[static_cast<int>(type)] == 0 && m_Condition.nFrameBetween[static_cast<int>(type)] > m_nFrame)
		{
			D3DXVECTOR3 CreatePos;
			D3DXVECTOR3 RandomRot;

			RandomRot.x = CCalculation::Random_PI();
			RandomRot.y = CCalculation::Random_PI();
			RandomRot.z = CCalculation::Random_PI();

			// ランダムな座標を求める
			if (Range != NULL)
			{
				CreatePos.x = CCalculation::Random(Originpos.x + Range.x);
				CreatePos.y = Originpos.y;
				CreatePos.z = Originpos.z;
			}
			else
			{
				CreatePos = Originpos;
			}

			// プレイヤーモデルをランダムに選択
			int nRandomModel = (int)CCalculation::GetRandomRange(nModelIdfirst, nModelIdend);

			// オブジェクトの生成
			CStagingObj::Create(
				CreatePos,
				RandomRot,
				D3DXVECTOR3(3.0f, 3.0f, 3.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				nRandomModel,
				false,
				type
			);
		}
		else
		{
			return;
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 固定値にブロックを生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::Create_TitleObj()
{
	CScene_X::TYPE type[MAX_TITLEBLOCK_NUM];
	type[0] = CScene_X::TYPE_OBJECT_BLOCK_C;
	type[1] = CScene_X::TYPE_OBJECT_BLOCK_L;
	type[2] = CScene_X::TYPE_OBJECT_BLOCK_I;
	type[3] = CScene_X::TYPE_OBJECT_BLOCK_M;
	type[4] = CScene_X::TYPE_OBJECT_BLOCK_B;
	type[5] = CScene_X::TYPE_OBJECT_BLOCK_G;
	type[6] = CScene_X::TYPE_OBJECT_BLOCK_U;
	type[7] = CScene_X::TYPE_OBJECT_BLOCK_Y;
	type[8] = CScene_X::TYPE_OBJECT_BLOCK_S;

	D3DXVECTOR3 pos[MAX_TITLEBLOCK_NUM];
	pos[0] = STAGINGBLOCK_POS_C;
	pos[1] = STAGINGBLOCK_POS_L;
	pos[2] = STAGINGBLOCK_POS_I;
	pos[3] = STAGINGBLOCK_POS_M;
	pos[4] = STAGINGBLOCK_POS_B;
	pos[5] = STAGINGBLOCK_POS_G;
	pos[6] = STAGINGBLOCK_POS_U;
	pos[7] = STAGINGBLOCK_POS_Y;
	pos[8] = STAGINGBLOCK_POS_S;


	for (int nCnt = 0; nCnt < MAX_TITLEBLOCK_NUM; nCnt++)
	{
		// オブジェクトの生成
		CStagingObj::Create(
			pos[nCnt],
			D3DVECTOR3_ZERO,
			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			type[nCnt],
			false,
			CStagingObj::STAGING_OBJTYPE::LEVITATION
		);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 条件の初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStagingObj_Info::InitCondition()
{
	for (int nCnt = 0; nCnt < static_cast<int>(CStagingObj::STAGING_OBJTYPE::MAX); nCnt++)
	{
		m_Condition.nPerFrame[nCnt] = NULL;
		m_Condition.nFrameBetween[nCnt] = NULL;
		m_Condition.nToOnes[nCnt] = NULL;
	}
}
