// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3Dマップ処理の説明[3Dmap.cpp]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Dmap.h"
#include "manager.h"
#include "debugproc.h"
#include "floor.h"
#include "meshwall.h"
#include "scene_X.h"
#include "scene_three.h"
#include "solider.h"
#include "columncollision.h"
#include "rectcollision.h"
#include "spherecollision.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MANAGER_FILE ("data/LOAD/MAPPING/map_manager.txt")

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<std::vector<C3DMap::OBJECT>> C3DMap::m_vec_obj;
std::vector<std::vector<C3DMap::CHARACTER>> C3DMap::m_vec_char;
std::vector<std::vector<C3DMap::POLYGON>> C3DMap::m_vec_polygon;
std::vector<std::vector<C3DMap::FLOOR>> C3DMap::m_vec_floor;
std::vector<std::vector<C3DMap::WALL>> C3DMap::m_vec_wall;
std::vector<std::vector<C3DMap::COLLISION>> C3DMap::m_vec_Collision;
std::vector<std::string> C3DMap::m_vec_String;

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DMap::C3DMap()
{
	m_nType = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DMap::~C3DMap()
{
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::Debug(void)
{
	ImGui::Begin("3DMap::Collision");
	for (int nCntCollision = 0; nCntCollision < (signed)m_vec_Collision[MAP_STAGE_1].size(); nCntCollision++)
	{
		std::string sNum = "Collision[" + std::to_string(nCntCollision) + "]";

		if (ImGui::TreeNode(sNum.c_str()))
		{
			ImGui::DragFloat3("pos", m_vec_Collision[MAP_STAGE_1][nCntCollision].pos);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
#endif // _DEBUG

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだ情報を生成
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::LoadCreate(MAP const &map)
{
	// 変数宣言
	int nCntMap = 0;
	// 読み込んだ情報の生成
	// シーンX
	for (nCntMap = 0; nCntMap < (signed)m_vec_obj[map].size(); nCntMap++)
	{
		// シーンXの生成
		CScene_X::Create(
			m_vec_obj[map][nCntMap].pos,
			m_vec_obj[map][nCntMap].rot,
			m_vec_obj[map][nCntMap].nModelType,
			true
		);
	}
	// 敵キャラクターの生成
	for (nCntMap = 0; nCntMap < (signed)m_vec_char[map].size(); nCntMap++)
	{
		// 敵1
		if (m_vec_char[map][nCntMap].nCharacter == CCharacter::CHARACTER_NPC)
		{
				CSolider::Create(m_vec_char[map][nCntMap].pos,
				m_vec_char[map][nCntMap].rot
			);
		}
	}
	// ポリゴン
	for (nCntMap = 0; nCntMap < (signed)m_vec_polygon[map].size(); nCntMap++)
	{
		// シーンXの生成
		CScene_THREE::Create(
			CScene_THREE::OFFSET_TYPE_VERTICAL_CENTER,
			m_vec_polygon[map][nCntMap].pos,
			D3DXVECTOR3(m_vec_polygon[map][nCntMap].size.x, m_vec_polygon[map][nCntMap].size.y, 0.0f),
			m_vec_polygon[map][nCntMap].nTexType,
			m_vec_polygon[map][nCntMap].rot
		);
	}
	// 床
	for (nCntMap = 0; nCntMap < (signed)m_vec_floor[map].size(); nCntMap++)
	{
		// 床の生成
		CFloor::Create(
			m_vec_floor[map][nCntMap].pos,
			D3DXVECTOR3(m_vec_floor[map][nCntMap].size.x,0.0f, m_vec_floor[map][nCntMap].size.y),
			D3DXCOLOR_INI,
			m_vec_floor[map][nCntMap].rot,
			m_vec_floor[map][nCntMap].nBlockWidth,
			m_vec_floor[map][nCntMap].nBlockDepth,
			m_vec_floor[map][nCntMap].nTexType
			);
	}
	// 壁
	for (nCntMap = 0; nCntMap < (signed)m_vec_wall[map].size(); nCntMap++)
	{
		// 壁の生成
		CMeshwall::Create(
			m_vec_wall[map][nCntMap].pos,
			D3DXVECTOR3(m_vec_wall[map][nCntMap].size.x, m_vec_wall[map][nCntMap].size.y,0.0f),
			m_vec_wall[map][nCntMap].rot,
			m_vec_wall[map][nCntMap].nBlockWidth,
			m_vec_wall[map][nCntMap].nBlockDepth,
			m_vec_wall[map][nCntMap].nTexType
			);
	}
	// 当たり判定の生成
	for (nCntMap = 0; nCntMap < (signed)m_vec_Collision[map].size(); nCntMap++)
	{
		// 矩形情報のNULLチェック
		if (m_vec_Collision[map][nCntMap].uni_Rect != NULL)
		{
			// 矩形の生成
			CRectCollision::Create(
				m_vec_Collision[map][nCntMap].uni_Rect->size,
				D3DVECTOR3_ZERO,
				(CCollision::OBJTYPE)m_vec_Collision[map][nCntMap].nObjType,
				NULL,
				NULL,
				false,
				m_vec_Collision[map][nCntMap].bPush,
				&m_vec_Collision[map][nCntMap].pos
			);
		}
		// 球情報のNULLチェック
		else if (m_vec_Collision[map][nCntMap].uni_Sphere != NULL)
		{
			// 球の生成
			CSphereCollision::Create(
				m_vec_Collision[map][nCntMap].uni_Sphere->fRadius,
				D3DVECTOR3_ZERO,
				(CCollision::OBJTYPE)m_vec_Collision[map][nCntMap].nObjType,
				NULL,
				NULL,
				false,
				m_vec_Collision[map][nCntMap].bPush,
				&m_vec_Collision[map][nCntMap].pos
			);
		}
		// 円柱情報のNULLチェック
		else if (m_vec_Collision[map][nCntMap].uni_Column != NULL)
		{
			// 円柱の生成
			CColumnCollision::Create(
				m_vec_Collision[map][nCntMap].uni_Column->fRadius,
				m_vec_Collision[map][nCntMap].uni_Column->fVertical,
				D3DVECTOR3_ZERO,
				(CCollision::OBJTYPE)m_vec_Collision[map][nCntMap].nObjType,
				NULL,
				NULL,
				false,
				m_vec_Collision[map][nCntMap].bPush,
				&m_vec_Collision[map][nCntMap].pos
			);
		}
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// スクリプト読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::LoadScript(char *add)
{
	FILE *pFile = NULL;																	// ファイル
	char cReadText[128];															// 文字
	char cHeadText[128];															// 比較
	char cDie[128];

	pFile = fopen(add, "r");				// ファイルを開くまたは作る

	if (pFile != NULL)						//ファイルが読み込めた場合
	{
		//スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//エンドスクリプトが来るまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//改行
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "FIELDSET") == 0)
					{//キャラクターの初期情報のとき
						CFloor *pField;
						pField = CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(50.0f, 50.0f, 50.0f),
							D3DXCOLOR_INI,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							50,
							50,
							7);

						if (pField != NULL)
						{
							//エンドキャラクターセットが来るまでループ
							while (strcmp(cHeadText, "END_FIELDSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								if (strcmp(cHeadText, "POS") == 0)
								{//パーツ総数のとき
									D3DXVECTOR3 pos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pos.x,
										&pos.y,
										&pos.z);

									// 位置の設定
									pField->Scene_SetPos(pos);
								}
								else if (strcmp(cHeadText, "CHECK_HEIGHT") == 0)
								{// 計算する必要のあるメッシュかどうか
								 //int nCal;
								 //sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nCal);
								 //pField->SetCalculation((nCal == 0) ? true : false);
									pField->SetCalculation(true);
								}
								else if (strcmp(cHeadText, "VERTEXINFO") == 0)
								{//パーツ総数のとき
									pField->vertexMove(pFile);			// 頂点情報の作成(ファイルから)
								}
							}
						}
					}
				}
			}
		}
		fclose(pFile);				// ファイルを閉じる
	}
	else
	{
		MessageBox(NULL, "地面情報のアクセス失敗！", "WARNING", MB_ICONWARNING);	// メッセージボックスの生成
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// リソース情報読み込む設定
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::Load(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();
	// マネージャー読み込み
	ManagerLoad();
	// スクリプト読み込み
	ScriptLoad();

	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マネージャー読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::ManagerLoad(void)
{
	// 変数宣言
	// ファイルの中身格納用
	std::vector<std::vector<std::string>> vsvec_Contens;
	// ファイルの中身を取得する
	vsvec_Contens = CCalculation::FileContens(MANAGER_FILE, '\0');
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
	// ファイルの中身情報の開放
	for (int nCntLine = 0; nCntLine < (signed)vsvec_Contens.size(); nCntLine++)
	{
		vsvec_Contens[nCntLine].clear();
		vsvec_Contens[nCntLine].shrink_to_fit();
	}
	vsvec_Contens.clear();
	vsvec_Contens.shrink_to_fit();
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// スクリプト読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DMap::ScriptLoad(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	char cRaedText[128] = {};	// 文字として読み取り用
	char cHeadText[128] = {};	// 比較するよう
	char cDie[128] = {};			// 不必要な文字
	int nCntError = 0;		// エラーカウント
	int nCntObj,			// オブジェカウント
		nCntChar,			// キャラクターカウント
		nCntPoly,			// ポリゴンカウント
		nCntFloor,			// 床カウント
		nCntWall,			// 壁カウント
		nCntCollision = 0;	// 当たり判定カウント
	// 情報を保存
	for (int nCntScript = 0; nCntScript < (signed)m_vec_String.size(); nCntScript++)
	{
		// 変数宣言
		std::vector<OBJECT> vec_obj;
		std::vector<CHARACTER> vec_char;
		std::vector<POLYGON> vec_bill;
		std::vector<FLOOR> vec_floor;
		std::vector<WALL> vec_wall;
		std::vector<COLLISION> vec_collision;
		nCntObj = 0;
		nCntChar = 0;
		nCntPoly = 0;
		nCntFloor = 0;
		nCntWall = 0;
		nCntCollision = 0;
		// ファイルが開かれていなかったら
		if ((pFile = fopen(m_vec_String[nCntScript].c_str(), "r")) == NULL)
		{// メッセージの表示
#ifdef _DEBUG
			std::string sErrow = m_vec_String[nCntScript] + "が見つかりません";
			CCalculation::Messanger(sErrow.c_str());
#endif // _DEBUG
			return E_FAIL;
		}
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
														// ファイルのパス情報読み込み
			// エラーカウントをインクリメント
			nCntError++;
			if (nCntError > FILELINE_ERROW)
			{// エラー
				nCntError = 0;
				fclose(pFile);
				CCalculation::Messanger("スタティックオブジェクトのスクリプトがありません");
				return E_FAIL;
			}
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}
				// オブジェクトが来たら
				else if (strcmp(cHeadText, "OBJECTSET") == 0)
				{
					// オブジェクト追加
					vec_obj.push_back(OBJECT());
					// エンドオブジェクトセットが来るまでループ
					while (strcmp(cHeadText, "END_OBJECTSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "MODELTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_obj[nCntObj].nModelType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_obj[nCntObj].pos.x,
								&vec_obj[nCntObj].pos.y,
								&vec_obj[nCntObj].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_obj[nCntObj].rot.x,
								&vec_obj[nCntObj].rot.y,
								&vec_obj[nCntObj].rot.z);
						}
						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// エラー
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("エンドオブジェクトセットがありません");
							return E_FAIL;
						}
					}
					// カウントオブジェ
					nCntObj++;
				}
				// キャラクターが来たら
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					// キャラクター追加
					vec_char.push_back(CHARACTER());
					// エンドキャラクターセットが来るまでループ
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "CHARACTERTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_char[nCntChar].nCharacter);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_char[nCntChar].pos.x,
								&vec_char[nCntChar].pos.y,
								&vec_char[nCntChar].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_char[nCntChar].rot.x,
								&vec_char[nCntChar].rot.y,
								&vec_char[nCntChar].rot.z);
						}
						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// エラー
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("エンドキャラクターセットがありません");
							return E_FAIL;
						}
					}
					// カウントキャラ
					nCntChar++;
				}
				// ポリゴンが来たら
				else if (strcmp(cHeadText, "POLYGONSET") == 0)
				{
					// ポリゴン追加
					vec_bill.push_back(POLYGON());
					// エンドポリゴンセットが来るまでループ
					while (strcmp(cHeadText, "END_POLYGONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// テクスチャータイプ情報読み込み
						if (strcmp(cHeadText, "TEXTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_bill[nCntPoly].nTexType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_bill[nCntPoly].pos.x,
								&vec_bill[nCntPoly].pos.y,
								&vec_bill[nCntPoly].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_bill[nCntPoly].rot.x,
								&vec_bill[nCntPoly].rot.y,
								&vec_bill[nCntPoly].rot.z);
						}
						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
								&vec_bill[nCntPoly].size.x,
								&vec_bill[nCntPoly].size.y
							);
						}
						// ビルボードをかけるかかけないか情報読み込み
						else if (strcmp(cHeadText, "BILLBOARD") == 0)
						{
							vec_bill[nCntPoly].bBillboard = true;
						}
						// Zバッファをかけるかかけないか情報読み込み
						else if (strcmp(cHeadText, "ZENEBLE") == 0)
						{
							vec_bill[nCntPoly].bZEneble = true;
						}
						// ライティングをかけるかかけないか情報読み込み
						else if (strcmp(cHeadText, "LIGHTING") == 0)
						{
							vec_bill[nCntPoly].bLighting = true;
						}
						// アルファブレンドをかけるかかけないか情報読み込み
						else if (strcmp(cHeadText, "ALPHA") == 0)
						{
							vec_bill[nCntPoly].bAlpha = true;
						}

						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// エラー
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("エンドポリゴンセットがありません");
							return E_FAIL;
						}
					}
					// カウントポリゴン
					nCntPoly++;
				}
				// 床が来たら
				else if (strcmp(cHeadText, "FLOORSET") == 0)
				{
					// 床追加
					vec_floor.push_back(FLOOR());
					// エンド床セットが来るまでループ
					while (strcmp(cHeadText, "END_FLOORSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "TEXTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_floor[nCntFloor].nTexType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_floor[nCntFloor].pos.x,
								&vec_floor[nCntFloor].pos.y,
								&vec_floor[nCntFloor].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_floor[nCntFloor].rot.x,
								&vec_floor[nCntFloor].rot.y,
								&vec_floor[nCntFloor].rot.z);
						}
						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_floor[nCntFloor].nBlockDepth
							);
						}
						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_floor[nCntFloor].nBlockWidth
							);
						}
						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f", &cDie, &cDie,
								&vec_floor[nCntFloor].size.x,
								&vec_floor[nCntFloor].size.y
							);
						}

						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// エラー
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("エンド床セットがありません");
							return E_FAIL;
						}
					}
					// カウント床
					nCntFloor++;
				}
				// 壁が来たら
				else if (strcmp(cHeadText, "WALLSET") == 0)
				{
					// 壁追加
					vec_wall.push_back(WALL());
					// エンド壁セットが来るまでループ
					while (strcmp(cHeadText, "END_WALLSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "TEXTYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &vec_wall[nCntWall].nTexType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_wall[nCntWall].pos.x,
								&vec_wall[nCntWall].pos.y,
								&vec_wall[nCntWall].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_wall[nCntWall].rot.x,
								&vec_wall[nCntWall].rot.y,
								&vec_wall[nCntWall].rot.z);
						}
						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_wall[nCntWall].nBlockDepth
							);
						}
						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_wall[nCntWall].nBlockWidth
							);
						}
						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f",
								&cDie, &cDie,
								&vec_wall[nCntWall].size.x,
								&vec_wall[nCntWall].size.y
							);
						}

						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// エラー
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("エンド壁セットがありません");
							return E_FAIL;
						}
					}
					// カウント壁
					nCntWall++;
				}
				// 当たり判定
				else if (strcmp(cHeadText, "COLLISIONSET") == 0)
				{
					// 当たり判定追加
					vec_collision.push_back(COLLISION());
					// エンド当たり判定セットが来るまでループ
					while (strcmp(cHeadText, "END_COLLISIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&vec_collision[nCntCollision].pos.x,
								&vec_collision[nCntCollision].pos.y,
								&vec_collision[nCntCollision].pos.z);
						}
						// オブジェクトタイプ情報読み込み
						else if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&vec_collision[nCntCollision].nObjType);
						}
						// 押し出し情報読み込み
						else if (strcmp(cHeadText, "PUSH") == 0)
						{
							vec_collision[nCntCollision].bPush = true;
						}
						// 矩形柱情報が来たら
						else if (strcmp(cHeadText, "RECT") == 0)
						{
							// 矩形情報のメモリ確保
							vec_collision[nCntCollision].uni_Rect.reset(new RECT);
							// エンド矩形セットが来るまでループ
							while (strcmp(cHeadText, "END_RECT") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// 半径情報読み込み
								if (strcmp(cHeadText, "SIZE") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Rect->size.x,
										&vec_collision[nCntCollision].uni_Rect->size.y,
										&vec_collision[nCntCollision].uni_Rect->size.z);
								}
								// 回転情報読み込み
								else if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Rect->rot.x,
										&vec_collision[nCntCollision].uni_Rect->rot.y,
										&vec_collision[nCntCollision].uni_Rect->rot.z);
								}
							}
						}
						// 球柱情報が来たら
						else if (strcmp(cHeadText, "SPHERE") == 0)
						{
							// 球情報のメモリ確保
							vec_collision[nCntCollision].uni_Sphere.reset(new SPHERE);
							// エンド球セットが来るまでループ
							while (strcmp(cHeadText, "END_SPHERE") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// 半径情報読み込み
								if (strcmp(cHeadText, "RADIUS") == 0)
								{
									sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Sphere->fRadius);
								}
							}
						}
						// 円柱情報が来たら
						else if (strcmp(cHeadText, "COLUMN") == 0)
						{
							// 円柱情報のメモリ確保
							vec_collision[nCntCollision].uni_Column.reset(new COLUMN);
							// エンド円柱セットが来るまでループ
							while (strcmp(cHeadText, "END_COLUMN") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);
								// 回転情報読み込み
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Column->rot.x,
										&vec_collision[nCntCollision].uni_Column->rot.y,
										&vec_collision[nCntCollision].uni_Column->rot.z);
								}
								// 半径情報読み込み
								else if (strcmp(cHeadText, "RADIUS") == 0)
								{
									sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Column->fRadius);
								}
								// 縦情報読み込み
								else if (strcmp(cHeadText, "VERTICAL") == 0)
								{
									sscanf(cRaedText, "%s %s %f", &cDie, &cDie,
										&vec_collision[nCntCollision].uni_Column->fVertical);
								}
							}
						}

						// エラーカウントをインクリメント
						nCntError++;
						if (nCntError > FILELINE_ERROW)
						{// エラー
							nCntError = 0;
							fclose(pFile);
							CCalculation::Messanger("エンド当たり判定セットがありません");
							return E_FAIL;
						}
					}
					// カウント当たり判定
					nCntCollision++;
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
			// オブジェクトの格納
			m_vec_obj.push_back(std::move(vec_obj));
			// キャラクターの格納
			m_vec_char.push_back(std::move(vec_char));
			// ポリゴンの格納
			m_vec_polygon.push_back(std::move(vec_bill));
			// 床の格納
			m_vec_floor.push_back(std::move(vec_floor));
			// 壁の格納
			m_vec_wall.push_back(std::move(vec_wall));
			// 当たり判定の格納
			m_vec_Collision.push_back(std::move(vec_collision));
		}
		// ファイル閉
		fclose(pFile);
	}
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだリソース情報を開放する
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::UnLoad(void)
{
	// オブジェクト情報の開放
	m_vec_obj.clear();
	m_vec_obj.shrink_to_fit();
	// キャラクター情報の開放
	m_vec_char.clear();
	m_vec_char.shrink_to_fit();
	// ポリゴン情報の開放
	m_vec_polygon.clear();
	m_vec_polygon.shrink_to_fit();
	// 床情報の開放
	m_vec_floor.clear();
	m_vec_floor.shrink_to_fit();
	// 壁情報の開放
	m_vec_wall.clear();
	m_vec_wall.shrink_to_fit();
	// 当たり判定の各形状情報の開放
	for (int nCntMap = 0; nCntMap < (signed)m_vec_Collision.size(); nCntMap++)
	{
		for (int nCntCollision = 0; nCntCollision < (signed)m_vec_Collision[nCntMap].size(); nCntCollision++)
		{
			// 矩形情報のNULLチェック
			if (m_vec_Collision[nCntMap][nCntCollision].uni_Rect != NULL)
			{
				// 矩形情報の開放
				m_vec_Collision[nCntMap][nCntCollision].uni_Rect.reset();
				m_vec_Collision[nCntMap][nCntCollision].uni_Rect = NULL;
			}
			// 球情報のNULLチェック
			else if (m_vec_Collision[nCntMap][nCntCollision].uni_Sphere != NULL)
			{
				// 球情報の開放
				m_vec_Collision[nCntMap][nCntCollision].uni_Sphere.reset();
				m_vec_Collision[nCntMap][nCntCollision].uni_Sphere = NULL;
			}
			// 円柱情報のNULLチェック
			else if (m_vec_Collision[nCntMap][nCntCollision].uni_Column != NULL)
			{
				// 円柱情報の開放
				m_vec_Collision[nCntMap][nCntCollision].uni_Column.reset();
				m_vec_Collision[nCntMap][nCntCollision].uni_Column = NULL;
			}

		}
	}
	// 当たり判定情報の開放
	m_vec_Collision.clear();
	m_vec_Collision.shrink_to_fit();
	// マネージャーマップの情報の開放
	m_vec_String.clear();
	m_vec_String.shrink_to_fit();
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// キャラクター生成
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DMap::CharaCreate(void)
{
}
