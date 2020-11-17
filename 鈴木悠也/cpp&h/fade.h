// ----------------------------------------
//
// 画面遷移処理の説明[fade.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FADE_H_
#define _FADE_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "2Dpresents.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_FADE (10)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CFade : C2DPresents
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// 何もない状態
		FADE_IN,			// フェードイン処理
		FADE_OUT,			// フェードアウト処理
		FADE_MAX
	} FADE;

	/* 関数 */
	CFade();
	~CFade();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 当たった後の処理
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
	void SetFade(CManager::MODE const &Mode);			// 画面遷移設定
	FADE GetFade(void);									// 画面遷移情報取得
	static CFade * Create(CManager::MODE const &mode);	// 作成
	static HRESULT Load(void);
	static void UnLoad(void);
protected:

private:
	FADE					m_fade;		// フェード状態
	CManager::MODE			m_modeNext;	// 次の画面（モード）
};

#endif