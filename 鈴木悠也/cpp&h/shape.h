// ----------------------------------------------------------------------------------------------------
//
// Shapeヘッダー処理の説明[shape.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _SHAPE_H_
#define _SHAPE_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// Shape種類
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 基底の形クラス
class CShape
{
public:
	/* 列挙型 */
	// 形の種類
	typedef enum
	{
		SHAPETYPE_RECT = 0,
		SHAPETYPE_SPHERE,
		SHAPETYPE_COLUMN,
		SHAPETYPE_MAX
	} SHAPETYPE;
	/* 関数 */
	CShape();
	virtual ~CShape() {};
	// 更新処理
	virtual void Update(void) = 0;
	// 形の種類
	virtual SHAPETYPE GetType(void) const = 0;
	// 行列情報受け渡し
	// 1:行列情報
	virtual void PassMatrix(D3DXMATRIX const &mtx);
	// 位置と回転情報受け渡し
	// 1:位置情報,2:開店情報
	virtual void PassPos(D3DXVECTOR3 const & rot);
	// 位置情報受け渡し
	// 1:位置情報
	// オフセット位置設定
	void SetOffset(D3DXVECTOR3 const &offset) { m_offset = offset; };
	// オフセット位置取得
	D3DXVECTOR3 const &GetOffset(void) { return m_offset; };
	// ポインター位置情報の設定
	inline void Set_PPos(D3DXVECTOR3 * pPos) { m_pCorePos = m_pPos = pPos; };
	// ポインター位置情報の取得
	D3DXVECTOR3 * Get_PPos(void) { return m_pPos; };
	// ポインター位置情報の設定
	void Set_PPosold(D3DXVECTOR3 * pPosold) { m_pCorePosOld = m_pPosold = pPosold; };
	// ポインター位置情報の取得
	D3DXVECTOR3 * Get_PPosold(void) { return m_pPosold; };
	// 位置情報の設定
	void Set_Pos(D3DXVECTOR3 pos) { m_pos = pos; };
	// 位置情報の取得
	D3DXVECTOR3 & Get_Pos(void) { return m_pos; };
	// 過去の位置情報の設定
	void Set_Posold(D3DXVECTOR3 posold) { m_posold = posold; };
	// 過去の位置情報の取得
	D3DXVECTOR3 & Get_Posold(void) { return m_posold; };
	// 絶対的過去の位置情報の設定
	D3DXVECTOR3 * Get_PosCore(void) { return m_pCorePos; };
	// 絶対的位置情報の取得
	D3DXVECTOR3 * Get_PosCoreold(void) { return m_pCorePosOld; };
	// 最終的な位置情報設定
	void Set_DestPos(void)
	{
		m_DestPos = *m_pCorePos + m_offset;
		m_DestPosOld = *m_pCorePosOld + m_offset;
	};

	// 変数 //
	D3DXVECTOR3 * m_pmove;			// 移動量
	D3DXVECTOR3 m_DestPos;			// オフセットを足した、最終的な位置情報
	D3DXVECTOR3 m_DestPosOld;		// オフセットを足した、過去の最終的な位置情報
	D3DXVECTOR3 * m_pCorePos;		// 位置情報の絶対的変数
	D3DXVECTOR3 * m_pCorePosOld;	// 過去の位置情報の絶対的変数
	bool		m_bPush;			// 押し出し処理があるかない
	bool		m_bOpponentPush;	// 相手を押し出すかどうか
protected:

private:
	D3DXVECTOR3 * m_pPos;			// ポインター位置情報
	D3DXVECTOR3 m_pos;				// 位置情報
	D3DXVECTOR3 * m_pPosold;		// 過去のポインター位置情報
	D3DXVECTOR3 m_posold;			// 過去の位置情報
	D3DXVECTOR3 m_offset;			// オフセット座標
};

// 球のクラス
class CSphereShape : public CShape
{
public:
	/* 列挙型 */

