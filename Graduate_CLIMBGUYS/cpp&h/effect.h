// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// エフェクトの処理[effect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Calculation.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EFFECT_MAX			(5000)				// 最大数
#define EFFECT_NUM_VERTEX	(4)					// 頂点数
#define BASEANGLE			(D3DX_PI * 0.25f)	// ベース角度

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CRenderer;	// レンダラークラス

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CEffect :public CScene
{
public:
	/* 列挙型 */
	// エフェクトタイプ
	typedef enum
	{
		EFFECT_TYPE_NONE = -1,		// なし
		EFFECT_TYPE_MAX				// 最大数
	} EFFECT_TYPE;

	/* 構造体 */
	// エフェクトアニメーション
	typedef struct _EFFECTANIM : public ANIMATION
	{
		_EFFECTANIM() : ANIMATION()
		{
			FirstPos = D3DVECTOR2_ZERO;		// 初期位置
			EndPos = D3DVECTOR2_ZERO;		// 末期位置
			bUse = false;					// アニメーション使用状態
			bTexUpdate = false;				// テクスチャー更新状態
		}
		// アニメーション読み込み代入
		_EFFECTANIM& operator = (ANIMATION_LOAD const & AnimLoad)
		{
			ANIMATION * pAnim = (ANIMATION *)this;
			*pAnim = AnimLoad;
			bUse = true;
			return *this;
		}
		// 初期化処理
		void Init(void)
		{
			ANIMATION::Init();				// アニメーションの初期化
			FirstPos = D3DVECTOR2_ZERO;		// 初期位置
			EndPos = D3DVECTOR2_ZERO;		// 末期位置
			bUse = false;					// アニメーション使用状態
			bTexUpdate = false;				// テクスチャー更新状態
		}

		D3DXVECTOR2			FirstPos;			// 初期位置
		D3DXVECTOR2			EndPos;				// 末期位置
		bool				bUse;				// アニメーション使用状態
		bool				bTexUpdate;			// テクスチャー更新状態
	} EFFECTANIM;
	// エフェクトプリミティブ
	typedef struct
	{
		D3DXVECTOR3			pos;				// 位置
		D3DXVECTOR3			rot;				// 回転量
		D3DXVECTOR3			move;				// 移動量
		D3DXCOLOR			col;				// 色
		D3DXVECTOR2			size;				// サイズ
		D3DXVECTOR2			sizeValue;			// サイズの変化値
		float				fAngle;				// 角度
		float				fAlphaValue;		// アルファ値の変化値
		int					nLife;				// 持ち時間
		int					nTexType;			// テクスチャの種類
		bool				bUse;				// 使用しているかどうか
		EFFECTANIM			Animation;			// アニメーション情報
		EFFECT_TYPE			EffectType;			// エフェクトの種類
		CRenderer::BLEND	BlendType;			// ブレンドタイプ
	} EFFECT;

	/* 関数 */
	// コンストラクタ
	CEffect();
	// デストラクタ
	virtual ~CEffect();
	// 全リソース情報の読み込み処理
	static HRESULT Load(void);
	// 全リソース情報の開放
	static void Unload(void);
	// 初期化処理
	virtual void Init(void);
	// 終了処理
	virtual void Uninit(void) = 0;
	// 更新処理
	virtual void Update(void) = 0;
	// 描画処理
	virtual void Draw(void) = 0;
#ifdef _DEBUG
	// デバッグ処理
	virtual void Debug(void);
#endif // _DEBUG
	// 自分から当たった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_MyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられた後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_OpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 自分から当たらなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};
	// 相手に当てられなかった後の処理
	//	nObjType	: オブジェクトタイプ
	//	pScene		: 相手のシーン情報
	void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// オブジェクトタイプ
		CScene * pScene = NULL		// 相手のシーン情報
	) {};


	// ポインター位置情報を取得
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// ポインター過去の位置情報を取得
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// ポインター移動量情報の取得
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// テクスチャーアニメーションの設定
	//	nMaxCntAnim			: 最大アニメーションカウント
	//	nMaxHorizonAnim		: 最大水平アニメーションカウント
	//	nMaxVirticalAnim	: 最大垂直アニメーションカウント
	//	bLoop				: ループ状態
	void SetTexAnim(
		EFFECT * pEffect,				// エフェクト情報
		int const & nMaxCntAnim,		// カウントアニメ最大数
		int const & nMaxHorizonAnim,	// 水平アニメーション最大数
		int const & nMaxVirticalAnim	// 垂直アニメーション最大数
	);

	// 値の初期化処理
	//	pEffect		: エフェクト情報
	void InitValues(
		EFFECT *pEffect	// エフェクト情報
	);
	// 値の初期化処理
	//	pEffect		: エフェクト情報
	//	nCntEffect	: エフェクトカウント
	static void Init_OneValues(
		EFFECT * pEffect		// エフェクト情報
	);

protected:
	/* 関数 */
	// ライフ更新処理
	//	pEffect	: エフェクト情報
	bool UpdateLife(
		EFFECT *pEffect		// エフェクト情報
	);
	// ライフ更新処理
	//	pVtx	: 2D頂点情報
	//	pEffect	: エフェクト情報
	bool UpdateVetex(
		EFFECT *pEffect		// エフェクト情報
	);
	// ライフ更新処理
	//	pVtx	: 2D頂点情報
	//	pEffect	: エフェクト情報
	void UpdateMove(
		EFFECT *pEffect		// エフェクト情報
	);
	// アニメーション更新処理
	//	pEffect	: エフェクト情報
	void Updata_Animation(
		EFFECT *pEffect		// エフェクト情報
	);
private:
};

#endif