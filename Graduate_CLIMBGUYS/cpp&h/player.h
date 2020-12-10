// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル処理 [player.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
class CMeshBox;		// メッシュボックスクラス
class CPlayerUI;	// プレイヤーUI

class C3DLine;	// 3D線クラス

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
	// ステータスタイプ
	typedef enum
	{
		STATUSTYPE_BLOCK,		// ブロックに押されたとき
		STATUSTYPE_JUMP,		// 踏まれたとき
		STATUSTYPE_ELECTRIC,	// 電気を食らったとき
		STATUSTYPE_MAX
	} STATUSTYPE;

	// ----- プレイヤーのステータス ----- //
	typedef struct _PLAYER_STATUS
	{
		_PLAYER_STATUS()
		{
			fMove				= 0;				// 移動量
			fJump				= 0;				// ジャンプ力
			fNormalInertia		= 0;				// 通常時の慣性
			fJumpInertia		= 0;				// ジャンプ時の慣性
			PlayerSize			= D3DVECTOR3_ZERO;	// プレイヤーサイズ
			PlayerOffSet		= D3DVECTOR3_ZERO;	// プレイヤーオフセット
			PushSize			= 0;				// 押し出し用のサイズ
			PushOffSet			= D3DVECTOR3_ZERO;	// 押し出し用のオフセット
			nMaxPowerTime		= 0;				// 最大パワータイム
			nMaxPanicTime		= 0;				// 最大パニックタイム
			for (int nCnt = 0; nCnt < STATUSTYPE_MAX; nCnt++)
			{
				nMaxStanTime[nCnt]		 = 0;		// 最大スタンタイム
				nMaxInvincibleTime[nCnt] = 0;		// 最大無敵タイム
			}
		}
		float				fMove;								// 移動量
		float				fJump;								// ジャンプ力
		float				fNormalInertia;						// 通常時の慣性
		float				fJumpInertia;						// ジャンプ時の慣性
		D3DXVECTOR3			PlayerSize;							// プレイヤーサイズ
		D3DXVECTOR3			PlayerOffSet;						// プレイヤーオフセット
		float				PushSize;							// 押し出し用のサイズ
		D3DXVECTOR3			PushOffSet;							// 押し出し用のオフセット
		int					nMaxPowerTime;						// 最大パワータイム
		int					nMaxStanTime[STATUSTYPE_MAX];		// 最大スタンタイム
		int					nMaxInvincibleTime[STATUSTYPE_MAX];	// 最大無敵タイム
		int					nMaxPanicTime;						// 最大パニックタイム
	}PLAYER_STATUS;
	// ----- 力溜め状態 ----- //
	typedef struct _POWER
	{
		// コンストラクタ
		_POWER()
		{
			bCharge = false;		// チャージ状態
			bDashFlag = false;		// ダッシュフラグ
			bTackleFrag = false;	// タックルフラグ
			nCntDashTime = 0;		// ダッシュ中の切り替えカウント
			nPushPower = 1;			// 押す力
			nCntTime = 0;			// 力溜めカウント
		}
		// 初期化処理
		void Init()
		{
			bCharge = false;		// チャージ状態
			bDashFlag = false;		// ダッシュフラグ
			bTackleFrag = false;	// タックルフラグ
			nCntDashTime = 0;		// ダッシュ中の切り替えカウント
			nPushPower = 1;			// 押す力
			nCntTime = 0;			// 力溜めカウント
		}
		bool		bCharge;		// チャージ状態
		bool		bDashFlag;		// ダッシュフラグ
		bool		bTackleFrag;	// タックルフラグ
		int			nCntDashTime;	// ダッシュ中の切り替えカウント
		int			nPushPower;		// 押す力
		int			nCntTime;		// 力溜めカウント
	} POWER;

	typedef struct _STATUSCHANGE
	{
		// コンストラクタ
		_STATUSCHANGE()
		{
			bChange = false;  // ステータス変動状態
			nChangeTime = 0;	// ステータス変動継続時間
			nMaxTime = 0;		// 最大継続時間
		}
		// 初期化処理
		void Init()
		{
			bChange = false;	// ステータス変動状態
			nChangeTime = 0;	// ステータス変動継続時間
			nMaxTime = 0;		// 最大継続時間
		}
		// ステータス変動設定
		void Set(bool const & bSouceChange,int const & nSouceMaxTime)
		{
			bChange = bSouceChange;			// ステータス変動状態
			nMaxTime = nSouceMaxTime;		// 最大継続時間
		}
		bool		bChange;			// ステータス変動状態
		int			nChangeTime;		// ステータス変動継続時間
		int			nMaxTime;			// 最大継続時間
	} STATUSCHANGE;
	// 共用体
	// 死亡状態
	union DIESTATUS
	{
		DIESTATUS() {}
		// 初期化処理
		void Init(void)
		{
			for (int nStatus = 0; nStatus < 6; nStatus++)
			{
				bStatus[nStatus] = false;
			}
		}
		struct
		{
			bool	bUp;		// 上の当たり判定
			bool	bDown;		// 下の当たり判定
			bool	bRight;		// 右の当たり判定
			bool	bLeft;		// 左の当たり判定
			bool	bFront;		// 前の当たり判定
			bool	bBack;		// 後の当たり判定
		};
		bool	bStatus[6];
	};

	// 列挙
	// 死んだ原因
	typedef enum
	{
		LIVE = 0,			// 生きている
		DIECAUSE_PRESS,		// 圧死
		DIECAUSE_FALL,		// 落下死
		DIECAUSEMAX			// 最大数
	} DIECAUSE;
	// ----- 記録 ----- //
	typedef struct _RECORD
	{
		// コンストラクタ
		_RECORD()
		{
			nRanking = 1;  					// ランキング
			nTime = -1;						// タイム
			nPushCnt = 0;					// 押した数
			DieCause = DIECAUSE::LIVE;		// 死んだ原因
		}
		int			nRanking;		// ランキング
		int			nTime;			// タイム
		int			nPushCnt;		// 押した数
		DIECAUSE	DieCause;		// 死んだ原因
	} RECORD;
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
#if IMGUI_DEBUG

	static void AllDebugImGui(void);

