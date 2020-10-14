// ----------------------------------------
//
// デバッグ用の当たり判定表示処理の説明[debugcollision.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "debugcollision.h"
#include "3Dline.h"

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
CDebugcollision::CDebugcollision()
{
	m_ppos = NULL;
	m_size = NULL;
	m_type = COLLISIONTYPE_RECT;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CDebugcollision::~CDebugcollision()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CDebugcollision::Init(void)
{
	switch (m_type)
	{
	case COLLISIONTYPE_RECT:
		break;
	default:
		break;
	}
}

// ----------------------------------------
// 矩形の当たり判定可視化生成
// ----------------------------------------
void CDebugcollision::Create_Rect(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 const &size
)
{
	// やること
	// 判定のやつとコネクトさせる





	// 1つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f)
	);
	// 2つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f)
	);
	// 3つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f)
	);
	// 4つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f)
	);
	// 5つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
	// 6つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
	// 7つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 8つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 9つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
	// 10つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 11つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f)
	);
	// 12つ目
	C3DLine::Create(
		pos,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, size.z * 0.5f),
		D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, size.z * 0.5f)
	);
}

// ----------------------------------------
// 球の当たり判定可視化生成
// ----------------------------------------
void CDebugcollision::Create_Sphere(
	D3DXVECTOR3 * pos,
	float const fRadius
)
{


}