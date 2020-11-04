// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.h]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

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
		D3DXVECTOR3 rot;	// 回転量
		D3DXVECTOR3 offset;	// 注視点のオフセット
		float fLengh;		// 注視点と視点の長さ
		float fHeight;		// 注視点と視点の高さ
		int nType;			// タイプ
	} LOAD;
	// 360度回転
	typedef struct TURN
	{
		int		nSpin;					// 回る回数
		int		nCntSpin;				// 回った回数
		int		nOneTime;				// 一周回転にかかる時間
		int		nCntTime;				// タイムカウント
		float	fTrunRot;				// 回転量
		bool	bSpin;					// 回転状態
	} TURN;
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
protected:

private:
	/* 関数 */
	void Update_Play(void);						// プレイ時のカメラ
	// チュートリアル初期化処理
	void Init_Tutorial(void);
	// タイトル初期化処理
	void Init_Title(void);
	// チュートリアル更新処理
	void Update_Tutorial(void);
	// タイトル更新処理
	void Update_Title(void);

	// 初期時のカメラ設定
	void InitCamera(void);
	/* 変数 */
	static	LOAD	m_load[TYPE_MAX];			// 情報保存
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
	TURN			m_Turn;						// 360°回転
};

#endif
