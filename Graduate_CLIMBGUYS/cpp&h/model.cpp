//=============================================================================
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "model.h"
#include "light.h"
#include "shadow_mapping.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel::CModel()
{
	m_posDest = D3DVECTOR3_ZERO;
	m_posDiff = D3DVECTOR3_ZERO;
	m_rotDest = D3DVECTOR3_ZERO;
	m_rotDiff = D3DVECTOR3_ZERO;
	m_nFrame = 0;
	m_nParent = 0;
	m_nFrame = 0;
	m_fAlpha = 1.0f;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel::~CModel()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::Init(void)
{
	m_nFrame = 0;
	// シーンXの初期化
	CScene_X::Init();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::Uninit(void)
{
	// シーンXの終了
	CScene_X::Uninit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 &pos = CScene_X::GetPos();	// 位置
	D3DXVECTOR3 &rot = CScene_X::GetRot();	// 回転
	// モデル事のモーション動作
	pos += m_posDiff / (float)m_nFrame;
	rot += m_rotDiff / (float)m_nFrame;
	// シーンXの更新
	CScene_X::Update();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::Draw(void)
{
	// シーンXの描画
	CScene_X::Draw();
	SetModelAlpha(m_fAlpha);
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::Debug(void)
{
	CDebugproc::Print("	pos(%.3f,%.3f,%.3f)\n", CScene_X::GetPos().x, CScene_X::GetPos().y, CScene_X::GetPos().z);
	CDebugproc::Print("	rot(%.3f,%.3f,%.3f)\n", CScene_X::GetRot().x, CScene_X::GetRot().y, CScene_X::GetRot().z);
}
#endif // _DEBUG


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel * CModel::Create()
{
	// 変数宣言
	CModel * pModel;	// モデル
	// メモリ確保
	pModel = new CModel();
	// 初期化
	pModel->Init();
	// 情報を返す
	return pModel;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルの情報渡し処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::BindModel(
	MODEL_OFFSET const &offset	// モデルのオフセット
)
{
	// 変数宣言
	D3DXVECTOR3 &pos = CScene_X::GetPos();	// 位置
	D3DXVECTOR3 &rot = CScene_X::GetRot();	// 回転
	// モデルのオフセット
	pos = offset.pos;
	rot = offset.rot;
	m_nParent = offset.nParent;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期モーションの設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::BeginMotion(
	KEY const &key,
	int const nMaxFrame
)
{
	// 変数宣言
	D3DXVECTOR3 &pos = CScene_X::GetPos();	// 位置
	D3DXVECTOR3 &rot = CScene_X::GetRot();	// 回転
	pos = key.pos;
	rot = key.rot;
	m_posDest = key.pos;
	m_rotDest = key.rot;
	m_posDiff = (m_posDest - pos);
	m_rotDiff = (m_rotDest - rot);
	m_nFrame = nMaxFrame;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーションの設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel::SetMotion(
	KEY const &key,
	int const nMaxFrame
)
{
	// 変数宣言
	D3DXVECTOR3 &pos = CScene_X::GetPos();	// 位置
	D3DXVECTOR3 &rot = CScene_X::GetRot();	// 回転
	m_posDest = key.pos;
	m_rotDest = key.rot;
	m_posDiff = (m_posDest - pos);
	m_rotDiff = (m_rotDest - rot);
	m_nFrame = nMaxFrame;
}
