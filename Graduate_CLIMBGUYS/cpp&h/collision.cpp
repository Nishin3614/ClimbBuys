// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Collision処理の説明[collision.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Collision.h"
#include "debugproc.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define COLLISION_FILE "data/LOAD/collision.txt"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// グローバル変数
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
int CCollision::nCollisionTime = 0;
#endif

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCollision::CCollision() : CScene::CScene()
{
	// 初期化
	m_pOwner = NULL;
	m_pParent = NULL;
	m_bCollision = false;
	m_bUse = true;
	m_nMyObjectId = 0;
	m_nOponentId = -1;
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Debug(void)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posold;

	// 情報代入 //
	// 位置情報取得
	if (GetShape()->Get_PPos() != NULL)
	{
		pos = *GetShape()->Get_PPos();
	}
	else
	{
		pos = GetShape()->Get_Pos();
	}
	// 過去の位置情報取得
	if (GetShape()->Get_PPosold() != NULL)
	{
		posold = *GetShape()->Get_PPosold();
	}
	else
	{
		posold = GetShape()->Get_Posold();
	}
	/*
	ImGui::Text("Pos(%.1f,%.1f,%.1f)",
		pos.x,
		pos.y,
		pos.z
	);
	ImGui::Text("PosOld(%.1f,%.1f,%.1f)",
		posold.x,
		posold.y,
		posold.z
	);
	ImGui::Text("nCollisionTime:%d", nCollisionTime);
	*/
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自分から当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_MyCollision(
	int const &nObjType,	// オブジェクトタイプ
	CScene * pScene			// 相手のシーン情報
)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手から当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_OpponentCollision(
	int const &nObjType,	// オブジェクトタイプ
	CScene * pScene			// 相手のシーン情報
)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自分から当たらなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_NoMyCollision(
	int const &nObjType,	// オブジェクトタイプ
	CScene * pScene			// 相手のシーン情報
)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::Scene_NoOpponentCollision(
	int const &nObjType,	// オブジェクトタイプ
	CScene * pScene			// 相手のシーン情報
)
{
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 形クラスの選択
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::SelectShape(
	CShape * const shape
)
{
	// クラス型比較 //
	// 矩形クラス
	if (shape->GetType() == CShape::SHAPETYPE_RECT)
	{
		return Judg((CRectShape*)shape);
	}
	// 球クラス
	else if (shape->GetType() == CShape::SHAPETYPE_SPHERE)
	{
		return Judg((CSphereShape*)shape);
	}
	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定(指定)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::CollisionDetection(
	CCollision * pCollision
)
{
	// 引数の当たり判定情報がNULLの場合 ||
	// 引数の当たり判定が自分のあたり判定なら
	// ->関数を抜ける
	if (pCollision == NULL ||
		this == pCollision)
	{
		return false;
	}
	// 自分の当たり判定の使用状態がfalse ||
	// 相手の当たり判定の使用状態がfalse ||
	// ->関数を抜ける
	if (!this->m_bUse ||
		!pCollision->m_bUse)
	{
		return false;
	}
	// 自分のオブジェクト情報のNULLチェック ||
	// 相手のオブジェクトの親情報のNULLチェック ||
	// 自分のオブジェクト情報と相手のオブジェクトの親情報が同じなら
	// ->関数を抜ける
	if (this->m_pOwner == NULL ||
		this->m_pOwner == pCollision->m_pParent)
	{
		return false;
	}

	// 変数宣言
	bool bJudg = false;	// 当たり判定状態
	// クラス型比較 //
	// 矩形クラス
	if (pCollision->GetShape()->GetType() == CShape::SHAPETYPE_RECT)
	{
		bJudg = Judg((CRectShape*)pCollision->GetShape());
	}
	// 球クラス
	else if (pCollision->GetShape()->GetType() == CShape::SHAPETYPE_SPHERE)
	{
		bJudg = Judg((CSphereShape*)pCollision->GetShape());
	}
	// 円柱クラス
	else if (pCollision->GetShape()->GetType() == CShape::SHAPETYPE_COLUMN)
	{
		bJudg = Judg((CColumnShape*)pCollision->GetShape());
	}
	// 判定がtrueなら
	// ->情報を保存
	if (bJudg ==  true)
	{

#ifdef _DEBUG
		// テスト変数
		nCollisionTime++;
#endif // _DEBUG

		// 相手の当たり判定状態をtrueへ
		pCollision->m_bCollision = true;
		// 相手の番号を代入
		pCollision->m_nOponentId = m_nMyObjectId;
		// シーン情報がNULLではないなら
		// ->当たった後の処理を行う
		if (m_pOwner != NULL)
		{
			m_pOwner->Scene_MyCollision(pCollision->m_nMyObjectId, pCollision->m_pOwner);
		}
		// 相手のシーン情報がNULLではないなら
		// ->当たった後の処理を行う
		if (pCollision->m_pOwner != NULL)
		{
			pCollision->m_pOwner->Scene_OpponentCollision(m_nMyObjectId, m_pOwner);
		}
	}
	else
	{
		// シーン情報がNULLではないなら
		// ->当たった後の処理を行う
		if (m_pOwner != NULL)
		{
			m_pOwner->Scene_NoMyCollision(pCollision->m_nMyObjectId, pCollision->m_pOwner);
		}
		// 相手のシーン情報がNULLではないなら
		// ->当たった後の処理を行う
		if (pCollision->m_pOwner != NULL)
		{
			pCollision->m_pOwner->Scene_NoOpponentCollision(m_nMyObjectId, m_pOwner);
		}
	}
	// 当たり判定状態を返す
	return bJudg;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定(指定オブジェクト)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::CollisionDetection(OBJTYPE const & obj)
{
	// 変数宣言
	CCollision * pCollision;	// 当たり判定情報
	bool bCollision = false;	// 当たり判定状態
	// 処理
	for (int nCntLayer = 0; nCntLayer < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer++)
	{
		// 当たり判定取得
		pCollision = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer);
		// 当たり判定のNULLチェック ||
		// あたり判定が自分の当たり判定とアドレスが同じなら
		// ->ループスキップ
		if (pCollision == NULL ||
			pCollision == this) continue;
		// 指定したオブジェクトではないなら
		// ->ループスキップ
		else if (pCollision->m_nMyObjectId != obj) continue;
		// 当たり判定をtrueにする
		bCollision |= this->CollisionDetection(pCollision);
	}
	// 当たり判定状態を返す
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定(管理している全体)
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::CollisionDetection(void)
{
	// 変数宣言
	CCollision * pCollision1 = NULL;	// 当たり判定情報
	CCollision * pCollision2 = NULL;	// 当たり判定情報

	// 処理
	for (int nCntLayer1 = 0; nCntLayer1 < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer1++)
	{
		// 当たり判定取得
		pCollision1 = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer1);
		// 当たり判定情報がNULLなら
		if (pCollision1 == NULL)
		{
			continue;
		}
		// !オブジェクトタイプがプレイヤーなら
		else if (!(pCollision1->m_nMyObjectId == OBJTYPE_PLAYER)
			)
		{
			continue;
		}
		// 処理
		for (int nCntLayer2 = 0; nCntLayer2 < CScene::GetMaxLayer(LAYER_COLLISION); nCntLayer2++)
		{
			// 当たり判定取得
			pCollision2 = (CCollision *)CScene::GetScene(LAYER_COLLISION, nCntLayer2);
			// 引数の当たり判定情報がNULLの場合 ||
			// 引数の当たり判定が自分のあたり判定なら
			// ->関数を抜ける
			if (pCollision2 == NULL ||
				pCollision1 == pCollision2)
			{
				continue;
			}
			// 自分の当たり判定の使用状態がfalse ||
			// 相手の当たり判定の使用状態がfalse ||
			// ->関数を抜ける
			else if (!pCollision1->m_bUse ||
				!pCollision2->m_bUse)
			{
				continue;
			}
			// 自分のオブジェクト情報のNULLチェック ||
			// 相手のオブジェクトの親情報のNULLチェック ||
			// 自分のオブジェクト情報と相手のオブジェクトの親情報が同じなら
			// ->関数を抜ける
			else if (pCollision1->m_pOwner == NULL ||
				pCollision1->m_pOwner == pCollision2->m_pParent)
			{
				continue;
			}
			// 変数宣言
			bool bJudg = false;	// 当たり判定状態
								// クラス型比較 //
								// 矩形クラス
			if (pCollision2->GetShape()->GetType() == CShape::SHAPETYPE_RECT)
			{
				bJudg = pCollision1->Judg((CRectShape*)pCollision2->GetShape());
			}
			// 球クラス
			else if (pCollision2->GetShape()->GetType() == CShape::SHAPETYPE_SPHERE)
			{
				bJudg = pCollision1->Judg((CSphereShape*)pCollision2->GetShape());
			}
			// 円柱クラス
			else if (pCollision2->GetShape()->GetType() == CShape::SHAPETYPE_COLUMN)
			{
				bJudg = pCollision1->Judg((CColumnShape*)pCollision2->GetShape());
			}
			// 判定がtrueなら
			// ->情報を保存
			if (bJudg == true)
			{

#ifdef _DEBUG
				// テスト変数
				nCollisionTime++;
#endif // _DEBUG

				// 相手の当たり判定状態をtrueへ
				pCollision2->m_bCollision = true;
				// 相手の番号を代入
				pCollision2->m_nOponentId = pCollision1->m_nMyObjectId;
				// シーン情報がNULLではないなら
				// ->当たった後の処理を行う
				if (pCollision1->m_pOwner != NULL)
				{
 					pCollision1->m_pOwner->Scene_MyCollision(pCollision2->m_nMyObjectId, pCollision2->m_pOwner);
				}
				// それ以外なら
				// ->当たり判定自体の当たった後の処理を行う
				else
				{
					pCollision1->Scene_MyCollision(pCollision2->m_nMyObjectId, pCollision2->m_pOwner);
				}
				// 相手のシーン情報がNULLではないなら
				// ->当てられた後の処理を行う
				if (pCollision2->m_pOwner != NULL)
				{
					pCollision2->m_pOwner->Scene_OpponentCollision(pCollision1->m_nMyObjectId, pCollision1->m_pOwner);
				}
				// それ以外なら
				// ->当たり判定自体の当てられた後の処理を行う
				else
				{
					pCollision2->Scene_OpponentCollision(pCollision1->m_nMyObjectId, pCollision1->m_pOwner);
				}
			}
			else
			{
				// シーン情報がNULLではないなら
				// ->当たった後の処理を行う
				if (pCollision1->m_pOwner != NULL)
				{
					pCollision1->m_pOwner->Scene_NoMyCollision(pCollision2->m_nMyObjectId, pCollision2->m_pOwner);
				}
				// 相手のシーン情報がNULLではないなら
				// ->当たった後の処理を行う
				if (pCollision2->m_pOwner != NULL)
				{
					pCollision2->m_pOwner->Scene_NoOpponentCollision(pCollision1->m_nMyObjectId, pCollision1->m_pOwner);
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定状態の強制falseへ
// 当たった後の行動が起こった時に判定をfalseにする
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::SetCollision(void)
{
	m_bCollision = false;
	m_nOponentId = -1;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// あたり判定を所有しているシーン情報を強制NULL代入
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCollision::CompulsionScene(void)
{
	m_pOwner = NULL;
	m_pParent = NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 矩形と矩形の判定
//	pRectShapeA	: 矩形A
//	pRectShapeB	: 矩形B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::RectAndRect(
	CRectShape * const pRectShapeA,	// 矩形A
	CRectShape * const pRectShapeB	// 矩形B
)
{
	// 変数宣言
	D3DXVECTOR3 const &min_A = pRectShapeA->GetMin();
	D3DXVECTOR3 const &min_B = pRectShapeB->GetMin();
	D3DXVECTOR3 const &max_A = pRectShapeA->GetMax();
	D3DXVECTOR3 const &max_B = pRectShapeB->GetMax();
	D3DXVECTOR3 const &minOld_A = pRectShapeA->GetMinOld();
	D3DXVECTOR3 const &minOld_B = pRectShapeB->GetMinOld();
	D3DXVECTOR3 const &maxOld_A = pRectShapeA->GetMaxOld();
	D3DXVECTOR3 const &maxOld_B = pRectShapeB->GetMaxOld();
	D3DXVECTOR3 *pos_A = NULL;
	bool bCollision = false;
	// ポインター位置情報がNULLではないなら
	// ->位置情報に代入
	if (pRectShapeA->Get_PPos() != NULL)
	{
		pos_A = pRectShapeA->Get_PPos();
	}
	// 接触していないときはfalseを返す
	if (max_A.y > min_B.y &&
		min_A.y < max_B.y)
	{
		// 素材のZ範囲
		if (max_A.z > min_B.z&&
			min_A.z < max_B.z)
		{
			// 当たり判定(左)
			if (max_A.x > min_B.x &&
				maxOld_A.x <= min_B.x)
			{
				// 過去の位置情報AがNULLではないなら ||
				// 押し出し処理がありなら
				// ->押し出し処理
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// 素材状の左に
					pos_A->x = min_B.x - pRectShapeA->GetSize().x * 0.6f;
				}
				// 接触しているときはtrueを返す
				bCollision = true;
			}

			// 当たり判定(右)
			else if (min_A.x < max_B.x &&
				minOld_A.x >= max_B.x)
			{
				// 過去の位置情報AがNULLではないなら ||
				// 押し出し処理がありなら
				// ->押し出し処理
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// 素材状の左に
					pos_A->x = max_B.x + pRectShapeA->GetSize().x * 0.6f;
				}
				// 接触しているときはtrueを返す
				bCollision = true;
			}
		}
		// 素材のX範囲
		if (max_A.x > min_B.x&&
			min_A.x < max_B.x)
		{
			// 当たり判定(手前)
			if (max_A.z > min_B.z&&
				maxOld_A.z <= min_B.z)
			{
				// 過去の位置情報AがNULLではないなら ||
				// 押し出し処理がありなら
				// ->押し出し処理
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// 素材状の左に
					pos_A->z = min_B.z - pRectShapeA->GetSize().z * 0.6f;
				}
				// 接触しているときはtrueを返す
				bCollision = true;
			}

			// 当たり判定(奥)
			else if (min_A.z < max_B.z &&
				minOld_A.z >= max_B.z)
			{
				// 過去の位置情報AがNULLではないなら ||
				// 押し出し処理がありなら
				// ->押し出し処理
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// 素材状の左に
					pos_A->z = max_B.z +
						pRectShapeA->GetSize().z * 0.6f;
				}
				// 接触しているときはtrueを返す
				bCollision = true;
			}

		}
	}
	// 素材のZ範囲
	if (max_A.z > min_B.z&&
		min_A.z < max_B.z)
	{
		// 素材のX範囲
		if (max_A.x > min_B.x&&
			min_A.x < max_B.x)
		{
			// 当たり判定(下)
			if (max_A.y > min_B.y &&
				maxOld_A.y <= min_B.y)
			{
				// 過去の位置情報AがNULLではないなら ||
				// 押し出し処理がありなら
				// ->押し出し処理
				if (pos_A != NULL ||
					pRectShapeA->m_bPush &&
					pRectShapeB->m_bOpponentPush
					)
				{
					// 素材状の左に
					pos_A->y = min_B.y - pRectShapeA->GetSize().y;
				}
				// 接触しているときはtrueを返す
 				bCollision = true;
			}

			// 当たり判定(上)
			else if (min_A.y < max_B.y&&
				minOld_A.y >= max_B.y)
			{
				// 過去の位置情報AがNULLではないなら ||
				// 押し出し処理がありなら
				// ->押し出し処理
				if (pos_A != NULL ||
					pRectShapeA->m_bPush)
				{
					// 素材状の左に
					pos_A->y = max_B.y + 0.1f;
				}
				// 接触しているときはtrueを返す
				bCollision = true;
			}

		}
	}
	// 当たっていたら更新
	if (bCollision)
	{
		// 位置情報更新
		pRectShapeA->PassPos(D3DVECTOR3_ZERO);
	}
	// 接触しているときはtrueを返す
	return bCollision;






	// やること
	// いったん情報を整理する
	// いらない情報を消す

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 矩形と球の当たり判定
//	pRectShapeA		: 矩形A
//	pSphereShapeB	: 球B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::RectAndSphere(
	CRectShape * const pRectShapeA,
	CSphereShape * const pSphereShapeB
)
{
	// 変数宣言
	D3DXVECTOR3 ClosestPoint;	// ある座標の最も近い、ボックス上の座標
	D3DXVECTOR3 *pos_A;			//
	D3DXVECTOR3 *pos_B;			//
	bool bCollision = false;	// 当たっているかどうか
	// 矩形Aのポイント位置情報がNULLではない場合
	// 位置情報代入
	if (pRectShapeA->Get_PPos() != NULL)
	{
		pos_A = pRectShapeA->Get_PPos();
	}
	else
	{
		pos_A = &pRectShapeA->Get_Pos();
	}
	// スフィアBのポイント位置情報がNULLではない場合
	// 位置情報代入
	if (pSphereShapeB->Get_PPos() != NULL)
	{
		pos_B = pSphereShapeB->Get_PPos();
	}
	else
	{
		pos_B = &pSphereShapeB->Get_Pos();
	}

	// ある座標の最も近い、ボックス上の座標
	ClosestPoint = pRectShapeA->GetClosestpoint(*pos_B + pSphereShapeB->GetOffset());
	float a = CCalculation::DiffPointSquare(*pos_B + pSphereShapeB->GetOffset(), ClosestPoint);
	// 当たり判定処理
	bCollision = CCalculation::DiffPointSquare(*pos_B + pSphereShapeB->GetOffset(), ClosestPoint) <
		pSphereShapeB->GetRadius() * pSphereShapeB->GetRadius();
	// 当たっていたら &&
	// 矩形Aの押し出し処理がtrueなら &&
	// 球Bの相手に対する押し出し処理がtrueなら
	// ->矩形Aが押し出される
	if (bCollision &&
		pRectShapeA->m_bPush &&
		pSphereShapeB->m_bOpponentPush
		)
	{

	}
	// 当たっていたら &&
	// 矩形Bの押し出し処理がtrueなら &&
	// 矩形Aの相手に対する押し出し処理がtrueなら
	// ->球Bが押し出される
	else if (bCollision &&
		pRectShapeA->m_bOpponentPush &&
		pSphereShapeB->m_bPush
		)
	{
		// 変数宣言
		D3DXVECTOR3 A_To_BVec;	// AからBのベクトル
		A_To_BVec = *pos_B + pSphereShapeB->GetOffset() - ClosestPoint;
		D3DXVec3Normalize(&A_To_BVec, &A_To_BVec);
		*pos_B = ClosestPoint + A_To_BVec * pSphereShapeB->GetRadius() - pSphereShapeB->GetOffset();
	}
	// 距離が半径より短い場合true,それ以外falseを返す
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 矩形と円柱の当たり判定
//	pRectShapeA		: 矩形A
//	pColumnShapeB	: 円柱B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::RectAndColumn(
	CRectShape * const pRectShapeA,
	CColumnShape * const pColumnShapeB
)
{
	return false;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 球と円柱の当たり判定
//	pSphereShapeA	: 球A
//	pColumnShapeB	: 円柱B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::SphereAndColumn(
	CSphereShape * const pSphereShapeA,
	CColumnShape * const pColumnShapeB
)
{
	// 変数宣言
	D3DXVECTOR3 *pos_A;			// 位置A
	D3DXVECTOR3 *pos_B;			// 位置B
	bool bCollision = false;	// 当たり判定状態
	// スフィアAのポイント位置情報がNULLではない場合
	// 位置情報代入
	if (pSphereShapeA->Get_PPos() != NULL)
	{
		pos_A = pSphereShapeA->Get_PPos();
	}
	else
	{
		pos_A = &pSphereShapeA->Get_Pos();
	}
	// スフィアBのポイント位置情報がNULLではない場合
	// 位置情報代入
	if (pColumnShapeB->Get_PPos() != NULL)
	{
		pos_B = pColumnShapeB->Get_PPos();
	}
	else
	{
		pos_B = &pColumnShapeB->Get_Pos();
	}

	// y制限
	if ((pos_B->y + pColumnShapeB->GetOffset().y + pColumnShapeB->GetVertical()) <
		(pos_A->y + pSphereShapeA->GetOffset().y - pSphereShapeA->GetRadius()))
	{
		return false;
	}
	else if ((pos_B->y + pColumnShapeB->GetOffset().y - pColumnShapeB->GetVertical()) >
		(pos_A->y + pSphereShapeA->GetOffset().y + pSphereShapeA->GetRadius()))
	{
		return false;
	}
	// 球と円柱の距離の比較
	// 当たっているかどうか
	bCollision = CCalculation::Collision_Sphere(
		*pos_A + pSphereShapeA->GetOffset(),
		pSphereShapeA->GetRadius(),
		*pos_B + pColumnShapeB->GetOffset(),
		pColumnShapeB->GetRadius()
	);
	if (bCollision)
	{
		// 当たり判定表示
		CDebugproc::Print("球と円柱の当たり判定状態 = 1\n");
	}
	else
	{
		// 当たり判定表示
		CDebugproc::Print("球と円柱の当たり判定状態 = 0\n");
	}
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 球と球の当たり判定
//	pSphereShapeA	: 球A
//	pSphereShapeB	: 球B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::SphereAndSphere(
	CSphereShape * const pSphereShapeA,
	CSphereShape * const pSphereShapeB
)
{
	// 変数宣言
	D3DXVECTOR3 *pos_A;
	D3DXVECTOR3 *pos_B;
	bool bCollision = false;
	// スフィアAのポイント位置情報がNULLではない場合
	// 位置情報代入
	if (pSphereShapeA->Get_PPos() != NULL)
	{
		pos_A = pSphereShapeA->Get_PPos();
	}
	else
	{
		pos_A = &pSphereShapeA->Get_Pos();
	}
	// スフィアBのポイント位置情報がNULLではない場合
	// 位置情報代入
	if (pSphereShapeB->Get_PPos() != NULL)
	{
		pos_B = pSphereShapeB->Get_PPos();
	}
	else
	{
		pos_B = &pSphereShapeB->Get_Pos();
	}

	// 変数宣言
	//D3DXVECTOR3 posA = pSphereShapeA->m_DestPos;	// 位置A
	//D3DXVECTOR3 posB = pSphereShapeB->m_DestPos;	// 位置B
	// 当たっているかどうか
	bCollision = CCalculation::Collision_Sphere(
		*pos_A + pSphereShapeA->GetOffset(),
		pSphereShapeA->GetRadius(),
		*pos_B + pSphereShapeB->GetOffset(),
		pSphereShapeB->GetRadius()
	);
	// 当たっていたら &&
	// 球Aの押し出し処理がtrueなら &&
	// 球Bの相手に対する押し出し処理がtrueなら
	// ->球Aが押し出される
	if (bCollision &&
		pSphereShapeA->m_bPush &&
		pSphereShapeB->m_bOpponentPush
		)
	{
		D3DXVECTOR3 diff = *pos_A - *pos_B;						// BからAの差
		D3DXVECTOR3 vec;									// B->Aのベクトル
															// ベクトルの正規化
		D3DXVec3Normalize(&vec, &diff);
		// 押し出し
		*pos_A = *pos_B + vec * (pSphereShapeA->GetRadius() + pSphereShapeB->GetRadius());

		pSphereShapeA->PassPos(D3DVECTOR3_ZERO);
	}
	return bCollision;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 円柱と円柱の当たり判定
//	pColumnShapeA	: 円柱A
//	pColumnShapeB	: 円柱B
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CCollision::ColumnAndColumn(
	CColumnShape * const pColumnShapeA,
	CColumnShape * const pColumnShapeB
)
{
	return false;
}
