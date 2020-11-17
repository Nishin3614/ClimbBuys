// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル情報処理 [model_info.cpp]
// Author : KOKI NISHIYAMA
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "model_info.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::CModel_info()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::~CModel_info()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルのテキストデータの読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CModel_info::TextLoad(
	MODEL_ALL *pmodelAll,
	std::vector<int> &ModelFile,
	int const &nMaxMotion,
	int &nMaxkey,
	int &nMaxParts,
	const char * file_name
)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;				// モデルカウント
	int	nCntMotion = 0;				// モーションカウント
	int	nCntKeySet = 0;				// フレーム数
	int	nCntKey = 0;				// モデル数
	char cRaedText[128];			// 文字として読み取り用
	char cHeadText[128];			// 比較するよう
	char cDie[128];					// 不必要な文字
	int nCameraType = 0;			// カメラタイプ
	int nBeginBlust = 0;			// 初期の吹っ飛び方
	int nCntObit_Basic = 0;			// 軌跡基本カウント
	int nCntObit_Detail = 0;		// 軌跡詳細カウント
	int nCntAttackCollision = 0;	// 攻撃当たり判定カウント
	int nCntCollision = 0;			// 当たり判定カウント
	MOTION_EFFECT motion_effect;	// モーションエフェクト情報格納

	//pmodelAll->pMotion = new MOTION[nMaxMotion];
	// ファイル開
	pFile = fopen(file_name, "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// エンドスクリプトが来るまでループ
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// モデルの個数
			if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxkey);
			}
			// モデルファイル名が来たら
			else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
			{
				// 変数宣言
				int nModelId;
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nModelId);
				// モデル番号格納
				ModelFile.push_back(nModelId);
			}
			// 軌跡セットが来たら
			else if (strcmp(cHeadText, "OBIT_PREPARATION") == 0)
			{
				// 軌跡情報の確保
				pmodelAll->v_MeshObitLoad.push_back(MESHOBIT_BASIC());
				// エンド軌跡セットが来るまでループ
				while (strcmp(cHeadText, "END_OBIT_PREPARATION") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// 線の数情報読み込み
					if (strcmp(cHeadText, "LINE") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nLine);
					}
					// パーツ情報読み込み
					else if (strcmp(cHeadText, "PARTS") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nPart);
					}
					// 始点オフセット情報読み込み
					else if (strcmp(cHeadText, "BEGINOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.z);
					}
					// 終点オフセット情報読み込み
					else if (strcmp(cHeadText, "ENDOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.z);
					}
					// テクスチャータイプ情報読み込み
					else if (strcmp(cHeadText, "TEXTUREID") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nTexType);
					}
				}
				// 軌跡カウントの更新
				nCntObit_Basic++;
			}
			// モデルのオフセットパーツが来たら
			else if (strcmp(cHeadText, "NUM_PARTS") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxParts);
				// 動かすモデル数分メモリ確保
				pmodelAll->pModel_offset = new MODEL_OFFSET[nMaxParts];
			}
			// パーツセットが来たら
			else if (strcmp(cHeadText, "PARTSSET") == 0)
			{
				// エンドパーツセットが来るまでループ
				while (strcmp(cHeadText, "END_PARTSSET") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// 親情報読み込み
					if (strcmp(cHeadText, "PARENT") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].nParent);
					}
					// 位置情報読み込み
					else if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].pos.x,
							&pmodelAll->pModel_offset[nCntModel].pos.y,
							&pmodelAll->pModel_offset[nCntModel].pos.z);
					}
					// 回転情報読み込み
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].rot.x,
							&pmodelAll->pModel_offset[nCntModel].rot.y,
							&pmodelAll->pModel_offset[nCntModel].rot.z);
					}
				}
				// モデルの更新
				nCntModel++;
			}
			// モーションセットが来たら
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// カメラタイプの初期化
				nCameraType = 0;	// カメラタイプ
				// モデルの初期化
				nCntKeySet = 0;
				// モーションの生成
				pmodelAll->pMotion.push_back(new MOTION);
				// エンドモーションセットが来るまでループ
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
#ifdef _DEBUG
					// モーションの最大数を超えてしまった場合
					// メッセージを知らせる
					if (nCntMotion >= nMaxMotion)
					{
						CCalculation::Messanger("model_infoのTextLoad関数->モーションの最大数を超えてしまいました。");
					}