#endif // IMGUI_DEBUG

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
	void SetDashFlag(bool const &bDashFlag) { m_Power.bDashFlag = bDashFlag; };
	// ダッシュしているかどうかのフラグの取得
	bool		&GetDashFlag(void)			{ return m_Power.bDashFlag; };
	// 記録情報の取得
	RECORD & GetRecord(void)				{ return m_Record; };
	// バネ用ジャンプ処理
	void SpringJump(void);

	// ゲームパッドの取得
	CXInputPad *GetPad()					{ return m_pPad; };

	static void InitDieCount() { m_nDieCnt = 0; };
	static int GetDieCount() { return m_nDieCnt; };
	// プレイヤーのステータス情報取得
	static PLAYER_STATUS GetPlayerStatus(void) { return m_PlayerStatus; };
	// プレイヤーの死亡状態の取得
	DIESTATUS &GetPlayerDieStatus(void) { return m_DieStatus; };
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
	// ブロックの押し出し処理
	void PushBlock(
		CBaseblock * pBlock,			// ブロック情報
		CBaseblock::GRID const & Grid	// 方向
	);
	// 当たり判定処理
	void Collision(void);
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
	static void PlayerStatusSave(void);
	// プレイヤーのステータスの初期値のロード
	static void PlayerStatusInitLoad(void);

	// プレイヤーの移動方向設定
	void PlayerMoveSet(
		D3DXVECTOR3 & Vec,
		D3DXVECTOR3 const & Rot,
		D3DXVECTOR3 & Move
	);

	// プレイヤーの状態更新
	void StateUpdate(void);
	// 力溜めの更新
	void PowerUpdate(void);
	// スタン状態の更新
	void StanUpdate(void);
	// 無敵状態の更新
	void InvincibleUpdate(void);
	// パニック状態の更新
	void PanicUpdate(void);
	// 電気ブロック使用時
	void ElectricUse(void);

	/* 変数 */
	CXInputPad					*m_pPad;						// パッドのポインタ
	bool						m_bDieFlag;						// 死亡フラグ
	static PLAYER_STATUS		m_PlayerStatus;					// プレイヤーのステータス
	static PLAYER_STATUS		m_PlayerStatusInit;				// プレイヤーの初期ステータス
	POWER						m_Power;						// 力溜め
	STATUSCHANGE				m_Stan;							// スタン状態
	STATUSCHANGE				m_Invincible;					// 無敵状態
	STATUSCHANGE				m_Panic;						// パニック状態
	CPlayerUI					*m_pPlayerUI;					// プレイヤーUI
	bool						m_bSpringFlag;					// ばねの判定を一回だけ通す
	RECORD						m_Record;						// 記録情報

	static int					m_nDieCnt;						// 死亡人数 仮
	DIESTATUS					m_DieStatus;					// 死亡状態

#ifdef _DEBUG
	CMeshBox * pCollisionBox;
	C3DLine *	pCollisionLine;
#endif // _DEBUG

};
#endif