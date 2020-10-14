// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2Dプレゼンツ処理の説明[2Dpresents.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "2Dpresents.h"
#include "manager.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// イニシャライザコンストラクタ処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents::C2DPresents() : CScene_TWO::CScene_TWO()
{
	// 変数の初期化
	m_pCooperation = NULL;
	m_pFadeIn = NULL;
	m_pFadeOut = NULL;
	m_Fadetype = FADETYPE_COOPERATION;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents::~C2DPresents()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Init(void)
{
	CScene_TWO::Init();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Uninit(void)
{
	// シーン2D終了処理
	CScene_TWO::Uninit();
	// 強調情報の開放
	if (m_pCooperation != NULL)
	{
		// 点滅情報の開放
		if (m_pCooperation->pCol_flash != NULL)
		{
			delete m_pCooperation->pCol_flash;
			m_pCooperation->pCol_flash = NULL;
		}
		// 拡大率情報の開放
		if (m_pCooperation->pScal != NULL)
		{
			delete m_pCooperation->pScal;
			m_pCooperation->pScal = NULL;
		}
		delete m_pCooperation;
		m_pCooperation = NULL;
	}
	// フェードインの開放
	if (m_pFadeIn != NULL)
	{
		// フェードイン(指定色)の開放
		if(m_pFadeIn->pDesign_Col != NULL)
		{
			delete m_pFadeIn->pDesign_Col;
			m_pFadeIn->pDesign_Col = NULL;
		}
		// フェードイン(移動)の開放
		if (m_pFadeIn->pDesign_Move != NULL)
		{
			delete m_pFadeIn->pDesign_Move;
			m_pFadeIn->pDesign_Move = NULL;
		}
		delete m_pFadeIn;
		m_pFadeIn = NULL;
	}
	// フェードアウトの開放
	if (m_pFadeOut != NULL)
	{
		// フェードアウト(指定色)の開放
		if (m_pFadeOut->pDesign_Col != NULL)
		{
			delete m_pFadeOut->pDesign_Col;
			m_pFadeOut->pDesign_Col = NULL;
		}
		// フェードアウト(移動)の開放
		if (m_pFadeOut->pDesign_Move != NULL)
		{
			delete m_pFadeOut->pDesign_Move;
			m_pFadeOut->pDesign_Move = NULL;
		}
		delete m_pFadeOut;
		m_pFadeOut = NULL;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update(void)
{
	switch (m_Fadetype)
	{
	case FADETYPE_IN:
		Update_FadeIn();
		break;
	case FADETYPE_COOPERATION:
		Update_Cooperation();
		break;
	case FADETYPE_OUT:
		Update_FadeOut();
		break;
	default:
		break;
	}
	// シーン2D更新
	CScene_TWO::Update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Draw(void)
{
	// シーン2D更新
	CScene_TWO::Draw();
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Debug(void)
{
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン最大フレームを取得
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInMaxFram(void)
{
	if (m_pFadeIn != NULL)
	{
		return m_pFadeIn->nMaxFade;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードインフレームを取得
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInFram(void)
{
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン最大開始アニメタイムを取得
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInMaxAnimTime(void)
{
	if (m_pFadeIn != NULL)
	{
		return m_pFadeIn->nMaxAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン開始アニメタイムを取得
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeInAnimTime(void)
{
	if (m_pFadeIn != NULL)
	{
		return m_pFadeIn->nCntAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト最大開始アニメタイムを取得
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeOutMaxAnimTime(void)
{
	if (m_pFadeOut != NULL)
	{
		return m_pFadeOut->nCntAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト開始アニメタイムを取得
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int const C2DPresents::GetFadeOutAnimTime(void)
{
	if (m_pFadeOut != NULL)
	{
		return m_pFadeOut->nCntAnimStart;
	}
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(シーン管理)処理
//	offsettype	: タイプ
//	pos			: 位置
//	size		: サイズ
//	nTextype	: テクスチャータイプ
//	frot		: 角度
//	col			: 色
//	layer		: レイヤー
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents * C2DPresents::Create(
	OFFSET_TYPE		const & type,		// タイプ
	D3DXVECTOR3		const & pos,		// 位置
	D3DXVECTOR2		const & size,		// サイズ
	int				const & nTextype,	// テクスチャータイプ
	float			const & frot,		// 角度
	D3DXCOLOR		const & col,		// 色
	CScene::LAYER	const & layer		// レイヤー
)
{
	// 変数宣言
	C2DPresents * p2DPresents;		// 2Dプレゼンツ2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	p2DPresents = new C2DPresents;
	// シーン管理設定
	p2DPresents->ManageSetting(layer);
	// 設定
	p2DPresents->SetOffset(type);		// タイプ
	p2DPresents->SetPosition(pos);		// 位置
	p2DPresents->SetSize(size);			// サイズ
	p2DPresents->BindTexture(nTextype);	// テクスチャータイプ
	p2DPresents->SetRot(frot);			// 角度
	p2DPresents->SetCol(col);			// カラー
	// 初期化処理
	p2DPresents->Init();
	// 生成したオブジェクトを返す
	return p2DPresents;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)処理
//	offsettype	: タイプ
//	pos			: 位置
//	size		: サイズ
//	nTextype	: テクスチャータイプ
//	frot		: 角度
//	col			: 色
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents * C2DPresents::Create_Self(
	OFFSET_TYPE		const & type,		// タイプ
	D3DXVECTOR3		const & pos,		// 位置
	D3DXVECTOR2		const & size,		// サイズ
	int				const & nTextype,	// テクスチャータイプ
	float			const & frot,		// 角度
	D3DXCOLOR		const & col			// 色
)
{
	// 変数宣言
	C2DPresents * p2DPresents;		// 2Dプレゼンツ2Dクラス
									// メモリの生成(初め->基本クラス,後->派生クラス)
	p2DPresents = new C2DPresents;
	// 設定
	p2DPresents->SetOffset(type);		// タイプ
	p2DPresents->SetPosition(pos);		// 位置
	p2DPresents->SetSize(size);			// サイズ
	p2DPresents->BindTexture(nTextype);	// テクスチャータイプ
	p2DPresents->SetRot(frot);			// 角度
	p2DPresents->SetCol(col);			// カラー
	// 初期化処理
	p2DPresents->Init();
	// 生成したオブジェクトを返す
	return p2DPresents;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成(個人管理)処理
//	offsettype	: タイプ
//	pos			: 位置
//	size		: サイズ
//	nTextype	: テクスチャータイプ
//	frot		: 角度
//	col			: 色
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<C2DPresents> C2DPresents::Create_Unique(
	OFFSET_TYPE		const & type,		// タイプ
	D3DXVECTOR3		const & pos,		// 位置
	D3DXVECTOR2		const & size,		// サイズ
	int				const & nTextype,	// テクスチャータイプ
	float			const & frot,		// 角度
	D3DXCOLOR		const & col			// 色
)
{
	// 変数宣言
	std::unique_ptr<C2DPresents> p2DPresents(new C2DPresents);		// 2Dプレゼンツ2Dクラス
	// 設定
	p2DPresents->SetOffset(type);		// タイプ
	p2DPresents->SetPosition(pos);		// 位置
	p2DPresents->SetSize(size);			// サイズ
	p2DPresents->BindTexture(nTextype);	// テクスチャータイプ
	p2DPresents->SetRot(frot);			// 角度
	p2DPresents->SetCol(col);			// カラー
	// 初期化処理
	p2DPresents->Init();
	// 生成したオブジェクトを返す
	return p2DPresents;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 点滅更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_ColorFlashing(void)
{
	// ヌルだったら関数を抜ける
	if (m_pCooperation->pCol_flash == NULL)
	{
		return;
	}
	// 変数宣言
	D3DXCOLOR col;	// 色
	// 情報取得
	col = CScene_TWO::GetCol();

	// 切り替えOFF時
	if (!m_pCooperation->pCol_flash->bSwitch)
	{
		// 切り替えON時
		if (m_pCooperation->pCol_flash->nCntTimeSwitch == m_pCooperation->pCol_flash->nTimeSwitch)
		{
			// 切り替えON
			m_pCooperation->pCol_flash->bSwitch = true;
			// 切り替わるOFF時
			if (!m_pCooperation->pCol_flash->bChange)
			{
				// 差分計算
				m_pCooperation->pCol_flash->colDiff =
					(m_pCooperation->pCol_flash->changecol - m_pCooperation->pCol_flash->Origincol) / (float)m_pCooperation->pCol_flash->nTimeChange;
			}
			// 切り替わるON時
			else
			{
				// 差分計算
				m_pCooperation->pCol_flash->colDiff =
					(m_pCooperation->pCol_flash->Origincol - m_pCooperation->pCol_flash->changecol) / (float)m_pCooperation->pCol_flash->nTimeChange;
			}
		}
		// 切り替え時間加算
		m_pCooperation->pCol_flash->nCntTimeSwitch++;
		// 関数を抜ける
		return;
	}
	// 切り替えON時
	else
	{
		// 切り替えOFF|切り替わるON/OFF
		if (m_pCooperation->pCol_flash->nCntTimeChange == m_pCooperation->pCol_flash->nTimeChange)
		{
			// 切り替えOFF
			m_pCooperation->pCol_flash->bSwitch = false;
			// 切り替わるOFF時
			if (!m_pCooperation->pCol_flash->bChange)
			{
				// 目標の色に強制変化
				col = m_pCooperation->pCol_flash->changecol;
			}
			// 切り替わるON時
			else
			{
				// 目標の色に強制変化
				col = m_pCooperation->pCol_flash->Origincol;
			}
			// 切り替わるON/OFF
			m_pCooperation->pCol_flash->bChange = !m_pCooperation->pCol_flash->bChange;
			// カウント初期化
			m_pCooperation->pCol_flash->nCntTimeSwitch = 0;
			m_pCooperation->pCol_flash->nCntTimeChange = 0;
			// 関数を抜ける
			return;
		}
		// 切り替わる時間加算
		m_pCooperation->pCol_flash->nCntTimeChange++;
	}
	// 色の変化
	col += m_pCooperation->pCol_flash->colDiff;
	// 色の設定
	CScene_TWO::SetCol(col);
	// 頂点カラーの設定
	CScene_TWO::Set_Vtx_Col();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 拡大縮小率更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_Scaling(void)
{
	// ヌルだったら関数を抜ける
	if (m_pCooperation->pScal == NULL)
	{
		return;
	}

	// 切り替えOFF|切り替わるON/OFF
	if (m_pCooperation->pScal->nCntTimeChange == m_pCooperation->pScal->nTimeChange)
	{
		// 切り替わるON/OFF
		m_pCooperation->pScal->bChange = !m_pCooperation->pScal->bChange;
		// 切り替わるOFF時
		if (!m_pCooperation->pScal->bChange)
		{
			// 拡大率の差分
			m_pCooperation->pScal->fScalDiff = (m_pCooperation->pScal->fScalChange - m_pCooperation->pScal->fScal) / m_pCooperation->pScal->nTimeChange;
			// 目標の拡大率に強制変化
			m_pCooperation->pScal->fScal = 1.0f;
		}
		// 切り替わるON時
		else
		{
			// 目標の拡大率に強制変化
			m_pCooperation->pScal->fScal = m_pCooperation->pScal->fScalChange;
			// 拡大率の差分
			m_pCooperation->pScal->fScalDiff = (1.0f - m_pCooperation->pScal->fScalChange) / m_pCooperation->pScal->nTimeChange;
		}
		// カウント初期化
		m_pCooperation->pScal->nCntTimeChange = 0;
		// 関数を抜ける
		return;
	}
	// 切り替わる時間加算
	m_pCooperation->pScal->nCntTimeChange++;
	// 拡大率の変化
	m_pCooperation->pScal->fScal += m_pCooperation->pScal->fScalDiff;
	// サイズの変化
	CScene_TWO::SetSize(m_pCooperation->pScal->OriginSize * m_pCooperation->pScal->fScal);
	// 頂点カラーの設定
	CScene_TWO::Set_Vtx_Pos();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェード(指定色)処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_Fade_DesignCol(
	D3DXCOLOR const & DiffCol ,	// 差分色
	D3DXCOLOR const & EndCol,	// 目標色
	int & nFram,				// 現在のフレーム
	int const & nMaxFram		// 最大のフレーム
)
{
	// 変数宣言
	D3DXCOLOR col;	// 色
	// 情報取得
	col = CScene_TWO::GetCol();
	// 最大フレームになったら目標色に強制的に変える
	if (nFram == nMaxFram)
	{
		col = EndCol;
		// 色の設定
		CScene_TWO::SetCol(col);
		// 頂点カラーの設定
		Set_Vtx_Col();
		// フレーム加算
		nFram++;
		return;
	}
	// フレーム期間の制限
	else if (nFram > nMaxFram)
	{
		return;
	}
	// 差分加算
	col += DiffCol;
	// 色の設定
	CScene_TWO::SetCol(col);
	// 頂点カラーの設定
	CScene_TWO::Set_Vtx_Col();
	// フレーム加算
	nFram++;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェード(移動)処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Update_Fade_DesignMove(
	D3DXVECTOR3 const & DiffPos,	// 差分色
	D3DXVECTOR3 const & GoalPos,	// 目標色
	int & nFram,					// 現在のフレーム
	int const & nMaxFram			// 最大のフレーム
)
{
	// 変数宣言
	D3DXVECTOR3 pos;					// 位置
	// 情報取得
	pos = CScene_TWO::GetPosition();	// 位置
	// 最大フレームになったら目標色に強制的に変える
	if (nFram == nMaxFram)
	{
		pos = GoalPos;
		// 位置の設定
		CScene_TWO::SetPosition(pos);
		// 頂点座標の設定
		Set_Vtx_Pos();
		// フレーム加算
		nFram++;
		return;
	}
	// フレーム期間の制限
	else if (nFram > nMaxFram)
	{
		return;
	}
	// 差分加算
	pos += DiffPos;
	// 位置の設定
	CScene_TWO::SetPosition(pos);
	// 頂点座標の設定
	CScene_TWO::Set_Vtx_Pos();
	// フレーム加算
	nFram++;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::Update_FadeIn(void)
{
	// フェードインがヌルだったら関数を抜ける
	if (m_pFadeIn == NULL)
	{
		m_Fadetype = FADETYPE_COOPERATION;
		return false;
	}
	// 強制終了処理
	if (m_pFadeIn->bCompulsion)
	{
		if (CManager::GetKeyConfig()->GetKeyConfigTrigger(CKeyConfig::CONFIG_DECISION))
		{
			// フェードインの強制終了
			Compulsion_FadeIn();
		}
	}
	// フェードが完了したら関数を抜ける
	if (!m_pFadeIn->bFade)
	{
		return false;
	}
	// フェードインがアニメスタート時間
	if (m_pFadeIn->nCntAnimStart < m_pFadeIn->nMaxAnimStart)
	{
		m_pFadeIn->nCntAnimStart++;
		return true;
	}
	// 指定色がヌルじゃなかったら関数に入る
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		// フェード(指定色)処理
		Update_Fade_DesignCol(
			m_pFadeIn->pDesign_Col->DiffCol,
			m_pFadeIn->pDesign_Col->EndCol,
			m_pFadeIn->pDesign_Col->nfram,
			m_pFadeIn->pDesign_Col->nMaxfram
		);
	}
	// 移動がヌルじゃなかったら関数に入る
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		// フェード(指定色)処理
		Update_Fade_DesignMove(
			m_pFadeIn->pDesign_Move->DiffPos,
			m_pFadeIn->pDesign_Move->EndPos,
			m_pFadeIn->pDesign_Move->nfram,
			m_pFadeIn->pDesign_Move->nMaxfram
		);
	}
	// フェードカウントの加算
	m_pFadeIn->nCntFade++;
	// フェード期間が終了していたら関数を抜ける
	if (m_pFadeIn->nCntFade >= m_pFadeIn->nMaxFade)
	{
		m_pFadeIn->bFade = false;
		// フェードタイプ
		m_Fadetype = FADETYPE_COOPERATION;
	}
	// フェード状態を返す
	return m_pFadeIn->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::Update_FadeOut(void)
{
	// ヌルだったら関数を抜ける
	if (m_pFadeOut == NULL)
	{
		m_Fadetype = FADETYPE_END;
		return false;
	}
	// 強制終了処理
	if (m_pFadeOut->bCompulsion)
	{
		if (CManager::GetKeyConfig()->GetKeyConfigTrigger(CKeyConfig::CONFIG_DECISION))
		{
			// フェードインの強制終了
			Compulsion_FadeOut();
		}
	}
	// フェードが完了したら関数を抜ける
	if (!m_pFadeOut->bFade)
	{
		return false;
	}
	// フェードインがアニメスタート時間
	if (m_pFadeOut->nCntAnimStart < m_pFadeOut->nMaxAnimStart)
	{
		m_pFadeOut->nCntAnimStart++;
		return true;
	}
	// 指定色がヌルじゃなかったら関数に入る
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		// フェード(指定色)処理
		Update_Fade_DesignCol(
			m_pFadeOut->pDesign_Col->DiffCol,
			m_pFadeOut->pDesign_Col->EndCol,
			m_pFadeOut->pDesign_Col->nfram,
			m_pFadeOut->pDesign_Col->nMaxfram
		);
	}
	// 移動がヌルじゃなかったら関数に入る
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		// フェード(移動)処理
		Update_Fade_DesignMove(
			m_pFadeOut->pDesign_Move->DiffPos,
			m_pFadeOut->pDesign_Move->EndPos,
			m_pFadeOut->pDesign_Move->nfram,
			m_pFadeOut->pDesign_Move->nMaxfram
		);
	}
	// フェードカウントの加算
	m_pFadeOut->nCntFade++;
	// フェード期間が終了したら関数を抜ける
	if (m_pFadeOut->nCntFade >= m_pFadeOut->nMaxFade)
	{
		m_pFadeOut->nCntAnimStart = 0;
		m_pFadeOut->bFade = false;
		// フェードタイプ
		m_Fadetype = FADETYPE_END;
	}
	// フェード状態を返す
	return m_pFadeOut->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 強調更新処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::Update_Cooperation(void)
{
	// ヌルだったら関数を抜ける
	if (m_pCooperation == NULL)
	{
		return false;
	}
	// 点滅情報がヌルじゃなかったら関数に入る
	if (m_pCooperation->pCol_flash != NULL)
	{
		// 色の点滅処理
		Update_ColorFlashing();
	}
	// 拡大率情報がヌルじゃなかったら関数に入る
	if (m_pCooperation->pScal != NULL)
	{
		// 拡大率更新
		Update_Scaling();
	}
	return false;
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン(移動)設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn(
	D3DXVECTOR3 const & BeginPos,	// 初期位置
	D3DXVECTOR3 const & EndPos,		// 目的位置
	int const & nMaxfram			// 最大フレーム
)
{
	// フェード情報のメモリ確保
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	// フェードアウト(位置)のメモリが確保済みだったら抜ける
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		return;
	}
	// フェードイン(移動)情報のメモリ確保
	m_pFadeIn->pDesign_Move = new DESIGN_MOVE;
	// 目的位置設定
	m_pFadeIn->pDesign_Move->EndPos = EndPos;
	// 初期位置設定
	CScene_TWO::SetPosition(m_pFadeIn->pDesign_Move->BeginPos = BeginPos);
	// 頂点座標の設定
	CScene_TWO::Set_Vtx_Pos();
	// 最大フレーム
	m_pFadeIn->pDesign_Move->nMaxfram = nMaxfram;
	// 差分計算
	m_pFadeIn->pDesign_Move->DiffPos =
		(m_pFadeIn->pDesign_Move->EndPos -
		m_pFadeIn->pDesign_Move->BeginPos) /
		(float)m_pFadeIn->pDesign_Move->nMaxfram;
	// フェード時間
	if (m_pFadeIn->nMaxFade < m_pFadeIn->pDesign_Move->nMaxfram)
	{
		m_pFadeIn->nMaxFade = m_pFadeIn->pDesign_Move->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン(色指定)設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn(
	D3DXCOLOR const & BeginCol,		// 初期カラー
	D3DXCOLOR const & EndCol,		// 目標カラー
	int const & nMaxfram			// 最大フレーム
)
{
	// フェード情報のメモリ確保
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	// フェードイン(位置)のメモリが確保済みだったら抜ける
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		return;
	}
	// フェードイン(位置)のメモリ確保
	m_pFadeIn->pDesign_Col = new DESIGN_COL;
	// 最大フレーム数
	m_pFadeIn->pDesign_Col->nMaxfram = nMaxfram;
	// 色の設定
	m_pFadeIn->pDesign_Col->BeginCol = BeginCol;
	// 初期カラー
	m_pFadeIn->pDesign_Col->EndCol = EndCol;
	// 最大フレーム
	m_pFadeIn->pDesign_Col->nMaxfram = nMaxfram;
	// 差分計算
	m_pFadeIn->pDesign_Col->DiffCol =
		(m_pFadeIn->pDesign_Col->EndCol -
			m_pFadeIn->pDesign_Col->BeginCol) /
			(float)m_pFadeIn->pDesign_Col->nMaxfram;
	// フェード時間
	if (m_pFadeIn->nMaxFade < m_pFadeIn->pDesign_Col->nMaxfram)
	{
		m_pFadeIn->nMaxFade = m_pFadeIn->pDesign_Col->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードインのスタート時間設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn_StarAnim(
	int const & nMaxStartAnim
)
{
	// フェード情報のメモリ確保
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	// アニメスタート時間
	m_pFadeIn->nMaxAnimStart = nMaxStartAnim;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン強制終了設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeIn_Compulsion(
	bool const & bCompulsion
)
{
	// フェード情報のメモリ確保
	if (m_pFadeIn == NULL)
	{
		m_pFadeIn = new FADE;
	}
	m_pFadeIn->bCompulsion = bCompulsion;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードインの使用状態取得処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::GetFadeIn_Bool(void)
{
	if (m_pFadeIn == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2Dプレゼンツ2D内のフェードイン情報がありません。\n使用状態を取得できません。");
#endif // _DEBUG
		return false;
	}
	// 強制終了設定
	return m_pFadeIn->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン強制終了処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Compulsion_FadeIn(void)
{
	if (m_pFadeIn == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2Dプレゼンツ2D内のフェードイン情報がありません。\n使用状態をOFFにできませんでした。");
#endif // _DEBUG
		return;
	}
	// フェードインの使用状態をfalseに
	m_pFadeIn->bFade = false;
	// アニメスタート時間を最大に
	m_pFadeIn->nCntAnimStart = m_pFadeIn->nMaxAnimStart;
	// フェードインの指定色情報がNULLではないとき
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		// 色の設定
		CScene_TWO::SetCol(m_pFadeIn->pDesign_Col->EndCol);
		// 頂点カラーの設定
		CScene_TWO::Set_Vtx_Col();
	}
	// フェードインの指定色情報がNULLではないとき
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		CScene_TWO::SetPosition(m_pFadeIn->pDesign_Move->EndPos);
		// 頂点座標の設定
		CScene_TWO::Set_Vtx_Pos();
	}
	// フェードタイプ
	m_Fadetype = FADETYPE_COOPERATION;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン(位置)開始処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Start_FadeIn(void)
{
	// フェードインがNULLなら
	// ->関数を抜ける
	if (m_pFadeIn == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2Dプレゼンツ2D内のフェードイン情報がありません。\nフェードイン開始ができませんでした。");
#endif // _DEBUG
		return;
	}

	// フェードイン(位置)初期化
	if (m_pFadeIn->pDesign_Move != NULL)
	{
		// 初期位置設定
		CScene_TWO::SetPosition(m_pFadeIn->pDesign_Move->BeginPos);
		// 頂点座標の設定
		CScene_TWO::Set_Vtx_Pos();
		// フレームの初期化
		m_pFadeIn->pDesign_Move->nfram = 0;
	}
	// フェードイン(カラー)初期化
	if (m_pFadeIn->pDesign_Col != NULL)
	{
		// 色の設定
		CScene_TWO::SetCol(m_pFadeIn->pDesign_Col->BeginCol);
		// 頂点カラーの設定
		CScene_TWO::Set_Vtx_Col();
		// フレームの初期化
		m_pFadeIn->pDesign_Col->nfram = 0;
	}
	// フェード中
	m_pFadeIn->bFade = true;
	// カウントリセット
	m_pFadeIn->nCntFade = 0;
	// カウントリセット
	m_pFadeIn->nCntAnimStart = 0;
	// フェードタイプ
	m_Fadetype = FADETYPE_IN;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト(移動)設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut(
	D3DXVECTOR3 const & BeginPos,	// 初期位置
	D3DXVECTOR3 const & EndPos,		// 目的位置
	int const & nMaxfram			// 最大フレーム
)
{
	// フェード情報のメモリ確保
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// フェードアウト(位置)のメモリが確保済みだったら抜ける
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		return;
	}
	// フェードイン(移動)情報のメモリ確保
	m_pFadeOut->pDesign_Move = new DESIGN_MOVE;
	// 目的位置設定
	m_pFadeOut->pDesign_Move->EndPos = EndPos;
	// 初期位置設定
	m_pFadeOut->pDesign_Move->BeginPos = BeginPos;
	// 最大フレーム
	m_pFadeOut->pDesign_Move->nMaxfram = nMaxfram;
	// 差分計算
	m_pFadeOut->pDesign_Move->DiffPos =
		(m_pFadeOut->pDesign_Move->EndPos -
		m_pFadeOut->pDesign_Move->BeginPos) /
		(float)m_pFadeOut->pDesign_Move->nMaxfram;
	// フェード時間
	if (m_pFadeOut->nMaxFade < m_pFadeOut->pDesign_Move->nMaxfram)
	{
		m_pFadeOut->nMaxFade = m_pFadeOut->pDesign_Move->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト(色指定)設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut(
	D3DXCOLOR const & BeginCol,		// 初期カラー
	D3DXCOLOR const & EndCol,		// 目標カラー
	int const & nMaxfram			// 最大フレーム
)
{
	// フェード情報のメモリ確保
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// フェードアウト(色指定)のメモリが確保済みだったら抜ける
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		return;
	}
	// フェードアウト(色指定)のメモリ確保
	m_pFadeOut->pDesign_Col = new DESIGN_COL;
	// 初期カラー
	m_pFadeOut->pDesign_Col->BeginCol = BeginCol;
	// 初期カラー
	m_pFadeOut->pDesign_Col->EndCol = EndCol;
	// 最大フレーム
	m_pFadeOut->pDesign_Col->nMaxfram = nMaxfram;
	// 差分計算
	m_pFadeOut->pDesign_Col->DiffCol =
		(m_pFadeOut->pDesign_Col->EndCol -
			m_pFadeOut->pDesign_Col->BeginCol) /
		(float)m_pFadeOut->pDesign_Col->nMaxfram;
	// フェード時間
	if (m_pFadeOut->nMaxFade < m_pFadeOut->pDesign_Col->nMaxfram)
	{
		m_pFadeOut->nMaxFade = m_pFadeOut->pDesign_Col->nMaxfram;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウトのスタート時間設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut_StarAnim(
	int const & nMaxStartAnim
)
{
	// フェード情報のメモリ確保
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// アニメスタート時間
	m_pFadeOut->nMaxAnimStart = nMaxStartAnim;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードイン強制終了設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetFadeOut_Compulsion(
	bool const & bCompulsion
)
{
	// フェード情報のメモリ確保
	if (m_pFadeOut == NULL)
	{
		m_pFadeOut = new FADE;
	}
	// 強制終了設定
	m_pFadeOut->bCompulsion = bCompulsion;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードインの使用状態設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool C2DPresents::GetFadeOut_Bool(void)
{
	if (m_pFadeOut == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2Dプレゼンツ2D内のフェードイン情報がありません。\n使用状態を取得できません。");
#endif // _DEBUG
		return false;
	}
	return m_pFadeOut->bFade;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト強制終了処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Compulsion_FadeOut(void)
{
	if (m_pFadeOut == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2Dプレゼンツ2D内のフェードアウト情報がありません。\n使用状態をOFFにできませんでした。");
#endif // _DEBUG
		return;
	}
	// フェードアウトの使用状態がfalseだったら関数を抜ける
	else if (!m_pFadeOut->bFade)
	{
		return;
	}
	// フェードインの使用状態をfalseに
	m_pFadeOut->bFade = false;
	// アニメスタート時間を最大に
	m_pFadeOut->nCntAnimStart = 0;
	// カウントリセット
	m_pFadeOut->nCntFade = 0;
	// フェードインの指定色情報がNULLではないとき
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		// 色の設定
		CScene_TWO::SetCol(m_pFadeOut->pDesign_Col->EndCol);
		// 頂点カラーの設定
		CScene_TWO::Set_Vtx_Col();
		// フレームの初期化
		m_pFadeOut->pDesign_Col->nfram = 0;
	}
	// フェードインの指定色情報がNULLではないとき
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		CScene_TWO::SetPosition(m_pFadeOut->pDesign_Move->EndPos);
		// 頂点座標の設定
		CScene_TWO::Set_Vtx_Pos();
		// フレームの初期化
		m_pFadeOut->pDesign_Move->nfram = 0;
	}
	// フェードタイプ
	m_Fadetype = FADETYPE_END;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードアウト開始処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::Start_FadeOut(void)
{
	// フェードアウトがNULLなら
	// ->関数を抜ける
	if (m_pFadeOut == NULL)
	{
#ifdef _DEBUG
		CCalculation::Messanger("2Dプレゼンツ2D内のフェードアウト情報がありません。\nフェードアウトの開始ができませんでした。");
#endif // _DEBUG
		return;
	}
	// フェードが行われていたら
	// ->関数を抜ける
	if (m_pFadeOut->bFade == true)
	{
		return;
	}
	// フェードアウト(カラー)初期化
	if (m_pFadeOut->pDesign_Col != NULL)
	{
		// 色の設定
		CScene_TWO::SetCol(m_pFadeOut->pDesign_Col->BeginCol);
		// 頂点カラーの設定
		CScene_TWO::Set_Vtx_Col();
		// フレームの初期化
		m_pFadeOut->pDesign_Col->nfram = 0;
	}
	// フェードアウト(位置)初期化
	if (m_pFadeOut->pDesign_Move != NULL)
	{
		// 初期位置設定
		CScene_TWO::SetPosition(m_pFadeOut->pDesign_Move->BeginPos);
		// 頂点座標の設定
		CScene_TWO::Set_Vtx_Pos();
		// フレームの初期化
		m_pFadeOut->pDesign_Move->nfram = 0;
	}
	// フェード中
	m_pFadeOut->bFade = true;
	// カウントリセット
	m_pFadeOut->nCntFade = 0;
	// カウントリセット
	m_pFadeOut->nCntAnimStart = 0;
	// フェードタイプ
	m_Fadetype = FADETYPE_OUT;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// フェードタイプ取得処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents::FADETYPE C2DPresents::GetFadetype(void)
{
	return m_Fadetype;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 強調(点滅)設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetCooperation(
	D3DXCOLOR const & changecol,
	int const & nTimeSwitch,
	int const & nTimeChange
)
{
	// 強調情報のメモリがNULLなら
	if (m_pCooperation == NULL)
	{
		m_pCooperation = new COOPERATION;
	}
	// 強調(点滅)のメモリが確保済みだったら抜ける
	if (m_pCooperation->pCol_flash != NULL)
	{
		return;
	}
	// 点滅情報のメモリ確保
	m_pCooperation->pCol_flash = new COL_FLASHING;
	// 初期カラー
	m_pCooperation->pCol_flash->Origincol = CScene_TWO::GetCol();
	// 変わるカラー
	m_pCooperation->pCol_flash->changecol = changecol;
	// タイムスイッチ
	m_pCooperation->pCol_flash->nTimeSwitch = nTimeSwitch;
	// タイムチェンジ
	m_pCooperation->pCol_flash->nTimeChange = nTimeChange;
	// フェードタイプ
	m_Fadetype = FADETYPE_COOPERATION;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 拡大率設定処理
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DPresents::SetCooperation(
	float const & fChangeScal,	// 拡大率
	int const & nTimeChange		// 切り替わる時間
)
{
	// 強調情報のメモリがNULLなら
	if (m_pCooperation == NULL)
	{
		m_pCooperation = new COOPERATION;
	}
	// 強調(拡大率)のメモリが確保済みだったら抜ける
	if (m_pCooperation->pScal != NULL)
	{
		return;
	}
	// 拡大率のメモリ確保
	m_pCooperation->pScal = new SCALING;
	// 最大拡大率拡大率
	m_pCooperation->pScal->fScalChange = fChangeScal;
	// タイムチェンジ
	m_pCooperation->pScal->nTimeChange = nTimeChange;
	// 拡大率差分
	m_pCooperation->pScal->fScalDiff = (m_pCooperation->pScal->fScalChange - m_pCooperation->pScal->fScal) / m_pCooperation->pScal->nTimeChange;
	// 初期サイズ
	m_pCooperation->pScal->OriginSize = CScene_TWO::GetSize();
}