	/* 関数 */
	CSphereShape() {};
	virtual ~CSphereShape() {};
	// 更新処理
	virtual void Update(void) {};
	// 作成
	static std::unique_ptr<CSphereShape> Create(
		D3DXVECTOR3 const &offset,
		float const &radius,
		bool const &bPush = false,
		bool const &bOpponentPush = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// 行列情報受け渡し
	// 1:行列情報
	void PassMatrix(D3DXMATRIX const &mtx);
	// 位置と回転情報受け渡し
	// 1:位置情報,2:開店情報
	void PassPos(
		D3DXVECTOR3 const & rot
	);
	// 設定 //
	// 半径設定
	void SetRadius(float const radius) { m_radius = radius; };
	// 取得 //
	// 形の種類取得
	SHAPETYPE GetType(void) const { return SHAPETYPE_SPHERE; };
	// 半径取得
	float GetRadius(void) const { return m_radius; };
protected:

private:
	float m_radius;
};

// 矩形のクラス
class CRectShape : public CShape
{
public:
	/* 列挙型 */

	/* 型宣言 */

	/* 関数 */
	CRectShape()
	{
		m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Min.x = m_Min.y = m_Min.z = FLT_MAX;
		m_Max.x = m_Max.y = m_Max.z = -FLT_MAX;
		m_MinOld.x = m_MinOld.y = m_MinOld.z = FLT_MAX;
		m_MaxOld.x = m_MaxOld.y = m_MaxOld.z = -FLT_MAX;
	};
	virtual ~CRectShape() {};
	// 更新処理
	virtual void Update(void) {};
	// 最大座標値と最小座標値の計算
	// 1:行列情報
	void PassMatrix(D3DXMATRIX const &mtx);
	// 最大座標値と最小座標値の計算
	// 1:位置情報
	void PassPos(
		D3DXVECTOR3 const & rot
	);
	// ある座標から最も近い、ボックス上の座標を返す
	D3DXVECTOR3 GetClosestpoint(D3DXVECTOR3 const &pos);
	// 作成
	static std::unique_ptr<CRectShape> Create(
		D3DXVECTOR3 const &offset,
		D3DXVECTOR3 const &size,
		bool const &bPush = false,
		bool const &bOpponentPush = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// 設定 //
	// 半径設定
	void SetSize(D3DXVECTOR3 const size) { m_size = size; };
	// 取得 //
	// 形の種類取得
	SHAPETYPE GetType(void) const { return SHAPETYPE_RECT; };
	// 半径取得
	D3DXVECTOR3 GetSize(void) const { return m_size; };
	// 最大座標値取得
	D3DXVECTOR3 GetMax(void) const & { return m_Max; };
	// 最小座標値取得
	D3DXVECTOR3 GetMin(void) const & { return m_Min; };
	// 過去の最大座標値取得
	D3DXVECTOR3 GetMaxOld(void) const & { return m_MaxOld; };
	// 過去の最小座標値取得
	D3DXVECTOR3 GetMinOld(void) const & { return m_MinOld; };
protected:

private:
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_Min;
	D3DXVECTOR3 m_Max;
	D3DXVECTOR3 m_MinOld;
	D3DXVECTOR3 m_MaxOld;
};

// 円柱のクラス
class CColumnShape : public CShape
{
public:
	/* 列挙型 */
	/* 関数 */
	CColumnShape() {};
	virtual ~CColumnShape() {};
	// 更新処理
	virtual void Update(void) {};
	// 作成
	static std::unique_ptr<CColumnShape> Create(
		D3DXVECTOR3 const &offset,
		float const & fRadius,
		float const & fVertical,
		bool const &bPush = false,
		bool const &bOpponentPush = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// 行列情報受け渡し
	// 1:行列情報
	// 未解決
	void PassMatrix(D3DXMATRIX const &mtx) {};
	// 位置と回転情報受け渡し
	// 1:位置情報,2:開店情報
	// 未解決
	void PassPos(
		D3DXVECTOR3 const & rot
	) {};
	// 設定 //
	// 半径設定
	void SetRadius(float const & fRadius) { m_fRadius = fRadius; };
	// 縦幅設定
	void SetVertical(float const & fVertical) { m_fVertical = fVertical; };
	// 取得 //
	// 形の種類取得
	SHAPETYPE GetType(void) const { return SHAPETYPE_COLUMN; };
	// 半径取得
	float GetRadius(void) const { return m_fRadius; };
	// 縦幅取得
	float GetVertical(void) const { return m_fVertical; };
protected:

private:
	float m_fRadius;
	float m_fVertical;
};

#endif