#endif // _DEBUG

					// 初期化
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// ループするかどうか情報読み込み
					if (strcmp(cHeadText, "LOOP") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion]->nLoop);
					}
					// キー数情報読み込み
					else if (strcmp(cHeadText, "NUM_KEY") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pmodelAll->pMotion[nCntMotion]->nNumKey);
						// モーション数生成
						pmodelAll->pMotion[nCntMotion]->KeyInfo = new KEY_INFO[pmodelAll->pMotion[nCntMotion]->nNumKey];
					}
					// キー数情報読み込み
					else if (strcmp(cHeadText, "CAMERAINFO") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nCameraType);
					}
					// 吹っ飛び方情報読み込み
					else if (strcmp(cHeadText, "BEGINBLUST") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nBeginBlust);
					}
					// 軌跡の詳細情報読み込み
					else if (strcmp(cHeadText, "OBIT_DETAIL") == 0)
					{
						// 軌跡の詳細の格納
						pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.push_back(MESHOBIT_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nObitID
						);
						// エンドキーが来るまでループ
						while (strcmp(cHeadText, "END_OBIT_DETAIL") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// フレーム情報読み込み
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nBeginFram,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nEndFram
								);
							}
							// 始点色情報読み込み
							else if (strcmp(cHeadText, "BEGINCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.a
								);
							}
							// 終点色情報読み込み
							else if (strcmp(cHeadText, "ENDCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.a
								);
							}
						}
						// 軌跡詳細カウントアップ
						nCntObit_Detail++;
					}
					// 当たり判定詳細情報読み込み
					else if (strcmp(cHeadText, "COLLISION_DETAIL") == 0)
					{
						// 当たり判定詳細の格納
						pmodelAll->pMotion[nCntMotion]->v_Collision.push_back(COLLISION_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nCollisionID
						);
						// エンド当たり判定詳細が来るまでループ
						while (strcmp(cHeadText, "END_COLLISION_DETAIL") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);


							// 倍率情報読み込み
							if (strcmp(cHeadText, "DOUBLE") == 0)
							{
								sscanf(cRaedText, "%s %s %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).fDouble
								);
							}
							// フレーム情報読み込み
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nBeginFrame,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nEndFrame
								);
							}
							// ダメージ情報読み込み
							else if (strcmp(cHeadText, "DAMAGE") == 0)
							{
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nDamage
								);
							}
						}
						// 軌跡詳細カウントアップ
						nCntCollision++;
					}
					// キー情報読み込み
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// キーの初期化
						nCntKey = 0;
						// 軌跡の詳細の初期化
						nCntObit_Detail = 0;
						// 当たり判定カウントの初期化
						nCntCollision = 0;
						// モデル数生成
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
						// 吹っ飛び方初期設定
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust = nBeginBlust;
						// エンドキーセットが来るまでループ
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// 攻撃当たり判定の頻度情報読み込み
							if (strcmp(cHeadText, "NUMCOLLISION") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision);
							}
							// カメラ情報読み取り
							else if (strcmp(cHeadText, "CAMERA") == 0)
							{
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera = new MOTION_CAMERA;
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_CAMERA") != 0)
								{
									// 初期化
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);
									// 注視点のオフセット情報読み込み
									if (strcmp(cHeadText, "OFFSET_R") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.z);
									}
									// 長さ情報読み込み
									else if (strcmp(cHeadText, "LENGTH") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fLength);
									}
									// 高さ情報読み込み
									else if (strcmp(cHeadText, "HEIGHT") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fHeight);
									}
									// 回転情報読み込み
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.z);
									}
									// 慣性情報読み込み
									else if (strcmp(cHeadText, "INTERTIA") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fIntertia);
									}
								}
							}
							// エフェクト情報読み取り
							else if (strcmp(cHeadText, "EFFECT") == 0)
							{
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_EFFECT") != 0)
								{
									// 初期化
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// オフセット情報読み込み
									if (strcmp(cHeadText, "OFFSET") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&motion_effect.offset.x,
											&motion_effect.offset.y,
											&motion_effect.offset.z);
									}
									// 開始・終了時間情報読み込み
									else if (strcmp(cHeadText, "START_END") == 0)
									{
										sscanf(cRaedText, "%s %s %d %d",
											&cDie, &cDie,
											&motion_effect.nStart,
											&motion_effect.nEnd
										);
									}
									// パーツ番号情報読み込み
									else if (strcmp(cHeadText, "PARTS") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nKeyID
										);
									}
									// パーティクル番号情報読み込み
									else if (strcmp(cHeadText, "PARTICLE") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nParticleType
										);
									}
								}
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].v_MotionEffect.push_back(motion_effect);
							}

							// 吹っ飛び方情報読み込み
							else if (strcmp(cHeadText, "BLUST") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust);
							}

							// フレーム情報読み込み
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame);
								// モーション全体のフレーム数加算
								pmodelAll->pMotion[nCntMotion]->nAllFrame +=
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								// 当たり判定の回数がない場合関数を抜ける
								if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision > 0)
								{
									// 攻撃判定頻度
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime =
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame /
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision;
								}
								else
								{
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime = pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								}

							}
							// キー情報読み込み
							else if (strcmp(cHeadText, "KEY") == 0)
							{
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									// 初期化
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// 位置情報読み込み
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

										// パーツの位置情報を加える
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos += pmodelAll->pModel_offset[nCntKey].pos;
									}

									// 回転情報読み込み
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
										// パーツの位置情報を加える
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot += pmodelAll->pModel_offset[nCntKey].rot;
									}
								}
								// カウントキー
								nCntKey++;
							}
						}
						// カウントモデル
						nCntKeySet++;
					}
				}
				// カウントプレイヤー
				nCntMotion++;
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "モデルデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

