// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ダメージ床[damagefloor.h]
// Author : Fujiwara Masato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _DAMAGEFLOOR_H_
#define _DAMAGEFLOOR_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_three.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayer;
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CDamageFloor : public CScene_THREE
{
public:
	/* 列挙型 */

	/* 関数 */
	CDamageFloor();
	~CDamageFloor();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 作成(シーン継承あり)
	static CDamageFloor * Create();						// 作成
	static HRESULT Load(void);							// 読み込み
	static void UnLoad(void);							// 破棄
	// 設定 //

	// 取得 //
	float GetDamageFloorMoveSpeed() { return m_MoveSpeed; };// 床の移動速度の取得
protected:
private:
	/* 関数 */
	void ComparisonHeight();							// 高さを比較
	void Move();										// 床の移動
	/* 変数 */
	float m_MoveSpeed;									// 移動量の速さ
	int m_AscendUpToTime;								// 床の上昇が始まるまでの時間 仮
	CPlayer *m_pPlayer[4];								// 四人分のポインタ
	int	m_nDieCnt;										// 死亡人数 仮
};
#endif