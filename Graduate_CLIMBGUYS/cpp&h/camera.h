// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.h]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCamera
{
public:
	/* 列挙型 */
	// タイプ
	typedef enum
	{
		TYPE_TITLE = 0,// タイトル
		TYPE_GAME,	   // ゲーム
		TYPE_RESULT,   // リザルト
		TYPE_MAX	   // 最大数
	} TYPE;
	/* 構造体 */
	// 読み込み用
	typedef struct
	{
		D3DXVECTOR3 offset;		// 注視点のオフセット
		float		fTrunSpeed;	// 回転スピード
		float		fLengh;		// 注視点と視点の長さ
		float		fHeight;	// 注視点と視点の高さ
	} LOAD;
	// 振動情報
	struct SHAKE
	{
		int			nInterbalCnt;			// 振動する回数
		int			nInterbalCntSave;		// 振動する回数をカウント
		float		fDistance;				// 距離
		float		fRange[2];				// 距離の範囲_最小最大
		float		fShakeSpeed;			// 振動する速さ
		D3DXVECTOR3 CameraCurrentPos;		// カメラの現在地_振動前の座標
		bool		bEnd;					// 終了フラグ

		//float fAngle;						// 角度
	};

	/* 関数 */
	// コンストラクタ
	CCamera();
	// デストラクタ
	~CCamera();
	// 初期化処理
	void Init(void);
	// 終了処理
	void Uninit(void);
	// 更新処理
	void Update(void);
	// 描画処理
	void Draw(void);
#ifdef _DEBUG
	// デバッグ処理
	void Debug(void);
#endif // _DEBUG
	// 生成処理
	static CCamera * Create(void);	// 作成
	// 読み込み
	static HRESULT Load(void);
	// 破棄
	static void Unload(void);
	// 書き込み処理
	static void Save(void);
	// 各モードのカメラの初期設定
	void ModeCameraInit(void);
	// カメラの情報初期化
	void InfoInit(
		D3DXVECTOR3 const &pos,	// 位置情報
		D3DXVECTOR3 const &rot	// 回転情報
		);
	// カメラ情報の設定取得
	bool GetCamera_infoSet(void)  const { return m_bSet; };
	// カメラ設定
	void Set(void);
	// 目的注視点の設定
	void SetPosDestR(
		D3DXVECTOR3 const & pos,	// 位置情報
		D3DXVECTOR3 const & rot		// 回転情報
	);
	// 注視点の設定
	void SetPosR(
		D3DXVECTOR3 const & pos,	// 位置情報
		D3DXVECTOR3 const & rot		// 回転情報
	);
	// カメラ情報設定の使用状態設定
	void SetCamera_InfoSet(bool const &bSet) { m_bSet = bSet; };
	// カメラタイプの設定
	void SetType(
		TYPE type
	);
	// 回転情報取得
	D3DXVECTOR3 GetRot(void);
	// 回転スピード取得
	D3DXVECTOR2 &GetRotSpeed(void);
	// カメラ設定すべての初期化
	void SetCameraInfo(void);
	// カメラモーション設定
	//	posR		: 位置R
	//	rot			: 回転
	//	fLength		: 長さ
	//	fHeight		: 高さ
	//	fIntertia	: 慣性
	void SetCamera_Motion(
		D3DXVECTOR3 const & posR,	// 位置R
		D3DXVECTOR3 const & rot,	// 回転
		float const & fLength,		// 長さ
		float const & fHeight,		// 高さ
		float const & fIntertia		// 慣性
	);
	// カメラの視点取得
	D3DXVECTOR3 &GetPosV(void) { return m_posV; };
	//
	D3DXVECTOR3 &GetPosR(void) { return m_posR; };

	// カメラの振動
	void CameraShake();

	// カメラの振動の情報設定 modeのカメラの初期化でする
	void SetShakeInfo();

	// カメラの振動が終了した際のフラグ取得bEnd
	bool GetShakeEndFlag() { return m_Shake.bEnd; };

protected:

private:
	/* 関数 */
	void Update_Play(void);						// プレイ時のカメラ
	// オープニング初期化処理
	void Init_Opening(void);
	// チュートリアル初期化処理
	void Init_Tutorial(void);
	// タイトル初期化処理
	void Init_Title(void);
	// ゲーム初期化処理
	void Init_Game(void);
	// リザルト初期化処理
	void Init_Result(void);
	// チュートリアル更新処理
	void Update_Tutorial(void);
	// タイトル更新処理
	void Update_Title(void);
	// ゲーム更新処理
	void Update_Game(void);


	// 初期時のカメラ設定
	void InitCamera(void);
	/* 変数 */
	static	LOAD	m_load;						// 情報保存
	D3DXVECTOR3		m_posV;						// 視点
	D3DXVECTOR3		m_posVDest;					// 目的視点
	D3DXVECTOR3		m_posVDiff;					// 目的視点から視点
	D3DXVECTOR3		m_posR;						// 注視点
	D3DXVECTOR3		m_posRDest;					// 目標注視点
	D3DXVECTOR3		m_posRDiff;					// 目的注視点から注視点
	D3DXVECTOR3		m_offset;					// 注視点のオフセット
	D3DXVECTOR3		m_posU;						// 上方向ベクトル
	D3DXVECTOR3		m_between;					// 基準との距離
	D3DXVECTOR3		m_move;						// 移動量
	D3DXVECTOR3		m_rot;						// 視点の回転量
	D3DXVECTOR3		m_rotDest;					// 視点の目的回転地点
	D3DXVECTOR3		m_rotDiff;					// 目的回転地点から現在回転地点
	D3DXVECTOR3		m_rotBetween;				// 目的回転地点と現在回転地点の差分
	D3DXVECTOR3		m_rotOrigin;				// 元の回転量
	D3DXMATRIX		m_mtxProjection;			// プロジェクションマトリックス(テレビ視点)
	D3DXMATRIX		m_mtxView;					// ビューマトリックス(カメラマン視点)
	D3DXVECTOR2		m_RotSpeed;					// 回転スピード
	TYPE			m_type;						// タイプ[
	float			m_fHeight;					// 注視点からの視点の高さ
	float			m_fLength;					// 視点と注視点の距離
	float			m_fIntertia;				// 慣性の比率
	bool			m_bSet;						// カメラ設定
	SHAKE			m_Shake;					// 振動情報
};

#endif