#ifdef _DEBUG
	// モーションの最大数を超えてしまった場合
	// メッセージを知らせる
	if (nCntMotion < nMaxMotion)
	{
		CCalculation::Messanger("model_infoのTextLoad関数->モーションの数が足りません。");
	}
#endif // _DEBUG
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルのテキストデータの読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CModel_info::TextLoad(
	MODEL_ALL *pmodelAll,
	std::vector<int> &ModelFile,
	int &nMaxkey,
	int &nMaxParts,
	const char * file_name
)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;				// モデルカウント
	int	nCntMotion = 0;				// モーションカウント
	int	nCntKeySet = 0;				// フレーム数
	int	nCntKey = 0;				// モデル数
	char cRaedText[128];			// 文字として読み取り用
	char cHeadText[128];			// 比較するよう
	char cDie[128];					// 不必要な文字
	int nCameraType = 0;			// カメラタイプ
	int nBeginBlust = 0;			// 初期の吹っ飛び方
	int nCntObit_Basic = 0;			// 軌跡基本カウント
	int nCntObit_Detail = 0;		// 軌跡詳細カウント
	int nCntAttackCollision = 0;	// 攻撃当たり判定カウント
	int nCntCollision = 0;			// 当たり判定カウント
	MOTION_EFFECT motion_effect;	// モーションエフェクト情報格納

	// ファイル開
	pFile = fopen(file_name, "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// エンドスクリプトが来るまでループ
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			cRaedText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// モデルの個数
			if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxkey);
			}
			// モデルファイル名が来たら
			else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
			{
				// 変数宣言
				int nModelId;
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nModelId);
				// モデル番号格納
				ModelFile.push_back(nModelId);
			}
			// 軌跡セットが来たら
			else if (strcmp(cHeadText, "OBIT_PREPARATION") == 0)
			{
				// 軌跡情報の確保
				pmodelAll->v_MeshObitLoad.push_back(MESHOBIT_BASIC());
				// エンド軌跡セットが来るまでループ
				while (strcmp(cHeadText, "END_OBIT_PREPARATION") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// 線の数情報読み込み
					if (strcmp(cHeadText, "LINE") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nLine);
					}
					// パーツ情報読み込み
					else if (strcmp(cHeadText, "PARTS") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nPart);
					}
					// 始点オフセット情報読み込み
					else if (strcmp(cHeadText, "BEGINOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).BeginOffset.z);
					}
					// 終点オフセット情報読み込み
					else if (strcmp(cHeadText, "ENDOFFSET") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.x,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.y,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).EndOffset.z);
					}
					// テクスチャータイプ情報読み込み
					else if (strcmp(cHeadText, "TEXTUREID") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->v_MeshObitLoad.at(nCntObit_Basic).nTexType);
					}
				}
				// 軌跡カウントの更新
				nCntObit_Basic++;
			}
			// モデルのオフセットパーツが来たら
			else if (strcmp(cHeadText, "NUM_PARTS") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxParts);
				// 動かすモデル数分メモリ確保
				pmodelAll->pModel_offset = new MODEL_OFFSET[nMaxParts];
			}
			// パーツセットが来たら
			else if (strcmp(cHeadText, "PARTSSET") == 0)
			{
				// エンドパーツセットが来るまでループ
				while (strcmp(cHeadText, "END_PARTSSET") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// 親情報読み込み
					if (strcmp(cHeadText, "PARENT") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].nParent);
					}
					// 位置情報読み込み
					else if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].pos.x,
							&pmodelAll->pModel_offset[nCntModel].pos.y,
							&pmodelAll->pModel_offset[nCntModel].pos.z);
					}
					// 回転情報読み込み
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].rot.x,
							&pmodelAll->pModel_offset[nCntModel].rot.y,
							&pmodelAll->pModel_offset[nCntModel].rot.z);
					}
				}
				// モデルの更新
				nCntModel++;
			}
			// モーションセットが来たら
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// カメラタイプの初期化
				nCameraType = 0;	// カメラタイプ
									// モデルの初期化
				nCntKeySet = 0;
				// モーションの生成
				pmodelAll->pMotion.push_back(new MOTION);
				// エンドモーションセットが来るまでループ
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					cRaedText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// ループするかどうか情報読み込み
					if (strcmp(cHeadText, "LOOP") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion]->nLoop);
					}
					// キー数情報読み込み
					else if (strcmp(cHeadText, "NUM_KEY") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pmodelAll->pMotion[nCntMotion]->nNumKey);
						// モーション数生成
						pmodelAll->pMotion[nCntMotion]->KeyInfo = new KEY_INFO[pmodelAll->pMotion[nCntMotion]->nNumKey];
					}
					// キー数情報読み込み
					else if (strcmp(cHeadText, "CAMERAINFO") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nCameraType);
					}
					// 吹っ飛び方情報読み込み
					else if (strcmp(cHeadText, "BEGINBLUST") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nBeginBlust);
					}
					// 軌跡の詳細情報読み込み
					else if (strcmp(cHeadText, "OBIT_DETAIL") == 0)
					{
						// 軌跡の詳細の格納
						pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.push_back(MESHOBIT_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nObitID
						);
						// エンドキーが来るまでループ
						while (strcmp(cHeadText, "END_OBIT_DETAIL") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// フレーム情報読み込み
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nBeginFram,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).nEndFram
								);
							}
							// 始点色情報読み込み
							else if (strcmp(cHeadText, "BEGINCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).BeginCol.a
								);
							}
							// 終点色情報読み込み
							else if (strcmp(cHeadText, "ENDCOL") == 0)
							{
								sscanf(cRaedText, "%s %s %f %f %f %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.r,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.g,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.b,
									&pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.at(nCntObit_Detail).EndCol.a
								);
							}
						}
						// 軌跡詳細カウントアップ
						nCntObit_Detail++;
					}
					// 当たり判定詳細情報読み込み
					else if (strcmp(cHeadText, "COLLISION_DETAIL") == 0)
					{
						// 当たり判定詳細の格納
						pmodelAll->pMotion[nCntMotion]->v_Collision.push_back(COLLISION_DETAILS());
						sscanf(cRaedText, "%s %d",
							&cDie,
							&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nCollisionID
						);
						// エンド当たり判定詳細が来るまでループ
						while (strcmp(cHeadText, "END_COLLISION_DETAIL") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);


							// 倍率情報読み込み
							if (strcmp(cHeadText, "DOUBLE") == 0)
							{
								sscanf(cRaedText, "%s %s %f",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).fDouble
								);
							}
							// フレーム情報読み込み
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cRaedText, "%s %s %d %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nBeginFrame,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nEndFrame
								);
							}
							// ダメージ情報読み込み
							else if (strcmp(cHeadText, "DAMAGE") == 0)
							{
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->v_Collision.at(nCntCollision).nDamage
								);
							}
						}
						// 軌跡詳細カウントアップ
						nCntCollision++;
					}
					// キー情報読み込み
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// キーの初期化
						nCntKey = 0;
						// 軌跡の詳細の初期化
						nCntObit_Detail = 0;
						// 当たり判定カウントの初期化
						nCntCollision = 0;
						// モデル数生成
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
						// 吹っ飛び方初期設定
						pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust = nBeginBlust;
						// エンドキーセットが来るまでループ
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							cRaedText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// 攻撃当たり判定の頻度情報読み込み
							if (strcmp(cHeadText, "NUMCOLLISION") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision);
							}
							// カメラ情報読み取り
							else if (strcmp(cHeadText, "CAMERA") == 0)
							{
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera = new MOTION_CAMERA;
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_CAMERA") != 0)
								{
									// 初期化
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);
									// 注視点のオフセット情報読み込み
									if (strcmp(cHeadText, "OFFSET_R") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->offsetR.z);
									}
									// 長さ情報読み込み
									else if (strcmp(cHeadText, "LENGTH") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fLength);
									}
									// 高さ情報読み込み
									else if (strcmp(cHeadText, "HEIGHT") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fHeight);
									}
									// 回転情報読み込み
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->rot.z);
									}
									// 慣性情報読み込み
									else if (strcmp(cHeadText, "INTERTIA") == 0)
									{
										sscanf(cRaedText, "%s %s %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].pMotionCamera->fIntertia);
									}
								}
							}
							// エフェクト情報読み取り
							else if (strcmp(cHeadText, "EFFECT") == 0)
							{
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_EFFECT") != 0)
								{
									// 初期化
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// オフセット情報読み込み
									if (strcmp(cHeadText, "OFFSET") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&motion_effect.offset.x,
											&motion_effect.offset.y,
											&motion_effect.offset.z);
									}
									// 開始・終了時間情報読み込み
									else if (strcmp(cHeadText, "START_END") == 0)
									{
										sscanf(cRaedText, "%s %s %d %d",
											&cDie, &cDie,
											&motion_effect.nStart,
											&motion_effect.nEnd
										);
									}
									// パーツ番号情報読み込み
									else if (strcmp(cHeadText, "PARTS") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nKeyID
										);
									}
									// パーティクル番号情報読み込み
									else if (strcmp(cHeadText, "PARTICLE") == 0)
									{
										sscanf(cRaedText, "%s %s %d",
											&cDie, &cDie,
											&motion_effect.nParticleType
										);
									}
								}
								pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].v_MotionEffect.push_back(motion_effect);
							}

							// 吹っ飛び方情報読み込み
							else if (strcmp(cHeadText, "BLUST") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nBlust);
							}

							// フレーム情報読み込み
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame);
								// モーション全体のフレーム数加算
								pmodelAll->pMotion[nCntMotion]->nAllFrame +=
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								// 当たり判定の回数がない場合関数を抜ける
								if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision > 0)
								{
									// 攻撃判定頻度
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime =
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame /
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nNumCollision;
								}
								else
								{
									pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nMaxCollisiontime = pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].nFrame;
								}

							}
							// キー情報読み込み
							else if (strcmp(cHeadText, "KEY") == 0)
							{
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									// 初期化
									cHeadText[0] = '\0';
									cRaedText[0] = '\0';
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// 位置情報読み込み
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

										// パーツの位置情報を加える
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].pos += pmodelAll->pModel_offset[nCntKey].pos;
									}

									// 回転情報読み込み
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
											&pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
										// パーツの位置情報を加える
										pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeySet].Key[nCntKey].rot += pmodelAll->pModel_offset[nCntKey].rot;
									}
								}
								// カウントキー
								nCntKey++;
							}
						}
						// カウントモデル
						nCntKeySet++;
					}
				}
				// カウントプレイヤー
				nCntMotion++;
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "モデルデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだテキストデータの破棄
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel_info::TextUnload(
	MODEL_ALL * pmodelAll	// モデル・モーションの情報
)
{
	// モデル情報の開放
	if (pmodelAll != NULL)
	{
		// オフセット情報の開放
		if (pmodelAll->pModel_offset != NULL)
		{
			delete[] pmodelAll->pModel_offset;
			pmodelAll->pModel_offset = NULL;
		}
		// 軌跡の基本情報の開放
		if (pmodelAll->v_MeshObitLoad.empty() == true)
		{
			std::vector<MESHOBIT_BASIC>().swap(pmodelAll->v_MeshObitLoad);
		}
		for (int nCntMotion = 0; nCntMotion < (signed)pmodelAll->pMotion.size(); nCntMotion++)
		{
			for (int nCntKeyInfo = 0; nCntKeyInfo < pmodelAll->pMotion[nCntMotion]->nNumKey; nCntKeyInfo++)
			{
				// カメラ情報の開放
				if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].pMotionCamera != NULL)
				{
					delete pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].pMotionCamera;
					pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].pMotionCamera = NULL;
				}
				// エフェクト情報の開放
				if (pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].v_MotionEffect.empty() == false)
				{
					std::vector<MOTION_EFFECT>().swap(pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].v_MotionEffect);
				}
				// キーの破棄
				delete[] pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].Key;
				pmodelAll->pMotion[nCntMotion]->KeyInfo[nCntKeyInfo].Key = NULL;
			}

			//キー情報の破棄
			delete[] pmodelAll->pMotion[nCntMotion]->KeyInfo;
			pmodelAll->pMotion[nCntMotion]->KeyInfo = NULL;
			// 攻撃の当たり判定情報の開放
			if (pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail.empty() == false)
			{
				// 軌跡の詳細情報の開放
				std::vector<MESHOBIT_DETAILS>().swap(pmodelAll->pMotion[nCntMotion]->v_MeshObit_detail);
			}
			// 攻撃の当たり判定情報の開放
			if (pmodelAll->pMotion[nCntMotion]->v_Collision.empty() == false)
			{
				// 当たり判定情報の開放
				std::vector<COLLISION_DETAILS>().swap(pmodelAll->pMotion[nCntMotion]->v_Collision);
			}
			// モーションの破棄
			delete pmodelAll->pMotion[nCntMotion];
			pmodelAll->pMotion[nCntMotion] = NULL;
		}
		// モーションの破棄
		pmodelAll->pMotion.clear();
		pmodelAll->pMotion.shrink_to_fit();

		// モデル情報全体の破棄
		delete pmodelAll;
		pmodelAll = NULL;
	}
}
