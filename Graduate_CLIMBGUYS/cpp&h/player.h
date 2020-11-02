// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル処理 [player.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"
#include "baseblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_FRONTFORCE		(10)
#define PLAYER_G				(0.5f)								// 重力
#define PLAYER_RESISTANCE		(0.5f)								// 抵抗力

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CMeshBox;	// メッシュボックスクラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayer : public CCharacter, public CPlayerTAG
{
public:
	// ---------モーションタイプ---------- //
	typedef enum
	{
		MOTIONTYPE_MAX = CCharacter::MOTIONTYPE_MAX
	} MOTIONTYPE;
	// 当たり判定
	typedef enum
	{
		COLLISIONTYPE_CHARACTER = 0,
		COLLISIONTYPE_PUSH,
		COLLISIONTYPE_MAX
	} COLLISIONTYPE;
	// 当たり判定
	typedef enum
	{
		DIESTATE_UP		= 0b000001,	// 上
		DIESTATE_UNDER	= 0b000010,	// 下
		DIESTATE_LEFT	= 0b000100,	// 左
		DIESTATE_RIGHT	= 0b001000,	// 右
		DIESTATE_FRONT	= 0b010000,	// 前
		DIESTATE_BACK	= 0b100000,	// 後ろ
		DIESTATE_MAX
	} DIESTATE;

	// ----- プレイヤーのステータス ----- //
	typedef struct
	{
		float				fMove;				// 移動量
		float				fJump;				// ジャンプ力
		float				fDash;				// ダッシュの移動量
		float				fNormalInertia;		// 通常時の慣性
		float				fJumpInertia;		// ジャンプ時の慣性
		D3DXVECTOR3			PlayerSize;			// プレイヤーサイズ
		D3DXVECTOR3			PlayerOffSet;		// プレイヤーオフセット
		D3DXVECTOR3			PushSize;			// 押し出し用のサイズ
		D3DXVECTOR3			PushOffSet;			// 押し出し用のオフセット
	}PLAYER_STATUS;

	/* 関数 */
	// コンストラクタ
	CPlayer(CCharacter::CHARACTER const &character);
	// デストラクタ
	virtual ~CPlayer();
	// 初期化処理
	virtual void Init(void);
	// 終章処理
	virtual void Uninit(void);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);
	// プレイヤーが死んだときの関数
	virtual void Die(void);
	void OtherDie(void);
	// 死亡フラグの取得
	bool GetDieFlag(void) { return m_bDieFlag; };

	// 当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	);
	// 押し出し当たり判定
	//	Obj		: オブジェタイプ
	//	pos		: 位置
	//	posOld	: 前回の位置
	//	move	: 移動量
	//	size	: サイズ
	COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * move,								// 移動量
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	);
	// 当たり判定
	//	Obj		: オブジェタイプ
	//	pos			: 位置
	//	size		: サイズ
	//	OffsetPos	: オフセット位置
	COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// 生成処理
	//	tag	: プレイヤータグ
	//	pos	: 位置
	static CPlayer * Create(
		PLAYER_TAG tag,								// プレイヤータグ
		D3DXVECTOR3 const & pos = D3DVECTOR3_ZERO	// 位置
);
	// リソースの読み込み処理
	static HRESULT Load(void);
	// リソースの開放処理
	static void UnLoad(void);
	// ダッシュしているかどうかのフラグの設定
	void SetDashFlag(bool const &bDashFlag) { m_bDashFlag = bDashFlag; };
	// ダッシュしているかどうかのフラグの取得
	bool		&GetDashFlag(void) { return m_bDashFlag; };
protected:
private:
	/* 構造体 */
	/* 関数 */
	// 自キャラ移動処理
	void MyMove(void);
	// 自キャラ行動処理
	void MyAction(void);
	// 状態によってのモーション設定処理
	void StatusMotion(void);
	// 足場に乗っている判定
	void StandJudg(
		CScene_X * pScene_X,
		bool const & bJudg
	);
	// ブロックの押し出し処理
	void PushBlock(
		CScene_X * pScene_X,			// シーンX情報
		CBaseblock::GRID const & Grid	// 方向
	);
	// 当たり判定処理
	void Collision(void);
	// 足場ブロックとの判定
	void StandCollision(void);
	// ブロックとの当たり判定
	void BlockCollision(void);
	// キャラクター同士の当たり判定
	void CharacterCollision(void);
	// 押し出した後のプレイヤーの当たり判定
	void PushAfter_Collision(
		CBaseblock * pBaseBlock,
		CBaseblock::GRID Grid
		);
	// プレイヤーのステータスのロード
	static void PlayerStatusLoad(void);
	// プレイヤーのステータスのセーブ
	void PlayerStatusSave(void);
	// プレイヤーのステータスの初期値のロード
	void PlayerStatusInitLoad(void);

	/* 変数 */
	CXInputPad				*m_pPad;						// パッドのポインタ
	bool					m_bDieFlag;						// 死亡フラグ
	bool					m_bDashFlag;					// ダッシュフラグ
	int						m_nCntDashTime;					// ダッシュ中の切り替えカウント
	static PLAYER_STATUS	m_PlayerStatus;					// プレイヤーのステータス
	PLAYER_STATUS			m_PlayerStatusInit;				// プレイヤーの初期ステータス

#ifdef _DEBUG
	CMeshBox * pCollisionBox[COLLISIONTYPE_MAX];
#endif // _DEBUG

};
#endif