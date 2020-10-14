// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2Dゲージ処理の説明[2Dgauge.h]
// Author : Nishiyama Koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _2DGAUGE_H_
#define _2DGAUGE_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "character.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 前方宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;	// 2Dシーンクラス

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C2DGauge : public CScene
{
public:
	/* 関数 */
	// コンストラクタ
	C2DGauge();
	// デストラクタ
	~C2DGauge();
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
	// 自分から当たった後の処理
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
	virtual D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	// ポインター過去の位置情報を取得
	virtual D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	virtual D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

	// 全リソース情報の読み込み
	static HRESULT Load(void);
	// 全リソース情報の解放
	static void UnLoad(void);
	// 生成(シーン管理)
	//	pos			: 位置
	//	size		: サイズ
	//	Maincol		: メインカラー
	//	Subcol		: サブカラー
	//	BGcol		: 背景色
	static C2DGauge * Create(
		D3DXVECTOR3 const &pos,												// 位置
		D3DXVECTOR2 const &size,											// サイズ
		D3DXCOLOR	const &Maincol,											// メインカラー
		CCharacter::CHARACTER	const &type,											// キャラクタータイプ
		D3DXCOLOR	const &Subcol = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),			// サブカラー
		D3DXCOLOR	const &BGcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)		// 背景色
		);
	// 生成(個人管理)
	//	pos			: 位置
	//	size		: サイズ
	//	Maincol		: メインカラー
	//	Subcol		: サブカラー
	//	Blockcol	: 背景色
	static C2DGauge * Create_Self(
		D3DXVECTOR3 const &pos,												// 位置
		D3DXVECTOR2 const &size,											// サイズ
		D3DXCOLOR	const &Maincol,											// メインカラー
		D3DXCOLOR	const &Subcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),		// サブカラー
		D3DXCOLOR	const &BGcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)		// 背景色
	);
	// 設定 //
	// ゲージの長さの固定値設定
	//	fMaxValue : ステータスの最大値
	void SetConstance(
		float const &fMaxValue	// ステータスの最大値
	);
	// メインカラー設定
	//	MainCol_R	: 右のメインカラー
	//	MainCol_L	: 左のメインカラー
	void SetMainCol(
		D3DXCOLOR const &MainCol_R,	// 右の色
		D3DXCOLOR const &MainCol_L	// 左の色
	);
	// メイン、サブ、背景のカラー設定
	//	Maincol	: メインカラー
	//	Subcp;	: サブカラー
	//	BGcol	: 背景色
	void SetCol(
		D3DXCOLOR	const &Maincol,										// メインカラー
		D3DXCOLOR	const &Subcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),	// サブカラー
		D3DXCOLOR	const &BGcol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// 背景色
	);
	// 位置設定
	//	pos	: 位置
	void SetPos(
		D3DXVECTOR3 const &pos	// 位置
	);
	// 取得 //
	// 位置取得
	D3DXVECTOR3 & GetPos(void);
	// ゲージ変化
	//	fValue	: ステータスの値
	void ChangeGauge(
		float const &fValue	// ステータスの値
	);
	// ゲージの初期設定
	//	fValue	: ステータスの値
	void BeginGauge(
		float const &fValue	// ステータスの値
	);
protected:

private:
	/* 列挙型 */
	// ゲージの種類
	typedef enum
	{
		GAUGE_BLOCK = 0,	// 背景枠
		GAUGE_UNDERLINE,	// サブ枠
		GAUGE_MAINLINE,		// メイン枠
		GAUGE_DESIGN,		// ゲージデザイン枠
		GAUGE_MAX
	} GAUGE;
	/* 関数 */

	/* 変数 */
	D3DXVECTOR2	m_size[GAUGE_MAX];							// サイズ情報
	float		m_fSizeLast;								// サイズの目標値
	float		m_fSizeDiff;								// サイズの差分
	float		m_fConstance;								// サイズが変わる定数
	D3DXCOLOR	m_col[GAUGE_MAX];							// カラー情報
	D3DXVECTOR3	m_pos;										// 位置情報
	CScene_TWO *m_aScene_Two[GAUGE_MAX];					// 2Dゲージ2D
	CCharacter::CHARACTER m_type;							// キャラクタータイプ
	static int	m_TexType[CCharacter::CHARACTER_MAX];	// テクスチャータイプ
};
#endif