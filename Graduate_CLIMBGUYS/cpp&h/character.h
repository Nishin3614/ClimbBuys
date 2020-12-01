// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// キャラクター処理 [character.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CMeshobit;
class CCollision;
class CStencilshadow;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCharacter : public CScene
{
public:
	/* 列挙型 */
	// ステータス
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_DIE,
		STATE_MAX
	} STATE;
	// 基本モーションタイプ
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		// 待機モーション
		MOTIONTYPE_MOVE,			// 移動モーション
		MOTIONTYPE_DASH,			// ダッシュモーション
		MOTIONTYPE_JUMP,			// ジャンプモーション
		MOTIONTYPE_BANZAI,			// バンザイモーション
		MOTIONTYPE_VICTORY_01,		// 勝利モーション_01
		MOTIONTYPE_VICTORY_02,		// 勝利モーション_02
		MOTIONTYPE_VICTORY_03,		// 勝利モーション_03
		MOTIONTYPE_VICTORY_04,		// 勝利モーション_04
		MOTIONTYPE_MAX				// 最大数
	} MOTIONTYPE;
	// キャラクター
	typedef enum
	{
		CHARACTER_PLAYER_0 = 0,	// プレイヤー0
		CHARACTER_PLAYER_1,		// プレイヤー1
		CHARACTER_PLAYER_2,		// プレイヤー2
		CHARACTER_PLAYER_3,		// プレイヤー3
		CHARACTER_MAX,			// キャラクター全体数
	} CHARACTER;

	/* 構造体 */

	/* 関数 */
	// コンストラクタ
	CCharacter(CHARACTER const &character);
	// デストラクタ
	virtual ~CCharacter();
	// 初期化処理
	virtual void Init(void);
	// 更新処理
	virtual void Uninit(void);
	// 更新処理
	virtual void Update(void);
	// 描画処理
	virtual void Draw(void);
	// キャラクターが死んだとき
	virtual void Die(void);
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
	) = 0;
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) = 0;
	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return &m_posold; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return &m_move; };
	// 押し出し当たり判定
	//	Obj		: オブジェタイプ
	//	pos		: 位置
	//	posOld	: 前回の位置
	//	move	: 移動量
	//	size	: サイズ
	virtual COLLISIONDIRECTION PushCollision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * move,								// 移動量
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	) = 0;
	// 当たり判定
	//	Obj		: オブジェタイプ
	//	pos			: 位置
	//	size		: サイズ
	//	OffsetPos	: オフセット位置
	virtual COLLISIONDIRECTION Collision(
		CScene::OBJ const & Obj,						// オブジェタイプ
		D3DXVECTOR3 * pos,								// 位置
		D3DXVECTOR3 * posOld,							// 前回の位置
		D3DXVECTOR3 * size,								// サイズ
		D3DXVECTOR3 const & OffsetPos = D3DVECTOR3_ZERO	// オフセット位置
	) = 0;
	// 必要に応じた動作 //
	// 重力
	void AddGravity(float const &fGravity)					{ m_move.y -= fGravity; };
	// 設定 //
	// 位置
	void SetPos(D3DXVECTOR3 const &pos)						{ m_pos = pos; };
	// 前回の位置
	void SetPosOld(D3DXVECTOR3 const &Posold)				{ m_posold = Posold; };
	// 移動量
	void SetMove(D3DXVECTOR3 const &move)					{ m_move = move; };
	// 回転
	void SetRot(D3DXVECTOR3 const &rot)						{ m_rot = rot; };
	// ジャンプできるかどうかのフラグの設定
	void SetJumpAble(bool const &jump)						{ m_bJumpable = jump; };
	//  死亡しているかどうかのフラグの設定
	void SetDie(bool const &die)							{ m_bDie = die; };

	// 方向ベクトル設定
	void SetDirectionVec(D3DXVECTOR3 const &direct)			{ m_Directvector = direct; };
	// 取得 //
	// 位置
	D3DXVECTOR3 &GetPos(void)								{ return m_pos; };
	// 前回の位置取得
	D3DXVECTOR3 &GetPosOld(void)							{ return m_posold; };
	// 移動量
	D3DXVECTOR3 &GetMove(void)								{ return m_move; };
	// 回転
	D3DXVECTOR3 &GetRot(void)								{ return m_rot; };
	// ジャンプできるかどうかのフラグの取得
	bool		&GetJumpAble(void)							{ return m_bJumpable; };
	// 死亡しているかどうかのフラグの取得
	bool		&GetDie(void)								{ return m_bDie; };
	// 親と子の回転量
	D3DXVECTOR3 *GetPartsRot(int const nModelID);
	// 親と子の位置
	D3DXVECTOR3 * GetPartsPos(int const nModelID);
	// 親と子の行列
	D3DXMATRIX *GetMatrix(int const nModelID);
	// 状態
	STATE GetState(void) const								{ return m_State; };
	// キャラクター
	CHARACTER	GetCharacter(void) const					{ return m_character; };
	// 前方方向ベクトル取得
	D3DXVECTOR3 GetDirectionVec(void)						{ return m_Directvector; };
	// 床の高さ
	bool GetFloorHeight(void);
	// モーションのフレーム情報取得処理
	int GetMaxFrame(
		CHARACTER character,					// キャラクター
		int nMotionID,							// モーションID
		int nNowKeyCnt = -1						// 現在のキーカウント
	);
	// 出現している人数取得
	int GetAllCharacter(void) { return m_nAllCharacter; };
	// キャラクター全ソースの読み込み
	static HRESULT Load(void);
	// キャラクターの情報読み込み
	static HRESULT Load_Character(void);
	// キャラクター全ソースの開放
	static void UnLoad(void);
	// キャラクターの静的変数の初期化
	static void InitStatic(void);
	// 目標回転量設定
	void SetRotDest(D3DXVECTOR3 const &rotDest)		{ m_rotLast = rotDest; };
	// 目標回転量取得
	D3DXVECTOR3 GetRotDest(void) const				{ return m_rotLast; };
