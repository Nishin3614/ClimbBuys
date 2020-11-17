// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ユーザーインターフェース処理 [ui.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "ui.h"
/* 描画 */
#include "fade.h"
#include "2Dpresents.h"
#include "game.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TEXTURE_XSPLIT(nX) (1.0f / nX)
#define TEXTURE_YSPLIT(nY) (1.0f / nY)
#define UI_MANAGER_FILE "data/LOAD/UI/ui_manager.txt"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<CUi::UI_LOAD> CUi::m_vec_UiLoad[CUi::UITYPE_MAX] = {};	// Ui読み込み用変数
bool	CUi::m_sta_UiUse[CUi::UITYPE_MAX] = {};					// このUIの使用状態
std::vector<std::string> CUi::m_vec_String = {};					// ファイル情報読み書き用

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::CUi()
{
	m_pScene_two = NULL;		// シーン2D
	m_pPresents = NULL;			// 2Dプレゼンツ
	m_Uitype = UITYPE_TILTE;	// UIタイプ
	m_nUi = 0;					// UI
	//m_nCount = 0;				// カウント
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::~CUi()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Init(void)
{
	// プレゼンの情報がヌルではない場合
	if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents != NULL)
	{
		// プレゼン情報作成
		m_pPresents = C2DPresents::Create_Self(
			(CScene_TWO::OFFSET_TYPE)m_vec_UiLoad[m_Uitype][m_nUi].nOffsetType,
			m_vec_UiLoad[m_Uitype][m_nUi].pos,
			m_vec_UiLoad[m_Uitype][m_nUi].size,
			m_vec_UiLoad[m_Uitype][m_nUi].nTexType,
			m_vec_UiLoad[m_Uitype][m_nUi].fRot,
			m_vec_UiLoad[m_Uitype][m_nUi].col
		);
		// テクスチャー座標設定
		m_pPresents->Set_Vtx_Tex(m_vec_UiLoad[m_Uitype][m_nUi].tex_first, m_vec_UiLoad[m_Uitype][m_nUi].tex_last);
		// 強調情報がヌルではない場合
		if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation != NULL)
		{
			// 色情報がヌルではない場合
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol != NULL)
			{
				// 色の設定
				m_pPresents->SetCooperation(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol->changecol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol->nTimeSwitch,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pCol->nTimeChange
				);
			}
			// 拡大率情報がヌルではない場合
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pScal != NULL)
			{
				// 拡大率の設定
				m_pPresents->SetCooperation(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pScal->fChangeScal,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pCooperation->pScal->nTimeChange
				);
			}
		}
		// フェードイン情報がヌルではない場合
		if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn != NULL)
		{
			// フェードイン(位置指定)情報がヌルではない場合
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos != NULL)
			{
				// フェードイン(位置指定)の設定
				m_pPresents->SetFadeIn(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos->BeginPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos->EndPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadePos->nMaxfram
				);
			}
			// フェードイン(色指定)情報がヌルではない場合
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol != NULL)
			{
				// フェードイン(色指定)の設定
				m_pPresents->SetFadeIn(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol->BeginCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol->EndCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->pFadeCol->nMaxfram
				);
			}
			// フェードイン強制終了情報設定
			m_pPresents->SetFadeIn_Compulsion(m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->bSkip);
			// フェードイン(スタート時間指定)の設定
			m_pPresents->SetFadeIn_StarAnim(
				m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeIn->FadeStart
			);
			// フェードイン開始
			m_pPresents->Start_FadeIn();
		}
		// フェードアウト情報がヌルではない場合
		if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut != NULL)
		{
			// フェードアウト(位置指定)情報がヌルではない場合
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos != NULL)
			{
				// フェードアウト(位置指定)の設定
				m_pPresents->SetFadeOut(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos->BeginPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos->EndPos,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadePos->nMaxfram
				);
			}
			// フェードアウト(色指定)情報がヌルではない場合
			if (m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol != NULL)
			{
				// フェードアウト(色指定)の設定
				m_pPresents->SetFadeOut(
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol->BeginCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol->EndCol,
					m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->pFadeCol->nMaxfram
				);
			}
			// フェードアウト強制終了情報設定
			m_pPresents->SetFadeOut_Compulsion(m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->bSkip);
			// フェードアウト(スタート時間指定)の設定
			m_pPresents->SetFadeOut_StarAnim(
				m_vec_UiLoad[m_Uitype][m_nUi].pPresents->pFadeOut->FadeStart
			);
			// フェード状態がFADETYPE_ENDなら
			// ->FADETYPE_COOPERATIONにする
			if (m_pPresents->GetFadetype() == C2DPresents::FADETYPE_END)
			{
				m_pPresents->SetFadetype(C2DPresents::FADETYPE_COOPERATION);
			}
		}
	}
	else
	{
		// シーン2D情報作成
		m_pScene_two = CScene_TWO::Create_Self(
			(CScene_TWO::OFFSET_TYPE)m_vec_UiLoad[m_Uitype][m_nUi].nOffsetType,
			m_vec_UiLoad[m_Uitype][m_nUi].pos,
			m_vec_UiLoad[m_Uitype][m_nUi].size,
			m_vec_UiLoad[m_Uitype][m_nUi].nTexType,
			m_vec_UiLoad[m_Uitype][m_nUi].fRot,
			m_vec_UiLoad[m_Uitype][m_nUi].col
		);
		// テクスチャー座標設定
		m_pScene_two->Set_Vtx_Tex(m_vec_UiLoad[m_Uitype][m_nUi].tex_first, m_vec_UiLoad[m_Uitype][m_nUi].tex_last);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Uninit(void)
{
	// プレゼン情報がヌルではない場合
	if (m_pPresents != NULL)
	{
		// 終了処理
		m_pPresents->Uninit();
		// メモリ開放
		delete m_pPresents;
		m_pPresents = NULL;
	}
	// シーン2D情報の場合
	else if(m_pScene_two != NULL)
	{
		// 終了処理
		m_pScene_two->Uninit();
		// メモリ開放
		delete m_pScene_two;
		m_pScene_two = NULL;
	}
	// 使用状態がtrueであればfalseに
	if (m_sta_UiUse[m_Uitype])
	{
		m_sta_UiUse[m_Uitype] = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Update(void)
{
	// プレゼン情報がヌルではない場合
	if (m_pPresents != NULL)
	{
		// 更新処理
		m_pPresents->Update();
	}
	// シーン2D情報の場合
	else
	{
		// 更新処理
		m_pScene_two->Update();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Draw(void)
{
	// プレゼン情報がヌルではない場合
	if (m_pPresents != NULL)
	{
		// 描画処理
		m_pPresents->Draw();
	}
	// シーン2D情報の場合
	else
	{
		// 描画処理
		m_pScene_two->Draw();
	}
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::Debug(void)
{
	// プレゼン情報がヌルではない場合
	if (m_pPresents != NULL)
	{
		// デバッグ処理
		m_pPresents->Debug();
	}
	// シーン2D情報の場合
	else
	{
		// デバッグ処理
		m_pScene_two->Debug();
	}
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// シーン2Dの取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CScene_TWO * CUi::GetScene_Two(void)
{
	// シーン2DがNULLではないのなら
	// ->情報を返す
	if (m_pScene_two != NULL) return m_pScene_two;
	// プレゼンツ2DがNULLではないのなら
	// ->情報を返す
	else if (m_pPresents != NULL) return m_pPresents;
	// NULLを返す
	else return nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレゼンツ2Dの取得
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DPresents * CUi::GetPresents(void)
{
	// プレゼンツ2DがNULLではないのなら
	// ->情報を返す
	if (m_pPresents != NULL) return m_pPresents;
	// NULLを返す
	else return nullptr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// テキストの読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CUi::Load(void)
{
	// 変数宣言
	HRESULT hr;
	// UIマネージャーの読み込み
	hr = UiManagerLoad();
	// UIスクリプトの読み込み
	hr = UiScriptLoad();
	// 結果を返す
	return hr;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// UIマネージャーの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CUi::UiManagerLoad(void)
{
	// 変数宣言
	// ファイルの中身格納用
	std::vector<std::vector<std::string>> vsvec_Contens;
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(UI_MANAGER_FILE, '\0');
	// 行ごとに回す
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		// 項目ごとに回す
		for (int nCntItem = 0; nCntItem < (signed)vsvec_Contens.at(nCntLine).size(); nCntItem++)
		{
			switch (nCntItem)
			{
				// パス情報
			case 0:
				m_vec_String.emplace_back(vsvec_Contens.at(nCntLine).at(nCntItem));
				break;
			default:
				break;
			}
		}
	}
	// ファイルの中身の開放
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		vsvec_Contens[nCntLine].clear();
		vsvec_Contens[nCntLine].shrink_to_fit();
	}
	vsvec_Contens.clear();
	vsvec_Contens.shrink_to_fit();

	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// UIスクリプトの読み込み
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CUi::UiScriptLoad(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntError = 0;				// カウントロード
	int	nCntUi = 0;					// カウントUI
	char cComp[128];				// 特定の文字列の比較
	char cRaedText[128];			// 文字として読み取り用
	char cHeadText[128];			// 比較するよう
	char cDie[128];					// 不必要な文字

	for (int nCntLoad = 0; nCntLoad < (signed)m_vec_String.size(); nCntLoad++)
	{
		// 読み込んだUIファイルの上限を超えたとき
		// ->ループを抜ける
		if (nCntLoad >= UITYPE_MAX)
		{
			break;
		}
		nCntUi = 0;
		cHeadText[0] = '\0';
		cHeadText[0] = '\0';
		// ファイルが開かれていなかったら
		if ((pFile = fopen(m_vec_String[nCntLoad].c_str(), "r")) == NULL)
		{// メッセージの表示
#ifdef _DEBUG
			CCalculation::Messanger("UIのテキストファイルが見つかりませんでした");
#endif // _DEBUG
			continue;
		}
		// スクリプトが存在しているかチェック
		if (!CCalculation::Existenceofscript(pFile))
		{
#ifdef _DEBUG
			CCalculation::Messanger("UIの[SCRIP]が見つかりません\n終了してください");
#endif // _DEBUG
			fclose(pFile);
			continue;
		}
		// エンドスクリプトが来るまでループ
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);

			// 改行だったら
			if (strcmp(cHeadText, "\n") == 0)
			{
			}

			// モデルセットが来たら
			else if (strcmp(cHeadText, "SET") == 0)
			{
				// 格納する
				m_vec_UiLoad[nCntLoad].emplace_back(UI_LOAD());
				// 比較の初期化
				cComp[0] = '\0';
				// 読み込んど文字列代入
				sscanf(cRaedText, "%s %s", &cDie, &cComp);
				// プレゼンツ用
				if (strcmp(cComp, "PRESENTS") == 0)
				{
					// メモリ確保
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents = new PRESENTS_LOAD;
				}
				// エンドモデルセットが来るまでループ
				while (strcmp(cHeadText, "END_SET") != 0)
				{
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// 位置情報読み込み
					if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).pos.x,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).pos.y,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).pos.z);
					}
					// 位置情報読み込み
					else if (strcmp(cHeadText, "COL") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.r,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.b,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.g,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).col.a
						);
					}
					// サイズ情報読み込み
					else if (strcmp(cHeadText, "SIZE") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).size.x,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).size.y);
					}
					// 角度情報読み込み
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).fRot);
					}
					// テクスチャー座標(最初)情報読み込み
					else if (strcmp(cHeadText, "FIRST_TEX") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_first.x,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_first.y);
					}
					// テクスチャー座標(最後)情報読み込み
					else if (strcmp(cHeadText, "LAST_TEX") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_last.x,
							&m_vec_UiLoad[nCntLoad][nCntUi].tex_last.y);
					}
					// テクスチャータイプ情報読み込み
					else if (strcmp(cHeadText, "TEXTYPE") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).nTexType);
					}
					// オフセットタイプ情報読み込み
					else if (strcmp(cHeadText, "OFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&m_vec_UiLoad[nCntLoad].at(nCntUi).nOffsetType);
					}
					// プレゼンツ用の変数がヌルではなかったら入る
					else if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents != NULL)
					{
						// フェードイン情報読み込み
						if (strcmp(cHeadText, "FADE_IN") == 0)
						{
							// フェードインのメモリ確保
							m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn = new FADEIN_LOAD;
							// エンドフェードインが来るまでループ
							while (strcmp(cHeadText, "END_FADE_IN") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// スタートフェードタイム情報読み込み
								if (strcmp(cHeadText, "STARTFADETIME") == 0)
								{
									sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
										&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->FadeStart);
								}
								// フェードスキップ情報読み込み
								else if (strcmp(cHeadText, "FADESKIP") == 0)
								{
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->bSkip = true;
								}
								// フェード色情報読み込み
								else if (strcmp(cHeadText, "FADE_COL") == 0)
								{
									// フェードカラーのメモリ確保
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol = new FADE_COL_LOAD;
									// エンドフェードカラーが来るまでループ
									while (strcmp(cHeadText, "END_FADE_COL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// 初期色情報読み込み
										if (strcmp(cHeadText, "BEGIN_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->BeginCol.a);
										}
										// 終了色情報読み込み
										else if (strcmp(cHeadText, "END_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->EndCol.a);
										}
										// 最大フレーム情報読み込み
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol->nMaxfram);
										}
									}
								}
								// フェード位置情報読み込み
								else if (strcmp(cHeadText, "FADE_POS") == 0)
								{
									// フェード位置のメモリ確保
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos = new FADE_POS_LOAD;
									// エンドフェード位置が来るまでループ
									while (strcmp(cHeadText, "END_FADE_POS") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// 初期位置情報読み込み
										if (strcmp(cHeadText, "BEGIN_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->BeginPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->BeginPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->BeginPos.z);
										}
										// 終了位置情報読み込み
										else if (strcmp(cHeadText, "END_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->EndPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->EndPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->EndPos.z);
										}
										// 最大フレーム情報読み込み
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos->nMaxfram);
										}
									}
								}
							}
						}
						// フェードアウト情報読み込み
						else if (strcmp(cHeadText, "FADE_OUT") == 0)
						{
							// フェードアウトメモリ確保
							m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut = new FADEOUT_LOAD;
							// エンドフェードアウトが来るまでループ
							while (strcmp(cHeadText, "END_FADE_OUT") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// スタートフェードタイム情報読み込み
								if (strcmp(cHeadText, "STARTFADETIME") == 0)
								{
									sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
										&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->FadeStart);
								}
								// フェードスキップ情報読み込み
								else if (strcmp(cHeadText, "FADESKIP") == 0)
								{
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->bSkip = true;
								}
								// フェード色情報読み込み
								else if (strcmp(cHeadText, "FADE_COL") == 0)
								{
									// フェードカラーのメモリ確保
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol = new FADE_COL_LOAD;
									// エンドフェードカラーが来るまでループ
									while (strcmp(cHeadText, "END_FADE_COL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// 初期色情報読み込み
										if (strcmp(cHeadText, "BEGIN_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->BeginCol.a);
										}
										// 終了色情報読み込み
										else if (strcmp(cHeadText, "END_COL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->EndCol.a);
										}
										// 最大フレーム情報読み込み
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol->nMaxfram);
										}
									}
								}
								// フェード位置情報読み込み
								else if (strcmp(cHeadText, "FADE_POS") == 0)
								{
									// フェード位置のメモリ確保
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos = new FADE_POS_LOAD;
									// エンドフェード位置が来るまでループ
									while (strcmp(cHeadText, "END_FADE_POS") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// 初期位置情報読み込み
										if (strcmp(cHeadText, "BEGIN_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->BeginPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->BeginPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->BeginPos.z);
										}
										// 終了位置情報読み込み
										else if (strcmp(cHeadText, "END_POS") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->EndPos.x,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->EndPos.y,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->EndPos.z);
										}
										// 最大フレーム情報読み込み
										else if (strcmp(cHeadText, "MAXFRAME") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos->nMaxfram);
										}
									}
								}
							}
						}
						// 強調情報読み込み
						else if (strcmp(cHeadText, "COOPERATION") == 0)
						{
							// 強調メモリ確保
							m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation = new COOPERATION_LOAD;
							// エンド強調が来るまでループ
							while (strcmp(cHeadText, "END_COOPERATION") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// 拡大率情報読み込み
								if (strcmp(cHeadText, "SCAL") == 0)
								{
									// 拡大率のメモリ確保
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal = new COOPERATION_SCAL_LOAD;
									// エンド拡大率が来るまでループ
									while (strcmp(cHeadText, "END_SCAL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// 拡大率情報読み込み
										if (strcmp(cHeadText, "CHANGESCAL") == 0)
										{
											sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal->fChangeScal);
										}
										// 変化の時間情報読み込み
										else if (strcmp(cHeadText, "TIMECHANGE") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal->nTimeChange);
										}
									}
								}
								// 色情報読み込み
								else if (strcmp(cHeadText, "COL") == 0)
								{
									// 色のメモリ確保
									m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol = new COOPERATION_COL_LOAD;
									// エンド色が来るまでループ
									while (strcmp(cHeadText, "END_COL") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);
										// 変化する色情報読み込み
										if (strcmp(cHeadText, "CHANGECOL") == 0)
										{
											sscanf(cRaedText, "%s %s %f %f %f %f", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.r,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.g,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.b,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->changecol.a);
										}
										// 切り替え時間情報読み込み
										else if (strcmp(cHeadText, "TIMESWITCH") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->nTimeSwitch);
										}
										// 切り替えにかかる時間情報読み込み
										else if (strcmp(cHeadText, "TIMECHANGE") == 0)
										{
											sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
												&m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol->nTimeChange);
										}
									}
								}
							}
						}
					}
					// エラーカウントをインクリメント
					nCntError++;
					if (nCntError > FILELINE_ERROW)
					{// エラー
						nCntError = 0;
						fclose(pFile);
						CCalculation::Messanger("UIセットがありません");
						return E_FAIL;
					}
				}
				// UIカウントアップ
				nCntUi++;
			}
			// エラーカウントをインクリメント
			nCntError++;
			if (nCntError > FILELINE_ERROW)
			{// エラー
				nCntError = 0;
				fclose(pFile);
				CCalculation::Messanger("エンドスクリプトがありません");
				return E_FAIL;
			}
		}
		fclose(pFile);
	}
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全ソースの開放処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CUi::UnLoad(void)
{
	// 全UIタイプのループ
	for (int nCntLoad = 0; nCntLoad < UITYPE_MAX; nCntLoad++)
	{
		// UIのループ
		for (int nCntUi = 0; nCntUi < (signed)m_vec_UiLoad[nCntLoad].size(); nCntUi++)
		{
			// プレゼンの情報がヌルの場合ループスキップ
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents == NULL)
			{
				continue;
			}
			// フェードインの情報がヌルではない場合情報を開放する
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn != NULL)
			{
				// フェード色の情報がヌルではない場合情報を開放する
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadeCol = NULL;
				}
				// フェード位置の情報がヌルではない場合情報を開放する
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn->pFadePos = NULL;
				}
				// フェードインの情報の開放
				delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn;
				m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeIn = NULL;
			}
			// フェードアウトの情報がヌルではない場合情報を開放する
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut != NULL)
			{
				// フェード色の情報がヌルではない場合情報を開放する
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadeCol = NULL;
				}
				// フェード位置の情報がヌルではない場合情報を開放する
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut->pFadePos = NULL;
				}
				// フェードアウトの情報の開放
				delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut;
				m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pFadeOut = NULL;
			}
			// 強調の情報がヌルではない場合情報を開放する
			if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation != NULL)
			{
				// 色情報がヌルではない場合情報を開放する
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pCol = NULL;
				}
				// 拡大率の情報がヌルではない場合情報を開放する
				if (m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal != NULL)
				{
					delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal;
					m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation->pScal = NULL;
				}
				// 強調の情報の開放
				delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation;
				m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents->pCooperation = NULL;
			}
			// プレゼンの情報の開放
			delete m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents;
			m_vec_UiLoad[nCntLoad].at(nCntUi).pPresents = NULL;
		}
		// std::vectorの開放
		std::vector<UI_LOAD>().swap(m_vec_UiLoad[nCntLoad]);
	}
	m_vec_String.clear();
	m_vec_String.shrink_to_fit();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理(シーン管理)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi * CUi::Create(
	UITYPE const &Uitype,
	int const &nUi
)
{
	// 変数宣言
	CUi * pUi;
	// メモリ確保
	pUi = new CUi;
	// UIタイプの設定
	pUi->m_Uitype = Uitype;
	// UIID設定
	pUi->m_nUi = nUi;
	// シーン管理設定
	pUi->ManageSetting(CScene::LAYER_UI);
	// 初期化処理
	pUi->Init();
	return pUi;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 作成処理(個人管理)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CUi> CUi::Create_Self(
	UITYPE const &Uitype,
	int const &nUi
)
{
	// 変数宣言
	std::unique_ptr<CUi> pUi(new CUi);
	// UIタイプの設定
	pUi->m_Uitype = Uitype;
	// UIID設定
	pUi->m_nUi = nUi;
	// 初期化処理
	pUi->Init();
	return pUi;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだものの作成(シーン管理)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::VEC_P_UI CUi::LoadCreate(UITYPE const &Uitype)
{
	// 使用状態がtrueなら抜ける
	if (m_sta_UiUse[Uitype])
	{
		return VEC_P_UI();
	}
	// 変数宣言
	VEC_P_UI vec_pUi;	// UI情報格納用変数
	for (int nCntUi = 0; nCntUi < (signed)m_vec_UiLoad[Uitype].size(); nCntUi++)
	{
		// 作成処理(シーン管理)
		vec_pUi.push_back(Create(Uitype, nCntUi));
	}
	// 使用状態をtrueへ
	m_sta_UiUse[Uitype] = true;
	// UI情報を渡す
	return vec_pUi;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだものの作成(個人管理)
// 返すときはstd::moveを使用すること
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CUi::VEC_UNI_UI CUi::LoadCreate_Self(UITYPE const & Uitype)
{
	// 変数宣言
	VEC_UNI_UI vec_pUi;	// UI情報格納用変数
	// 使用状態がtrueなら抜ける
	if (m_sta_UiUse[Uitype])
	{
		return vec_pUi;
	}
	for (int nCntUi = 0; nCntUi < (signed)m_vec_UiLoad[Uitype].size(); nCntUi++)
	{
		// 作成処理(個人管理)
		vec_pUi.push_back(std::move(Create_Self(Uitype, nCntUi)));
	}
	// 使用状態をtrueへ
	m_sta_UiUse[Uitype] = true;
	// UI情報を渡す
	return vec_pUi;
}