// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 足場処理 [stand.cpp]
// Author : FujiwaraMasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "stand.h"
#include "rectcollision.h"
#include "signboard.h"
#include "player.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define STAND_TARANSITION_SIZE	(200.0f)	// 遷移範囲のサイズ

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CStand::m_bDetermination = false;
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// オーバーローバーコンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand::CStand() : CScene_X::CScene_X()
{
	m_type = TYPE_BLOCK_NORMAL;	// タイプ
	m_bFall = false;		// 落ちる状態
	CScene::SetObj(CScene::OBJ::OBJ_STAND);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand::~CStand()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Init()
{
	CScene_X::Init();
	// 当たり判定の設定
	//CScene_X::SetCollision(CShape::SHAPETYPE_RECT, CCollision::OBJTYPE_BLOCK, false, true, NULL, D3DXVECTOR3(0.0, 50.0f, 0.0));
	//CRectCollision::Create(
	//	CScene_X::GetSize(),
	//	D3DXVECTOR3(0.0f,1000.0f,0.0f),
	//	CCollision::OBJTYPE_BLOCK,
	//	this,
	//	NULL,
	//	false,
	//	false
	//);
}	//

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Uninit(void)
{
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Update(void)
{
	CScene_X::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Draw(void)
{
	CScene_X::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ベースブロック全ソースの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CStand::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::UnLoad(void)
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)
//	pos			: 位置
//	nModelId	: モデル番号
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand * CStand::Create(
	D3DXVECTOR3		const & pos,		// 位置
	int				const & nModelId,	// モデル番号
	CScene::LAYER	const & layer		// レイヤー
)
{
	// 変数宣言
	CStand * pStand;				// シーン2Dクラス
									// メモリの生成(初め->基本クラス,後->派生クラス)
	pStand = new CStand();
	// 設定
	// シーン管理設定
	pStand->ManageSetting(layer);
	pStand->SetPos(pos);			// 位置
	pStand->SetModelId(nModelId);	// 位置
	// 初期化処理
	pStand->Init();
	// 生成したオブジェクトを返す
	return pStand;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)
//	pos			: 位置
//	nModelId	: モデル番号
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStand * CStand::Create_Self(
	D3DXVECTOR3		const & pos,									// 位置
	int				const & nModelId								// モデル番号
)
{
	// 変数宣言
	CStand * pStand;		// シーン2Dクラス
									// メモリの生成(初め->基本クラス,後->派生クラス)
	pStand = new CStand;
	// 設定
	pStand->SetPos(pos);			// 位置
	pStand->SetModelId(nModelId);	// 位置
										// 初期化処理
	pStand->Init();
	// 生成したオブジェクトを返す
	return pStand;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// unique_ptr作成(個人管理unique)
// ※戻り値はstd::moveで受け取る
//	pos			: 位置
//	nModelId	: モデル番号
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CStand> CStand::Creat_Unique(
	D3DXVECTOR3		const & pos,									// 位置
	int				const & nModelId								// モデル番号
)
{
	// 変数宣言
	std::unique_ptr<CStand> pStand(new CStand);		// シーン2Dクラス
																// 設定
	pStand->SetPos(pos);			// 位置
	pStand->SetModelId(nModelId);	// 位置
										// 初期化処理
	pStand->Init();
	// 生成したオブジェクトを返す
	return pStand;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// チュートリアル用足場生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::CreateStand_Tutorial()
{
	static D3DXVECTOR3 StandPos[3] = {};

	// 足場
	StandPos[0] = D3DXVECTOR3(-400, 0, 0);
	StandPos[1] = D3DXVECTOR3(0, 150, 0);
	StandPos[2] = D3DXVECTOR3(400, 300, 0);

	// チュートリアルで使用する足場ブロックの数
	for (int nBlockCnt = 0; nBlockCnt < 3; nBlockCnt++)
	{
		// 変数宣言
		CStand * pStand;		// シーン2Dクラス
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pStand = new CStand();
		// 設定
		// シーン管理設定
		pStand->ManageSetting(LAYER_3DBLOCK);				// レイヤーの番号
		pStand->SetModelId(3);								// モデルのid
		pStand->SetPos(StandPos[nBlockCnt]);				// 位置
		pStand->SetType(TYPE(TYPE_BLOCK_MAP1 + nBlockCnt));	// タイプ
		pStand->Init();										// 初期化処理

		// 看板
		CSignboard::Create(D3DXVECTOR3(
			StandPos[nBlockCnt].x,
			StandPos[nBlockCnt].y + 150.0f,
			StandPos[nBlockCnt].z));
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
	// シーン情報がNULLなら
	// ->関数を抜ける
	if (pScene == NULL) return;

	// オブジェクトタイプがプレイヤーなら
	if (nObjType == CCollision::OBJTYPE_PLAYER)
	{
		// 通常ブロック以外なら
		if (m_type != TYPE_BLOCK_NORMAL)
		{
			CPlayer *pPlayer = (CPlayer *)pScene;
			// プレイヤータグがプレイヤー1なら
			if (pPlayer->GetPlayerTag() == PLAYER_TAG::PLAYER_1)
			{
				m_bDetermination = true;
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene)
{
	// オブジェクトタイプがプレイヤーなら
	if (nObjType == CCollision::OBJTYPE_PLAYER)
	{
		CPlayer *pPlayer = (CPlayer *)pScene;
		// プレイヤータグがプレイヤー1なら
		if (pPlayer->GetPlayerTag() == PLAYER_TAG::PLAYER_1)
		{
			m_bDetermination = false;
		}
	}
}


#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CStand::Debug(void)
{

}
#endif // _DEBUG