#ifdef _DEBUG
	virtual void  Debug(void);
	static void AllDebug(void);
#endif // _DEBUG
protected:
	/* 関数 */
	// 設定 //
	// キャラクター
	void SetCharacter(CHARACTER const character)	{ m_character = character; };
	// モーション設定
	void SetMotion(int const nMotiontype);
	// 強制モーション設定
	void ComplusionSetMotion(int const nMotiontype);
	// 重力
	void FagGravity(void);
	// 透明度の設定
	static void SetAlpha(float fAlpha)				{ m_fAlpha = fAlpha; }
	// 設定 //
	// 現在のモーション
	int GetMotion(void) const						{ return m_nMotiontype; };
	// 過去のモーション
	int GetOldMotion(void) const					{ return m_nMotiontypeOld; };
	// キーカウント
	int GetKeyInfoCnt(void) const					{ return m_keyinfoCnt; };
	// モーションカメラの更新
	void MotionCamera(void);
	/* 変数 */
	D3DXVECTOR3									m_pos;							// 位置
	D3DXVECTOR3									m_move;							// 移動量
	D3DXVECTOR3									m_rot;							// 現在回転量
	// 仮
	STATE										m_State;						// 現状のステータス
	int											m_nCntState;					// カウントステータス



	/* プロトタイプ用 */
	void Limit(void);										// 制限区域
private:
	/* 関数 */
	void Update_Normal(void);								// 通常時の更新
	void NextKeyMotion(void);								// 次のモーション
	void Move(void);										// 移動
	void Motion(void);										// モーション
	void ModelUpdate(void);									// モデルの更新
	void Motion_Effect(void);								// モーションエフェクト
	void Motion_Obit(void);									// モーション軌跡
	/* 変数 */
	/* 構造体のスタティックにする */
	static MODEL_ALL							*m_modelAll[CHARACTER_MAX];		// モデル全体の情報
	static std::vector<int>						m_modelId[CHARACTER_MAX];		// モデル番号
	static CModel_info							*m_model_info[CHARACTER_MAX];	// モデル情報
	static D3DXVECTOR3							m_CharacterSize[CHARACTER_MAX];	// キャラクターのサイズ
	static int									m_NumModel[CHARACTER_MAX];		// 最大モデル数
	static int									m_NumParts[CHARACTER_MAX];		// 動かすモデル数
	static int									m_nAllCharacter;				// 出現しているキャラクター人数
	static float								m_fAlpha;						// アルファ値
	CMeshobit									*m_pMeshobit;					// 軌跡
	CModel 										*m_pModel;						// モデル
	CHARACTER									m_character;					// キャラクター
	D3DXMATRIX									m_mtxWorld;						// 行列
	D3DXVECTOR3									m_posold;						// 前の位置
	D3DXVECTOR3									m_rotLast;						// 向きたい方向
	D3DXVECTOR3									m_rotbetween;					// 回転の差分
	int											m_nMotiontype;					// モーションタイプ
	int											m_nMotiontypeOld;				// 前回のモーションタイプ
	int											m_nMaxMotion;					// 最大モーション数
	int											m_keyinfoCnt;					// キー情報のカウント
	int											m_nFrame;						// フレームカウント
	int											m_nMotionFrame;					// 一つのモーションのカウント
	float										m_fLength;						// 攻撃の当たり範囲
	bool										m_bJumpable;					// ジャンプ可能かどうか
	bool										m_bDie;							// 死亡フラグ
	D3DXVECTOR3									m_Directvector;					// 方向のベクトル
	std::vector<std::unique_ptr<CMeshobit>>		m_vec_pMeshObit;				// 奇跡
	CStencilshadow								* m_pStencilshadow;				// ステンシルシャドウ
};

#endif