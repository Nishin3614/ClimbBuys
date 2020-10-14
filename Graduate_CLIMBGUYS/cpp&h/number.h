// ----------------------------------------
//
// 番号処理の説明[number.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _NUMBER_H_
#define _NUMBER_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_two.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CNumber : public CScene_TWO
{
public:
	/* 列挙型 */
	// テクスチャータイプ
	typedef enum
	{
		TEX_TIME,
		TEX_SCORE,
		TEX_KNOCK,
		TEX_MAX
	} TEX;

	/* 関数 */
	CNumber();
	~CNumber();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void) {};
#endif // _DEBUG
	// 当たった後の処理
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
	void SetNum(int const &nNum);											// 番号設定
	void SetTex(TEX const &tex);											// テクスチャー番号設定
	void SetAnim(int const &nRandamTime) {
		m_nRandamTime = nRandamTime;
		m_bRand = true;
	};	// ランダム数字切り替え演出時間設定
	bool &GetAnim(void) { return m_bRand; };
	// 強制アニメーション終了
	void Complusion(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	// 作成
	static CNumber * Create(
		int			const & nScore,								// スコア
		D3DXVECTOR3 const & pos,								// 位置
		TEX			const & tex = TEX_SCORE,					// テクスチャータイプ
		D3DXVECTOR2 const & size = D3DXVECTOR2(100.0f,100.0f),	// サイズ
		D3DXCOLOR	const & col = D3DXCOLOR_INI					// 色
	);	// 作成
protected:

private:
	static int		m_TexId[TEX_MAX];	// テクスチャーID
	TEX				m_texID;			// テクスチャー番号
	int				m_nNum;				// 渡された数字
	int				m_nRandamTime;		// ランダムに数字が切り替わる演出時間
	int				m_nFram;			// フレームタイム
	bool			m_bRand;			// ランダム化
};

#endif