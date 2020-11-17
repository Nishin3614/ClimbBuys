// ----------------------------------------------------------------------------------------------------
//
// SphereCollisionヘッダー処理の説明[spherecollision.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _SPHERECOLLISION_H_
#define _SPHERECOLLISION_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#include "collision.h"
#include "meshsphere.h"

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
#define SPHERECOLLISION_MAX (128)

// ----------------------------------------------------------------------------------------------------
//
// SphereCollision種類
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 名前定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef _DEBUG
class CMeshsphere;	// メッシュスフィアクラス
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CSphereCollision : public CCollision
{
public:
	/* 列挙型 */
	/* 関数 */
	CSphereCollision();
	virtual ~CSphereCollision() {};
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
#ifdef _DEBUG
	virtual void Debug(void);
	// あたり判定可視カの設定
	virtual void Collision_Visible_Set(void);
#endif // _DEBUG
	// 当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	CShape * const GetShape(void) { return m_pSphereShape.get(); };
	//
	// 矩形クラスの当たり判定比較
	bool Judg(CRectShape * const RectShape);
	// 矩形クラスの当たり判定比較(押し出し処理)
	bool Judg(CRectShape * const RectShape, D3DXVECTOR3 * pPos);
	// 球クラスの当たり判定比較
	bool Judg(CSphereShape * const SphereShape);
	// 円柱クラスの当たり判定比較
	bool Judg(CColumnShape * const ColumnShape);
	// 設定
	// 作成処理(シーン管理)
	static CSphereCollision *Create(
		float const &fRadius,
		D3DXVECTOR3 const offset = D3DVECTOR3_ZERO,
		OBJTYPE const &obj = OBJTYPE_ACTOR,
		CScene * pScene = NULL,
		CScene * pParent = NULL,
		bool const &bPush = false,
		bool const &bOpponent = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
		);
	// 作成処理(個人管理)
	static std::unique_ptr<CSphereCollision> Create_Self(
		float const &fRadius,
		D3DXVECTOR3 const offset = D3DVECTOR3_ZERO,
		OBJTYPE const &obj = OBJTYPE_ACTOR,
		CScene * pScene = NULL,
		CScene * pParent = NULL,
		bool const &bPush = false,
		bool const &bOpponent = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
protected:

private:
	// 変数宣言
	std::unique_ptr<CSphereShape> m_pSphereShape;	// 球
#ifdef _DEBUG
	// あたり判定可視化の変数
	CMeshsphere * m_pDebugSphere;
#endif // _DEBUG
};

// ----------------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ----------------------------------------------------------------------------------------------------

#endif