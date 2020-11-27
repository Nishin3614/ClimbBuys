// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 演出ブロック処理 [stagingblock.h]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _STAGINGBLOCK_H_
#define _STAGINGBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "scene_x.h"
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_STAGINGBLOCK (9)

// 仮
#define SPACE_Y (500)
// タイトルロゴ形成用 ブロック座標
// CLIMBGUYS
#define STAGINGBLOCK_POS_C (D3DXVECTOR3(-600.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L (D3DXVECTOR3(-450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I (D3DXVECTOR3(-300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M (D3DXVECTOR3(-150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B (D3DXVECTOR3(0.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_G (D3DXVECTOR3(150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U (D3DXVECTOR3(300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y (D3DXVECTOR3(450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S (D3DXVECTOR3(600.0f, 0.0f + SPACE_Y, 0));

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CStagingBlock : public CScene_X
{
public:
	/* 関数 */
	// コンストラクタ
	CStagingBlock();
	// デストラクタ
	~CStagingBlock();
	// 初期化処理
	void Init(void);
	// 更新処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);

	// ベースブロック全ソースの読み込み
	static HRESULT Load(void);
	// ベースブロック全ソースの開放
	static void UnLoad(void);

	// メモリの確保
	static CStagingBlock *Create(
		D3DXVECTOR3 const &pos,						// 位置
		D3DXVECTOR3 const &rot,						// 回転
		D3DXVECTOR3 const &size,					// サイズ倍率
		D3DXCOLOR color,							// カラー
		int const &nModelId = 0,					// モデル番号
		bool const &bShadowMap = false				// シャドウマッピング状態
	);

	void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);

	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void);
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void);
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void);

	// ブロックの番号取得
	int GetBlockNum() {return m_nBlockNun;};
	// ブロックの番号設定
	void SetBlockNum(int num) { m_nBlockNun = num; };
	// ブロックが飛んでいくベクトルの設定
	void SetBurstVector(D3DXVECTOR3 vector) { m_Vector = vector; };
	// ブロックのバーストベクトルの取得
	D3DXVECTOR3 GetBurstVector() { return m_Vector; };
	// ブロックを停止させる設定
	void SetMoveStop(bool stop) { m_bStopMove = stop; };
	// ブロックの停止状態を取得
	bool GetMoveStop() { return m_bStopMove; };

	// 演出終了フラグの設定
	static void SetEnd(bool end) { m_bEnd = end; };
	// 演出終了フラグの取得
	static bool GetEnd() { return m_bEnd; };


	// ブロックの移動処理
	void BlockFall();
	// ブロックの破裂
	void BlockBurst();

protected:
private:
	/* 関数 */
	void				Levitating();							// 空中浮遊 タイトルで使う
	/* 変数 */
	static bool			m_bEnd;									// 演出終了フラグ

	float				m_fSpeed;								// 移動量
	float				m_fBurstSpeed;							// 移動量

	D3DXVECTOR3			m_Vector;								// ブロックが飛んでいく方向ベクトル
	int					m_nBlockNun;							// ブロックの番号
	bool				m_bStopMove;							// 座標の更新を止める
	int					m_nAngle;								// 角度
	int					m_nTime;								// パーティクルの出現時間カウント
	CTitle				*m_pTitle;								// タイトルのポインタ
};

#endif