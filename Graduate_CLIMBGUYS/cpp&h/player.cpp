// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "input.h"
#include "camera.h"
#include "ui.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "XInputPad.h"
#include "keyboard.h"
#include "ui_group.h"
#include "meshdome.h"
#include "3Dparticle.h"
#include "debugproc.h"
#include "meshbox.h"
#include "playerUI.h"
#include "3Dline.h"
#include "springblock.h"
#include "resultUI.h"
#include "Calculation.h"
#include "sound.h"
#include "electricblock.h"
#include "2Dpresents.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_UPMOVELIMIT			(20.0f)		// プレイヤーの上昇移動量制限
#define PLAYER_UNDERMOVELIMIT		(5.0f)		// プレイヤーの下降移動量制限
#define DASH_TIME_MAX				(30)		// ダッシュしている時間
#define DASH_ENABLE_STICK_RANGE		(0.8f)		// ダッシュを有効にするスティックの傾き
#define PLAYER_STATUS_TXT			("data/LOAD/STATUS/PlayerStatus.txt")	// プレイヤーのステータスのテキスト
#define RESPAWN_POS					(D3DXVECTOR3(0.0f, 300.0f, 0.0f))		// リスポーン地点
#define INVINCIBLEALPHA				(0.2f)									// 無敵時の透明度
#define NORMALALPHA					(1.0f)									// 通常時の透明度

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::PLAYER_STATUS CPlayer::m_PlayerStatus		= {};		// プレイヤーのステータス
CPlayer::PLAYER_STATUS CPlayer::m_PlayerStatusInit	= {};		// プレイヤーの初期ステータス
int CPlayer::m_nDieCnt = 0;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer(CHARACTER const &character) : CCharacter::CCharacter(character)
{
	m_nCntState				= 0;					// ステートカウント
	m_bDieFlag				= false;				// 死亡フラグ
	m_Power.bDashFlag		= false;				// ダッシュフラグ
	m_Power.bTackleFrag		= false;				// タックルフラグ
	m_Power.nCntDashTime	= 0;					// ダッシュ中の切り替えカウント
	m_pPlayerUI				= nullptr;				// プレイヤーUIのポインタ
	m_Record				= RECORD();				// 記録
	m_Invincible			= _STATUSCHANGE();		// 無敵
	m_Stan					= _STATUSCHANGE();		// スタン
	m_Panic					= _STATUSCHANGE();		// パニック
	m_pPanic				= nullptr;				// パニックエフェクト

	CScene::SetObj(CScene::OBJ::OBJ_PLAYER);	// オブジェクトタイプの設定

#ifdef _DEBUG
	// 当たり判定ボックスの初期化
	pCollisionBox = NULL;
	// 当たり判定線の初期化
	pCollisionLine = NULL;
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	// バルーンキャラクター初期化
	CCharacter::Init();

	// パッドのポインタ取得
	m_pPad = CManager::GetPad(GetPlayerTag());

	// プレイヤーUIの生成
	m_pPlayerUI = CPlayerUI::Create(GetPlayerTag());


#ifdef _DEBUG
	// 当たり判定ボックスの初期化
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		// 当たり判定のタイプがキャラクターなら
		if (nCntCollision == CPlayer::COLLISIONTYPE_CHARACTER)
		{
			// メッシュボックスの生成
			pCollisionBox = CMeshBox::Create(m_pos + m_PlayerStatus.PlayerOffSet, m_PlayerStatus.PlayerSize, CMeshBox::COLLISION_TYPE::TYPE_CENTER);
			// 薄青色に
			pCollisionBox->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}

		// 当たり判定のタイプが押し出しなら
		else if (nCntCollision == CPlayer::COLLISIONTYPE_PUSH)
		{
			// 3D線生成
			pCollisionLine = C3DLine::Create(NULL, D3DVECTOR3_ZERO,
				m_pos + m_PlayerStatus.PushOffSet,
				m_pos + m_PlayerStatus.PushOffSet + D3DXVECTOR3(sinf(m_rot.y) * m_PlayerStatus.PushSize,0.0f,cosf(m_rot.y) * m_PlayerStatus.PushSize));
		}
	}

#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	// 記録更新_タイム
	m_Record.nTime = CGame::GetSecond();

	// 記録情報をリザルトに渡す
	CResultUI::SetRecord(GetPlayerTag(), m_Record);

	// キャラクター終了処理
	CCharacter::Uninit();

	// プレイヤーUIの初期化
	m_pPlayerUI = nullptr;

	// パニックエフェクトの初期化
	m_pPanic = nullptr;

#ifdef _DEBUG
	pCollisionBox = NULL;
	pCollisionLine = NULL;
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		// 行動可能状態なら行動可能
		CGame *pGame = (CGame*)CManager::GetBaseMode();

		if (pGame && pGame->GetbOperatable())
		{
			// 自キャラの行動処理
			MyAction();
		}
		else
		{
			D3DXVECTOR3 move;
			move = CCharacter::GetMove();
			move.x = 0;
			move.z = 0;
			CCharacter::SetMove(move);
			m_bRun = false;
		}
	}
	else if (CManager::GetMode() == CManager::MODE_RESULT)
	{

	}
	else
	{
		// 自キャラの行動処理
		MyAction();
	}

	if (m_pPlayerUI)
	{	// プレイヤーUIの位置の設定
		m_pPlayerUI->SetPos(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}

	// 状態更新
	StateUpdate();
	// モーション設定処理
	StatusMotion();
	// キャラクター更新
	CCharacter::Update();
	// 限界地点
	CCharacter::Limit();
	// 当たり判定の処理
	Collision();
#ifdef _DEBUG
	// 当たり判定ボックスの位置の更新
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		// 当たり判定のタイプがキャラクターなら
		if (nCntCollision == CPlayer::COLLISIONTYPE_CHARACTER)
		{
			if (!pCollisionBox) continue;
			// 位置設定
			pCollisionBox->SetPos(m_pos + m_PlayerStatus.PlayerOffSet);
			// サイズ設定
			pCollisionBox->SetSize(m_PlayerStatus.PlayerSize);
			// 頂点座標の設定
			pCollisionBox->SetVtxPos();
			// 当たり判定の可視化設定
			if (CBaseMode::GetCollisionDisp() != pCollisionBox->GetDisp())
			{
				pCollisionBox->SetDisp(CBaseMode::GetCollisionDisp());
			}
		}
		// 当たり判定のタイプが押し出しなら
		else if (nCntCollision == CPlayer::COLLISIONTYPE_PUSH)
		{
			if (!pCollisionLine) continue;
			D3DXVECTOR3 DirectVec = CCharacter::GetDirectionVec();
#ifdef _DEBUG
			CDebugproc::Print("方向:(%.2f,%.2f,%.2f)\n", DirectVec.x, DirectVec.y, DirectVec.z);
			CDebugproc::Print("JumpAble:(%d)\n", GetJumpAble());

#endif // _DEBUG

			// 位置設定
			pCollisionLine->SetPos(
				m_pos + m_PlayerStatus.PushOffSet,
				m_pos + m_PlayerStatus.PushOffSet - D3DXVECTOR3(CCharacter::GetDirectionVec().x * m_PlayerStatus.PushSize, 0.0f, CCharacter::GetDirectionVec().z * m_PlayerStatus.PushSize));
			// 当たり判定の可視化設定
			if (CBaseMode::GetCollisionDisp() != pCollisionLine->GetDisp())
			{
				pCollisionLine->SetDisp(CBaseMode::GetCollisionDisp());
			}
		}
	}
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自キャラ行動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyAction(void)
{
	if (m_Stan.bChange) return;
	// 自キャラの移動処理
	MyMove();

	// 試験的ダッシュの切り替え
	if (m_Power.bDashFlag)
	{
		m_Power.nCntDashTime++;

		if (m_Power.nCntDashTime > DASH_TIME_MAX)
		{
			// 初期化
			m_Power.nCntDashTime	= 0;
			m_Power.bDashFlag		= false;
			m_Power.bTackleFrag		= false;
		}
	}
	CDebugproc::Print("\nPlayerダッシュ %d\n", m_Power.bDashFlag);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 移動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyMove(void)
{
	// 変数宣言
	D3DXVECTOR3 move, rot;			// 移動量、回転
	D3DXVECTOR3 vec;				// ベクトル
	float fRot;						// 回転

	// 情報取得
	rot = CCharacter::GetRotDest();								// 目的回転量
	move = CCharacter::GetMove();								// 移動量
	fRot = CManager::GetRenderer()->GetCamera()->GetRot().y;	// カメラ回転
	vec = CCharacter::GetDirectionVec();						// ベクトル
	CKeyboard *pKeyboard = CManager::GetKeyboard();

	// 走っている状態ではない
	m_bRun = false;
	// 移動 //
	if (!m_Power.bDashFlag &&
		!m_Power.bCharge)
	{
		/* キーボード */
		// 左
		if (pKeyboard->GetKeyboardPress(DIK_A))
		{
			// 奥
			if (pKeyboard->GetKeyboardPress(DIK_W))
			{
				// パニック状態なら
				if (!m_Panic.bChange)
				{
					rot.y = ROT_KEY_AW_ON_CAMERA(fRot);
				}
				// それ以外
				else
				{
					rot.y = ROT_KEY_SD_ON_CAMERA(fRot);
				}
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}
			// 手前
			else if (pKeyboard->GetKeyboardPress(DIK_S))
			{
				// パニック状態なら
				if (!m_Panic.bChange)
				{
					rot.y = ROT_KEY_AS_ON_CAMERA(fRot);
				}
				// それ以外
				else
				{
					rot.y = ROT_KEY_WD_ON_CAMERA(fRot);
				}
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}
			// 左
			else
			{
				// パニック状態なら
				if (!m_Panic.bChange)
				{
					rot.y = ROT_KEY_A_ON_CAMERA(fRot);
				}
				// それ以外
				else
				{
					rot.y = ROT_KEY_D_ON_CAMERA(fRot);
				}
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}
			// 走っている状態
			m_bRun = true;
		}
		// 右
		else if (pKeyboard->GetKeyboardPress(DIK_D))
		{

			// 奥
			if (pKeyboard->GetKeyboardPress(DIK_W))
			{
				// パニック状態なら
				if (!m_Panic.bChange)
				{
					rot.y = ROT_KEY_WD_ON_CAMERA(fRot);
				}
				// それ以外
				else
				{
					rot.y = ROT_KEY_AS_ON_CAMERA(fRot);
				}
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}
			// 手前
			else if (pKeyboard->GetKeyboardPress(DIK_S))
			{
				// パニック状態なら
				if (!m_Panic.bChange)
				{
					rot.y = ROT_KEY_SD_ON_CAMERA(fRot);
				}
				// それ以外
				else
				{
					rot.y = ROT_KEY_AW_ON_CAMERA(fRot);
				}
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}
			// 右
			else
			{
				// パニック状態なら
				if (!m_Panic.bChange)
				{
					rot.y = ROT_KEY_D_ON_CAMERA(fRot);
				}
				// それ以外
				else
				{
					rot.y = ROT_KEY_A_ON_CAMERA(fRot);
				}
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}
			// 走っている状態
			m_bRun = true;
		}
		// 奥に行く
		else if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			// パニック状態なら
			if (!m_Panic.bChange)
			{
				rot.y = ROT_KEY_W_ON_CAMERA(fRot);
			}
			// それ以外
			else
			{
				rot.y = ROT_KEY_S_ON_CAMERA(fRot);
			}
			PlayerMoveSet(
				vec,
				rot,
				move
			);
			// 走っている状態
			m_bRun = true;
		}
		// 手前に行く
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			// パニック状態なら
			if (!m_Panic.bChange)
			{
				rot.y = ROT_KEY_S_ON_CAMERA(fRot);
			}
			// それ以外
			else
			{
				rot.y = ROT_KEY_W_ON_CAMERA(fRot);
			}
			PlayerMoveSet(
				vec,
				rot,
				move
			);
			// 走っている状態
			m_bRun = true;
		}
		// それ以外
		else
		{
			// 走っている状態ではない
			m_bRun = false;
		}

		// 試験的キーボードジャンプ
		if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) && GetJumpAble())
		{
			SetMotion(MOTIONTYPE_JUMP);
			move.y += m_PlayerStatus.fJump;
			SetJumpAble(false);
		}
		// 試験的タックル
		if (pKeyboard->GetKeyboardTrigger(DIK_J))
		{
			SetMotion(MOTIONTYPE_TACKLECHARGE);
			m_Power.bCharge = true;
		}
	}
	/* ゲームパッド */
	// パッド用 //
	float fValueH, fValueV;		// ゲームパッドのスティック情報の取得用
	float fAngle;				// スティック角度の計算用変数
	fAngle = 0.0f;				// 角度

	if (m_pPad)
	{
		// ゲームパッドのスティック情報を取得
		m_pPad->GetStickLeft(&fValueH, &fValueV);

		if (!m_Power.bDashFlag &&	// ダッシュ時
			!m_Power.bCharge)		// チャージ時
		{
			// プレイヤー移動
			// ゲームパッド移動
			if (fValueH != 0 || fValueV != 0)
			{
				// 走っている状態
				m_bRun = true;

				// 角度の計算
				if (m_Panic.bChange)
				{
					fAngle = atan2f(-fValueH, -fValueV);
				}
				else
				{
					fAngle = atan2f(fValueH, fValueV);
				}
				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				else if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				rot.y = fAngle + fRot;
				PlayerMoveSet(
					vec,
					rot,
					move
				);
			}

			// 試験的ジャンプ ( のちに中身変わる予定 多分 )
			if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_A, 1) && GetJumpAble())
			{
				SetMotion(MOTIONTYPE_JUMP);

				move.y += m_PlayerStatus.fJump;
				SetJumpAble(false);
			}

			// 試験的タックル ( のちに中身変わる予定 多分 )
			if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_X, 1))
			{
				SetMotion(MOTIONTYPE_TACKLECHARGE);
				m_Power.bCharge = true;
			}
		}
	}

	// ジャンプしているときの慣性
	if (!GetJumpAble())
	{
		move.x += ( -m_move.x) * m_PlayerStatus.fJumpInertia;
		move.z += ( -m_move.z) * m_PlayerStatus.fJumpInertia;
	}
	else
	{
		move.x += (-m_move.x) * m_PlayerStatus.fNormalInertia;
		move.z += (-m_move.z) * m_PlayerStatus.fNormalInertia;
	}

	/*
	if (vec.x < 0)
	{
		vec.x *= -1;
	}
	if (vec.z < 0)
	{
		vec.z *= -1;
	}
	*/
	CCharacter::SetDirectionVec(vec);
	// yの上限設定
	if (move.y > PLAYER_UPMOVELIMIT)
	{
		move.y = PLAYER_UPMOVELIMIT;
	}
	if (move.y < -PLAYER_UNDERMOVELIMIT)
	{
		move.y = -PLAYER_UNDERMOVELIMIT;
	}
	CCharacter::SetMove(move);

	// 目標回転量の設定
	CCharacter::SetRotDest(rot);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤーの移動方向設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerMoveSet(D3DXVECTOR3 & Vec, D3DXVECTOR3 const & Rot,D3DXVECTOR3 & Move)
{
	Vec = D3DXVECTOR3(sinf(Rot.y), 0.0f, cosf(Rot.y));
	// スティックの角度によってプレイヤー移動
	Move.x -= Vec.x * (m_PlayerStatus.fMove);
	Move.z -= Vec.z * (m_PlayerStatus.fMove);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 状態によってのモーション設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StatusMotion(void)
{
	// 勝利モーション01のとき
	if (CCharacter::GetMotion() == CCharacter::MOTIONTYPE::MOTIONTYPE_V_01_WAIT ||
		CCharacter::GetMotion() == CCharacter::MOTIONTYPE::MOTIONTYPE_V_02_WAIT ||
		CCharacter::GetMotion() == CCharacter::MOTIONTYPE::MOTIONTYPE_V_03_WAIT ||
		CCharacter::GetMotion() == CCharacter::MOTIONTYPE::MOTIONTYPE_V_04_WAIT ||
		CCharacter::GetMotion() == CCharacter::MOTIONTYPE::MOTIONTYPE_BANZAI) return;
	// 移動中
	if (m_bRun)
	{
		// モーション設定(移動)
		SetMotion(MOTIONTYPE_MOVE);
	}
	// 待機
	else
	{
		SetMotion(MOTIONTYPE_NEUTRAL);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定の処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Collision(void)
{
	// キャラクターの当たり判定
	CharacterCollision();
	// ブロックの当たり判定
	BlockCollision();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::BlockCollision(void)
{
	// 変数宣言
	CBaseblock *			pBaseBlock;							// シーンX情報
	COLLISIONDIRECTION		Direct = COLLISIONDIRECTION::NONE;	// 当たり判定の方向
	CBaseblock::PUSHBLOCK	Pushblock;							// プッシュブロック情報
	float					fDistance = -1.0f;					// 距離
	D3DXVECTOR3				PredictionPoint;					// 予測点
	PredictionPoint = m_pos + m_PlayerStatus.PushOffSet - D3DXVECTOR3(CCharacter::GetDirectionVec().x * m_PlayerStatus.PushSize, 0.0f, CCharacter::GetDirectionVec().z * m_PlayerStatus.PushSize);
	Pushblock = CBaseblock::PUSHBLOCK(NULL, -1.0f, Direct);



	// ブロックループ
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DBLOCK); nCntBlock++)
	{
		// 当たり判定の初期化
		Direct = COLLISIONDIRECTION::NONE;
		// NULL代入
		pBaseBlock = NULL;
		// 情報取得
		pBaseBlock = (CBaseblock *)CScene::GetScene(CScene::LAYER_3DBLOCK, nCntBlock);
		// NULLなら
		// ->関数を抜ける
		if (!pBaseBlock) continue;
		else if (!pBaseBlock->GetUse()) continue;
		else if (!CCalculation::Collision_Sphere(
			CCharacter::GetPos(),
			100.0f,
			pBaseBlock->GetPos(),
			1.0f
		)) continue;
		if (!(pBaseBlock->GetType() == CBaseblock::BLOCKTYPE_FIELD ||	// フィールドブロックではない
			pBaseBlock->GetType() == CBaseblock::BLOCKTYPE_STEEL) &&	// 鋼鉄ブロックではない
			m_Power.bTackleFrag &&										// 自身がタックルしている状態
			!pBaseBlock->GetPushAfter().bPushState &&					// ブロックが押されている状態ではない
			!pBaseBlock->GetFall()										// 落ちている状態ではない
			)
		{
			// 方向に直線を出し
			// 線とポリゴンで判定を取る
			Direct = pBaseBlock->PushBlock(
				m_pos + m_PlayerStatus.PushOffSet,
				PredictionPoint,
				fDistance);

			// ブロックに当たっていたら
			if (Direct != COLLISIONDIRECTION::NONE)
			{
				// プッシュブロックの情報が存在するなら
				if (Pushblock.pBlock)
				{
					// 距離を比較してどちらが近いか
					if (Pushblock.fDistance > fDistance ||
						Pushblock.fDistance < 0)
					{
						// 情報を代入
						Pushblock = CBaseblock::PUSHBLOCK(pBaseBlock, fDistance, Direct);
					}
				}
				else
				{
					Pushblock = CBaseblock::PUSHBLOCK(pBaseBlock, fDistance, Direct);
				}
			}
		}
		// 当たり判定
		Direct = pBaseBlock->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&m_PlayerStatus.PlayerSize,
			m_PlayerStatus.PlayerOffSet
		);
		// ブロックの判定
		// 前
		if (Direct == COLLISIONDIRECTION::FRONT)
		{
			m_DieStatus.bFront = true;
		}
		// 後
		else if (Direct == COLLISIONDIRECTION::BACK)
		{
			m_DieStatus.bBack = true;
		}
		// 左
		else if (Direct == COLLISIONDIRECTION::LEFT)
		{
			m_DieStatus.bLeft = true;
		}
		// 右
		else if (Direct == COLLISIONDIRECTION::RIGHT)
		{
			m_DieStatus.bRight = true;
		}
		// 上
		else if (Direct == COLLISIONDIRECTION::UP)
		{
			// パニックブロックなら
			if (pBaseBlock->GetType() == CBaseblock::BLOCKTYPE_PANIC)
			{
				m_Panic.Set(true, m_PlayerStatus.nMaxPanicTime);

				// パニックエフェクトの生成
				PanicEffectCreate();
				// ジャンプ可能設定
				SetJumpAble(true);
			}
			// 電気ブロックなら
			else if (pBaseBlock->GetType() == CBaseblock::BLOCKTYPE_ELECTRIC)
			{
				CElectricblock * pElectBlock = (CElectricblock *)pBaseBlock;
				if (pElectBlock)
				{
					pElectBlock->SetPlayerTag(GetPlayerTag());
					pElectBlock->SetElectric(true);
				}
				// ジャンプ可能設定
				SetJumpAble(true);
			}
			// ばねブロックなら
			else if (pBaseBlock->GetType() == CBaseblock::BLOCKTYPE_SPRING)
			{
				SpringJump();
				// ジャンプ可能設定
				SetJumpAble(false);
			}
			else
			{
				// ジャンプ可能設定
				SetJumpAble(true);
			}
			m_DieStatus.bUp = true;
		}
		// 下
		else if (Direct == COLLISIONDIRECTION::DOWN)
		{
			m_DieStatus.bDown = true;
		}
	}


	// 挟まったら死ぬ処理
	if ((m_DieStatus.bUp && m_DieStatus.bDown) ||
		(m_DieStatus.bFront && m_DieStatus.bBack) ||
		(m_DieStatus.bRight && m_DieStatus.bLeft)
		)
	{
		if (!this->GetDie())
		{
			if (CPlayer::GetDieCount() < 3)
			{
				// 記録更新
				m_Record.DieCause = DIECAUSE::DIECAUSE_PRESS;
				// 死亡設定
				CManager::GetSound()->PlaySound(CSound::LABEL_SE_DIE1);
				// 死亡処理
				Die();
			}
		}
	}
	m_DieStatus.Init();

	// ブロックの押し出し処理
	if (m_Power.bTackleFrag)
	{
		if (Pushblock.pBlock)
		{
			// 殴り音
			CManager::GetSound()->PlaySound(CSound::LABEL_SE_PUNCH);

			// 前
			if (Pushblock.Direction == COLLISIONDIRECTION::FRONT)
			{
				// キーボードとパッドのボタンを離したときの反応処理
				// 押した瞬間から話したときの時間
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(0, 0, -m_Power.nPushPower));
			}
			// 後
			else if (Pushblock.Direction == COLLISIONDIRECTION::BACK)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(0, 0, m_Power.nPushPower));
			}
			// 左
			else if (Pushblock.Direction == COLLISIONDIRECTION::LEFT)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(m_Power.nPushPower, 0, 0));
			}
			// 右
			else if (Pushblock.Direction == COLLISIONDIRECTION::RIGHT)
			{
				PushBlock(Pushblock.pBlock, CBaseblock::GRID(-m_Power.nPushPower, 0, 0));
			}

		}
		// 押し出す力を初期化
		m_Power.nPushPower = 1;
		m_Power.bTackleFrag = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// キャラクター同士の当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::CharacterCollision(void)
{
	// 変数宣言
	CPlayer * pPlayer;	// キャラクター情報
	COLLISIONDIRECTION Direct;	// 当たり判定の方向
	// ブロックループ
	for (int nCntPlayer = 0; nCntPlayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntPlayer++)
	{
		// NULL代入
		pPlayer = NULL;
		// 情報取得
		pPlayer = (CPlayer *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntPlayer);
		// NULLなら
		// ->関数を抜ける
		if (pPlayer == NULL)
		{
			continue;
		}
		// 無敵状態なら
		else if (pPlayer->m_Invincible.bChange) continue;
		// 当たり判定
		Direct = pPlayer->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&m_PlayerStatus.PlayerSize,
			m_PlayerStatus.PlayerOffSet
		);
		// 相手プレイヤーが上にいるとき
		if (Direct == COLLISIONDIRECTION::UP)
		{
			// 変数宣言
			D3DXVECTOR3 move;				// 移動量
			move = CCharacter::GetMove();	// 移動量

			// プレイヤーを踏んだ時のジャンプ
			move.y += m_PlayerStatus.fJump * 0.5f;
			SetJumpAble(false);
			// yの上限設定
			if (move.y > PLAYER_UPMOVELIMIT)
			{
				move.y = PLAYER_UPMOVELIMIT;
			}
			if (move.y < -PLAYER_UNDERMOVELIMIT)
			{
				move.y = -PLAYER_UNDERMOVELIMIT;
			}
			// 移動設定
			CCharacter::SetMove(move);

			// 相手プレイヤーの処理
			// スタン状態設定
			pPlayer->m_Stan.Set(true, m_PlayerStatus.nMaxStanTime[STATUSTYPE_JUMP]);
			// 無敵状態設定
			pPlayer->m_Invincible.Set(true, m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_JUMP] + m_PlayerStatus.nMaxStanTime[STATUSTYPE_JUMP]);
			// 透明化
			pPlayer->AlphaCharacter(INVINCIBLEALPHA);
			// 気絶モーション
			pPlayer->SetMotion(MOTIONTYPE_FAINTED);
			// 移動状態の初期化処理
			D3DXVECTOR3 Oppmove = pPlayer->GetMove();
			Oppmove.x = 0.0f;
			Oppmove.z = 0.0f;
			pPlayer->SetMove(Oppmove);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出した後のブロックに当たったプレイヤーの押し出し判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PushAfter_Collision(
	CBaseblock * pBaseBlock,	// ベースブロック情報
	CBaseblock::GRID Grid		// 行列高情報
)
{
	// 変数宣言
	CPlayer * pPlayer = NULL;	// プレイヤー
	// キャラクターループ
	for (int nCntPlayer = 0; nCntPlayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntPlayer++)
	{
		// 情報取得
		pPlayer = (CPlayer *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntPlayer);	// プレイヤー情報
		// 取得プレイヤーの情報がNULLなら ||
		// 取得プレイヤーと同じ情報なら
		// ->ループスキップ
		if (pPlayer == NULL || pPlayer == this) continue;
		// 方向
		COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;
		// 当たった後の方向を取得
		Direct = pBaseBlock->Collision(
			pPlayer->GetObj(), &pPlayer->GetPos(), &pPlayer->GetPosOld(),
			&D3DXVECTOR3(25.0f, 40.0f, 25.0f),
			D3DXVECTOR3(0.0f, 20.0f, 0.0f)
		);
		// もし当たっていれば
		if (Direct != COLLISIONDIRECTION::NONE)
		{
			// プレイヤーの行列高
			CBaseblock::GRID PlayerGrid;
			// プレイヤーの最新行列高
			PlayerGrid = pBaseBlock->GetGrid() + Grid;
			// プレイヤーの位置設定
			pPlayer->SetPos(PlayerGrid.GetPos(CBaseblock::GetSizeRange()));
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤーのステータスのロード
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerStatusLoad(void)
{
	// ファイルポイント
	FILE *pFile = nullptr;

	char cReadText[128];			// 文字として読み取る
	char cHeadText[128];			// 比較用
	char cDie[128];					// 不要な文字
	int nCntStatus = 0;				// ステータスカウント
	// ファイルを開く
	pFile = fopen(PLAYER_STATUS_TXT, "r");

	// 開いているとき
	if (pFile)
	{
		// SCRIPTが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile); // 一文読み込み
			sscanf(cReadText, "%s", &cHeadText);		// 比較用テキストに文字を代入
		}

		// SCRIPTが来たら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// END_SCRIPTが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				// STATUS_SETが来たら
				if (strcmp(cHeadText, "STATUS_SET") == 0)
				{
					// END_STATUS_SETが来るまでループ
					while (strcmp(cHeadText, "END_STATUS_SET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						// Moveが来たら
						if (strcmp(cHeadText, "Move") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fMove);
						}
						// Jumpが来たら
						else if (strcmp(cHeadText, "Jump") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fJump);
						}
						// NormalInertiaが来たら
						else if (strcmp(cHeadText, "NormalInertia") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fNormalInertia);
						}
						// JumpInertiaが来たら
						else if (strcmp(cHeadText, "JumpInertia") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fJumpInertia);
						}
						// PlayerSizeが来たら
						else if (strcmp(cHeadText, "PlayerSize") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PlayerSize.x, &m_PlayerStatus.PlayerSize.y, &m_PlayerStatus.PlayerSize.z);
						}
						// PlayerOffSetが来たら
						else if (strcmp(cHeadText, "PlayerOffSet") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PlayerOffSet.x, &m_PlayerStatus.PlayerOffSet.y, &m_PlayerStatus.PlayerOffSet.z);
						}
						// PushSizeが来たら
						else if (strcmp(cHeadText, "PushSize") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.PushSize);
						}
						// PushOffSetが来たら
						else if (strcmp(cHeadText, "PushOffSet") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PushOffSet.x, &m_PlayerStatus.PushOffSet.y, &m_PlayerStatus.PushOffSet.z);
						}
						// PowerTimeが来たら
						else if (strcmp(cHeadText, "PowerTime") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_PlayerStatus.nMaxPowerTime);
						}
						// StanTimeが来たら
						else if (strcmp(cHeadText, "StanTime") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_PlayerStatus.nMaxStanTime[nCntStatus]);
						}
						// InvincibleTimeが来たら
						else if (strcmp(cHeadText, "InvincibleTime") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_PlayerStatus.nMaxInvincibleTime[nCntStatus]);
							nCntStatus++;
						}
						// PanicTimeが来たら
						else if (strcmp(cHeadText, "PanicTime") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_PlayerStatus.nMaxPanicTime);
						}
						else if (strcmp(cHeadText, "END_STATUS_SET") == 0)
						{
							// プレイヤーの初期ステータスを保存
							m_PlayerStatusInit = m_PlayerStatus;
						}
					}
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		// 読み込み失敗時の警告表示
		MessageBox(NULL, "プレイヤーのステータス読み込み失敗", PLAYER_STATUS_TXT, MB_ICONWARNING);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤーのステータスのセーブ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerStatusSave(void)
{
	// ファイルポイント
	FILE	*pFile = nullptr;

	// ファイルを開く
	pFile = fopen(PLAYER_STATUS_TXT, "w");

	// 開いているとき
	if (pFile)
	{
		fprintf(pFile, COMMENT02);
		fprintf(pFile, "// プレイヤーのステータス\n");
		fprintf(pFile, COMMENT02);

		fprintf(pFile, "SCRIPT\n");
		fprintf(pFile, NEWLINE);

		// セーブするモデルの情報
		fprintf(pFile, "STATUS_SET\n");
		fprintf(pFile, "	Move			= %.1f\n", m_PlayerStatus.fMove);
		fprintf(pFile, "	Jump			= %.1f\n", m_PlayerStatus.fJump);
		fprintf(pFile, "	NormalInertia	= %.2f\n", m_PlayerStatus.fNormalInertia);
		fprintf(pFile, "	JumpInertia		= %.2f\n", m_PlayerStatus.fJumpInertia);
		fprintf(pFile, "	PlayerSize		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PlayerSize.x, m_PlayerStatus.PlayerSize.y, m_PlayerStatus.PlayerSize.z);
		fprintf(pFile, "	PlayerOffSet	= %.1f	%.1f	%.1f\n", m_PlayerStatus.PlayerOffSet.x, m_PlayerStatus.PlayerOffSet.y, m_PlayerStatus.PlayerOffSet.z);
		fprintf(pFile, "	PushSize		= %.1f\n", m_PlayerStatus.PushSize);
		fprintf(pFile, "	PushOffSet		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PushOffSet.x, m_PlayerStatus.PushOffSet.y, m_PlayerStatus.PushOffSet.z);
		fprintf(pFile, "	PowerTime		= %d\n", m_PlayerStatus.nMaxPowerTime);

		fprintf(pFile, "	StatusType_Block\n");
		fprintf(pFile, "		StanTime		= %d\n", m_PlayerStatus.nMaxStanTime[STATUSTYPE_BLOCK]);
		fprintf(pFile, "		InvincibleTime	= %d\n", m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_BLOCK]);
		fprintf(pFile, "	StatusType_Jump\n");
		fprintf(pFile, "		StanTime		= %d\n", m_PlayerStatus.nMaxStanTime[STATUSTYPE_JUMP]);
		fprintf(pFile, "		InvincibleTime	= %d\n", m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_JUMP]);
		fprintf(pFile, "	StatusType_Electric\n");
		fprintf(pFile, "		StanTime		= %d\n", m_PlayerStatus.nMaxStanTime[STATUSTYPE_ELECTRIC]);
		fprintf(pFile, "		InvincibleTime	= %d\n", m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_ELECTRIC]);

		fprintf(pFile, "	PanicTime		= %d\n", m_PlayerStatus.nMaxPanicTime);

		fprintf(pFile, "END_STATUS_SET\n\n");

		fprintf(pFile, "END_SCRIPT\n");

		// ファイルを閉じる
		fclose(pFile);

		// 読み込み成功時の結果表示
		MessageBox(NULL, "セーブしました", PLAYER_STATUS_TXT, MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		// 読み込み失敗時の警告表示
		MessageBox(NULL, "読み込み失敗", PLAYER_STATUS_TXT, MB_ICONWARNING);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// プレイヤーのステータスの初期値のロード
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PlayerStatusInitLoad(void)
{
	// プレイヤーの初期ステータスを代入
	m_PlayerStatus	= m_PlayerStatusInit;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 状態更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StateUpdate(void)
{
	// 力溜めの更新
	PowerUpdate();
	// スタン状態の更新
	StanUpdate();
	// 無敵状態の更新
	InvincibleUpdate();
	// パニック状態の更新
	PanicUpdate();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 力溜めの更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PowerUpdate(void)
{
	if (!m_Power.bCharge) return;
	if (m_Power.nCntTime == m_PlayerStatus.nMaxPowerTime)
	{
		m_Power.bCharge = false;
		m_Power.bDashFlag = true;
		m_Power.bTackleFrag = true;
		m_Power.nCntTime = 0;
		SetMotion(MOTIONTYPE_TACKLE);
		m_Power.nPushPower = 2;
	}
	// ボタンが離されたら
	else if (CManager::GetKeyboard()->GetKeyboardRelease(DIK_J))
	{
		m_Power.bCharge = false;
		m_Power.bDashFlag = true;
		m_Power.bTackleFrag = true;
		m_Power.nCntTime = 0;
		SetMotion(MOTIONTYPE_TACKLE);
	}
	// パッド
	else if (m_pPad)
	{
		// 試験的タックル ( のちに中身変わる予定 多分 )
		if (m_pPad->GetRelease(CXInputPad::XINPUT_KEY::JOYPADKEY_X, 1))
		{
			m_Power.bCharge = false;
			m_Power.bDashFlag = true;
			m_Power.bTackleFrag = true;
			m_Power.nCntTime = 0;
			SetMotion(MOTIONTYPE_TACKLE);
		}
	}
	// 力溜めカウントアップ
	m_Power.nCntTime++;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// スタン状態の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StanUpdate(void)
{
	// スタン状態ではないなら
	if (!m_Stan.bChange) return;
	// 規定値を超えたら
	else if (m_Stan.nChangeTime >= m_Stan.nMaxTime)
	{
		// スタン状態の初期化処理
		m_Stan.Init();
		// 通常モーション
		SetMotion(MOTIONTYPE_NEUTRAL);
	}
	// スタンカウント更新
	m_Stan.nChangeTime++;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 無敵状態の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::InvincibleUpdate(void)
{
	// 無敵状態ではないなら
	if (!m_Invincible.bChange) return;
	// 規定値を超えたら
	else if (m_Invincible.nChangeTime >= m_Invincible.nMaxTime)
	{
		// 無敵状態の初期化処理
		m_Invincible.Init();
		// 通常状態
		CCharacter::AlphaCharacter(NORMALALPHA);
	}
	// 無敵カウント更新
	m_Invincible.nChangeTime++;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// パニック状態の更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PanicUpdate(void)
{
	// パニック状態ではないなら
	if (!m_Panic.bChange) return;
	// 規定値を超えたら
	else if (m_Panic.nChangeTime >= m_Panic.nMaxTime)
	{
		// パニック状態の初期化処理
		m_Panic.Init();

		// パニックエフェクトを消す
		if (m_pPanic)
		{
			m_pPanic->Release();
			m_pPanic = nullptr;
		}
	}
	// パニックエフェクトの位置をプレイヤーに合わせる
	if (m_pPanic)
	{
		m_pPanic->SetPos(GetPos());
	}
	// パニックカウント更新
	m_Panic.nChangeTime++;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// パニックエフェクトの生成
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PanicEffectCreate(void)
{
	// 生成されていないとき
	if (!m_pPanic)
	{
		m_pPanic = CScene_THREE::Create(
			CScene_THREE::OFFSET_TYPE_VERTICAL_UNDER,			// タイプ
			GetPos(), 											// 位置
			D3DXVECTOR3(70.0f, 50.0f, 0.0f),					// サイズ
			CTexture_manager::TYPE_EFFECT_PANIC,				// テクスチャータイプ
			CScene::LAYER_3DOBJECT,
			D3DVECTOR3_ZERO,									// 角度
			true,												// ビルボード
			false,												// Zバッファ
			true,												// ライティング
			true);												// アルファブレンド
	}
	// 生成されているとき
	if (m_pPanic)
	{
		m_pPanic->SetTexAnim(
			4,			//アニメーションカウント
			6,			//水平のアニメーション数
			1,			//垂直のアニメーション数
			true		//ループするかしないか
		);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 電気ブロック使用時
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::ElectricUse(PLAYER_TAG const & PlayerTag)
{
	C2DPresents * pThunder = C2DPresents::Create(CScene_TWO::OFFSET_TYPE_CENTER, SCREEN_CENTER_POS, SCREEN_SIZE, -1,0.0f,D3DXCOLOR(0.0f, 0.0f, 0.0f,0.3f),CScene::LAYER_PARTICLE);
	pThunder->SetCooperation(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), 1,1);
	pThunder->SetTimeOver(40);
	// 変数宣言
	CPlayer * pPlayer;	// キャラクター情報
	// プレイヤーループ
	for (int nCntPlayer = 0; nCntPlayer < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntPlayer++)
	{
		// NULL代入
		pPlayer = NULL;
		// 情報取得
		pPlayer = (CPlayer *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntPlayer);
		// NULLなら
		// ->関数を抜ける
		if (pPlayer == NULL) continue;
		else if (pPlayer->GetPlayerTag() == PlayerTag) continue;
		// 無敵状態なら
		else if (pPlayer->m_Invincible.bChange) continue;
		pPlayer->m_Stan.Set(true, m_PlayerStatus.nMaxStanTime[STATUSTYPE_ELECTRIC]);
		pPlayer->m_Invincible.Set(true, m_PlayerStatus.nMaxStanTime[STATUSTYPE_ELECTRIC] + m_PlayerStatus.nMaxStanTime[STATUSTYPE_ELECTRIC]);
		// 透明化
		pPlayer->AlphaCharacter(INVINCIBLEALPHA);
		// 気絶モーション
		pPlayer->SetMotion(MOTIONTYPE_FAINTED);
		// 移動状態の初期化処理
		D3DXVECTOR3 move = pPlayer->GetMove();
		move.x = 0.0f;
		move.z = 0.0f;
		pPlayer->SetMove(move);
		// 雷エフェクト生成
		CScene_THREE * pThunder = CScene_THREE::Create(
			CScene_THREE::OFFSET_TYPE_VERTICAL_UNDER,			// タイプ
			pPlayer->GetPos(),									// 位置
			D3DXVECTOR3(50.0f, 500.0f, 0.0f),					// サイズ
			CTexture_manager::TYPE_EFFECT_THUNDER,				// テクスチャータイプ
			CScene::LAYER_3DPARTICLE,								// レイヤー順番
			D3DVECTOR3_ZERO,									// 角度
			true,												// ビルボード
			false,												// Zバッファ
			true,												// ライティング
			true);												// アルファブレンド
		pThunder->SetTexAnim(
			5, 5, 2, false
		);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// バネ用ジャンプ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::SpringJump(void)
{
	//if (!m_bSpringFlag)
	{
		// バネ
		CManager::GetSound()->PlaySound(CSound::LABEL_SE_SPRING);

		// 変数宣言
		D3DXVECTOR3 move;				// 移動量
		move = CCharacter::GetMove();	// 移動量

										// バネ用ジャンプ
		move.y += m_PlayerStatus.fJump * 1.5f;
		SetJumpAble(false);

		// ジャンプしているときの慣性
		if (!GetJumpAble())
		{
			move.x += (-m_move.x) * m_PlayerStatus.fJumpInertia;
			move.z += (-m_move.z) * m_PlayerStatus.fJumpInertia;
		}
		else
		{
			move.x += (-m_move.x) * m_PlayerStatus.fNormalInertia;
			move.z += (-m_move.z) * m_PlayerStatus.fNormalInertia;
		}

		// yの上限設定
		if (move.y > PLAYER_UPMOVELIMIT)
		{
			move.y = PLAYER_UPMOVELIMIT;
		}
		if (move.y < -PLAYER_UNDERMOVELIMIT)
		{
			move.y = -PLAYER_UNDERMOVELIMIT;
		}
		CCharacter::SetMove(move);

		//m_bSpringFlag = true;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの押し出し処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PushBlock(
	CBaseblock * pBlock,			// ブロック情報
	CBaseblock::GRID const & Grid	// 方向
)
{
	// プレイヤーがダッシュ時以外なら
	// ->関数を抜ける
	if (!CPlayer::GetDashFlag()) return;

	// ノックバック処理
	this->SetPos(this->GetPos() + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	// 落ちている状態なら
	if (pBlock->GetFall() ||
		pBlock->GetType() == CBaseblock::BLOCKTYPE_FIELD) return;
	CBaseblock::GRID NextGrid = pBlock->GetGrid();					// 押し出しブロックの行列高情報
	CBaseblock::GRID PushGrid = CBaseblock::GRID(0,0,0);
	bool bPush = false;												// 押し出し状態
	for (int nCntColumn = 0; nCntColumn < abs(Grid.nColumn); nCntColumn++)
	{
		// 列が0超過
		if (Grid.nColumn > 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに加算
			NextGrid.nColumn++;
		}
		// 列が0未満
		else if (Grid.nColumn < 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに減算
			NextGrid.nColumn--;
		}
		// 押し出し方向にブロックが置かれていないかをチェック
		if (NextGrid.nHeight <= CBaseblock::GetHeight(NextGrid.nColumn, NextGrid.nLine))
		{
			if (!PushGrid.nColumn) return;
			break;
		}
		// 列が0超過
		if (Grid.nColumn > 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに加算
			PushGrid.nColumn++;
		}
		// 列が0未満
		else if (Grid.nColumn < 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに減算
			PushGrid.nColumn--;
		}
	}
	for (int nCntLine = 0; nCntLine < abs(Grid.nLine); nCntLine++)
	{
		// 行が0超過
		if (Grid.nLine > 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに加算
			NextGrid.nLine++;
		}
		// 行が0未満
		else if (Grid.nLine < 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに減算
			NextGrid.nLine--;
		}
		// 押し出し方向にブロックが置かれていないかをチェック
		if (NextGrid.nHeight <= CBaseblock::GetHeight(NextGrid.nColumn, NextGrid.nLine))
		{
			if (!PushGrid.nLine) return;
			break;
		}
		// 行が0超過
		if (Grid.nLine > 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに加算
			PushGrid.nLine++;
		}
		// 行が0未満
		else if (Grid.nLine < 0)
		{
			// 行列高さの方向を押し出しブロックの行列高さに減算
			PushGrid.nLine--;
		}
	}
	// 記録更新_押し出し回数
	m_Record.nPushCnt++;
	// 押し出せるブロックなら
	if (PushGrid.nColumn != 0 ||
		PushGrid.nLine != 0)
	{
		// ブロックがパニックブロックなら
		if (pBlock->GetType() == CBaseblock::BLOCKTYPE_PANIC)
		{
			m_Panic.Set(true, m_PlayerStatus.nMaxPanicTime);

			// パニックエフェクトの生成
			PanicEffectCreate();
		}
		// ブロックが電気ブロックなら
		else if (pBlock->GetType() == CBaseblock::BLOCKTYPE_ELECTRIC)
		{
			CElectricblock * pElectBlock = (CElectricblock * )pBlock;
			if (pElectBlock)
			{
				pElectBlock->SetPlayerTag(GetPlayerTag());
			}
		}
		// 移動時の処理
		pBlock->BlockMoveOrDelete();
	}
	// 押し出し後の設定
	pBlock->SetPushAfter(CBaseblock::PUSHAFTER(true, PushGrid));
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 押し出し当たり判定
//	pos		: 位置
//	posOld	: 前回の位置
//	move	: 移動量
//	size	: サイズ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CPlayer::PushCollision(
	CScene::OBJ const & Obj,		// オブジェタイプ
	D3DXVECTOR3 * pos,				// 位置
	D3DXVECTOR3 * posOld,			// 前回の位置
	D3DXVECTOR3 * move,				// 移動量
	D3DXVECTOR3 * size,				// サイズ
	D3DXVECTOR3 const & OffsetPos	// オフセット位置
)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	bool bPush = false;
	// 変数宣言
	D3DXVECTOR3 MyPos = CCharacter::GetPos() + m_PlayerStatus.PlayerOffSet;
	CGame::STAGE Stage = CGame::GetStage();						// ステージ
	// 素材のZ範囲
	if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
	{
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
		{
			// 当たり判定(下)
			if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;

				// 素材状の左に
				pos->y = MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f - size->y * 0.5f - OffsetPos.y;
				if (move)
				{
					// 移動量の初期化
					move->y = 0.0f;
				}
				// 押し出し状態がtrue
				bPush = true;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
				// 素材状の左に
				pos->y = MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f + size->y * 0.5f - OffsetPos.y;
				if (move)
				{
					// 移動量の初期化
					move->y = 0.0f;
				}
				// 押し出し状態がtrue
				bPush = true;

			}
			// 当たり判定(下)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - m_PlayerStatus.PlayerSize.z * 0.5f&&
				pos->y + OffsetPos.y <= MyPos.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f&&
				pos->y + OffsetPos.y - size->y > MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// 当たった方向に情報が入っているなら
	//if (bPush) return Direct;
	// 素材のY範囲
	if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f&&
		pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f)
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
				// 素材状の左に
				pos->x = MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f - size->x * 0.5f - OffsetPos.x;
				if (move)
				{
					// 移動量の初期化
					move->x = 0.0f;
				}
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;

				// 素材状の左に
				pos->x = MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f + size->x * 0.5f - OffsetPos.x;
				if (move)
				{
					// 移動量の初期化
					move->x = 0.0f;
				}
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(左)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// 当たった方向に情報が入っているなら
		//if (bPush) return Direct;
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f)
			{
			}
		}


		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
		{
			// 当たり判定(手前)
			if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;

				// 素材状の左に
				pos->z = MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f - size->z * 0.5f - OffsetPos.z;
				if (move)
				{
					// 移動量の初期化
					move->z = 0.0f;
				}
			}

			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;

				// 素材状の左に
				pos->z =
					MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f +
					size->z * 0.5f - OffsetPos.z;
				if (move)
				{
					// 移動量の初期化
					move->z = 0.0f;
				}
			}
			// 当たり判定(手前)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たり判定
//	pos			: 位置
//	size		: サイズ
//	OffsetPos	: オフセット位置
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
COLLISIONDIRECTION CPlayer::Collision(
	CScene::OBJ const & Obj,		// オブジェタイプ
	D3DXVECTOR3 * pos,				// 位置
	D3DXVECTOR3 * posOld,			// 前回の位置
	D3DXVECTOR3 * size,				// サイズ
	D3DXVECTOR3 const & OffsetPos	// オフセット位置
)
{
	// 変数宣言
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;		// どこの当たり判定か
	bool bPush = false;
	// 変数宣言
	D3DXVECTOR3 MyPos = CCharacter::GetPos();
	CGame::STAGE Stage = CGame::GetStage();						// ステージ

	// 素材のZ範囲
	if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
	{
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
		{
			// 当たり判定(下)
			if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
				// 押し出し状態がtrue
				bPush = true;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
				// 押し出し状態がtrue
				bPush = true;

			}
			// 当たり判定(下)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - m_PlayerStatus.PlayerSize.z * 0.5f&&
				pos->y + OffsetPos.y <= MyPos.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f&&
				pos->y + OffsetPos.y - size->y > MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// 当たった方向に情報が入っているなら
	//if (bPush) return Direct;
	// 素材のY範囲
	if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - m_PlayerStatus.PlayerSize.y * 0.5f&&
		pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + m_PlayerStatus.PlayerSize.y * 0.5f)
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(左)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// 当たった方向に情報が入っているなら
		//if (bPush) return Direct;
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - m_PlayerStatus.PlayerSize.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + m_PlayerStatus.PlayerSize.x * 0.5f)
		{
			// 当たり判定(手前)
			if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;

			}

			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;

			}
			// 当たり判定(手前)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + m_PlayerStatus.PlayerSize.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;
			}
		}
	}

	return Direct;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	// キャラクター描画
	CCharacter::Draw();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 死亡処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Die(void)
{
	// チュートリアル以外のとき死ぬ
	if (CManager::GetMode() != CManager::MODE_TUTORIAL)
	{
			// パーティクル生成
			C3DParticle::Create(
				C3DParticle::PARTICLE_ID_UNKNOWN,
				m_pos
			);

			// 死亡人数の加算
			m_nDieCnt++;

				if (m_pPlayerUI)
				{
					// リリース時にのみ通る
#ifndef _DEBUG
					// バイブレーションの設定
					m_pPad->StartVibration(60);
#endif // _DEBUG

					// プレイヤーUIを開放
					m_pPlayerUI->Release();
					m_pPlayerUI = nullptr;
				}
				// 記録更新_ランキング
				m_Record.nRanking = CCharacter::GetAllCharacter();
				// 記録更新_タイム
				m_Record.nTime = CGame::GetSecond();
				// 死亡を確定させる
				CPlayer::SetDie(true);
				// 死亡処理
				CCharacter::Die();
#ifdef _DEBUG
				// 当たり判定ボックスの開放
					// NULLチェック
				if (pCollisionBox != NULL)
				{
					pCollisionBox->Release();
					pCollisionBox = NULL;
				}
				// 当たり判定線の開放
				// NULLチェック
				if (pCollisionLine != NULL)
				{
					pCollisionLine->Release();
					pCollisionLine = NULL;
				}
#endif // _DEBUG

	}
	// チュートリアルのときリスポーンする
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// パーティクル生成
		C3DParticle::Create(
			C3DParticle::PARTICLE_ID_UNKNOWN,
			m_pos
		);

		// 死亡判定をオフにする ( 復活 )
		SetDie(false);
		// リスポーン位置の設定
		SetPos(RESPAWN_POS);
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 人形の死亡処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::OtherDie(void)
{
	// 死亡処理
	CCharacter::Die();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 当たった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_MyCollision(int const & nObjType, CScene * pScene)
{
	// バルーンキャラクターの当たった後の処理
	CCharacter::Scene_MyCollision(nObjType, pScene);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられた後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_OpponentCollision(int const & nObjType, CScene * pScene)
{
	// キャラクターの相手に当てられた後の処理
	CCharacter::Scene_OpponentCollision(nObjType, pScene);

	// シーン情報がNULLなら
	// ->関数を抜ける
	if (pScene == NULL) return;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自分から当たらなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_NoMyCollision(int const & nObjType, CScene * pScene)
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 相手に当てられなかった後の処理
//	nObjType	: オブジェクトタイプ
//	pScene		: 相手のシーン情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Scene_NoOpponentCollision(int const & nObjType, CScene * pScene)
{

}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Debug(void)
{
	if (CManager::GetKeyboard()->GetKeyboardTrigger(DIK_O))
	{
		m_move.x -= 0.1f;
	}
}
#endif // _DEBUG


#if IMGUI_DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ImGuiのデバッグ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::AllDebugImGui(void)
{

	if (ImGui::Begin(u8"プレイヤーのステータス"))
	{
		//if (ImGui::CollapsingHeader(u8"プレイヤーのステータス"))
		{
			// 移動量
			ImGui::DragFloat(u8"移動量", &m_PlayerStatus.fMove, 0.1f, 0.1f, 100.0f);						/* 3.0f */
																										// ジャンプ力
			ImGui::DragFloat(u8"ジャンプ力", &m_PlayerStatus.fJump, 1.0f, 1.0f, 50.0f);						/* 10.0f */
																										// 通常時の慣性
			ImGui::DragFloat(u8"通常時の慣性", &m_PlayerStatus.fNormalInertia, 0.01f, 0.01f, 10.0f);		/* 0.7f */
																									// ジャンプ時の慣性
			ImGui::DragFloat(u8"ジャンプ時の慣性", &m_PlayerStatus.fJumpInertia, 0.01f, 0.01f, 10.0f);		/* 1.6f */
																									// プレイヤーのサイズ
			ImGui::DragFloat3(u8"プレイヤーのサイズ", m_PlayerStatus.PlayerSize, 1.0f, 0.0f, 500.0f);
			// プレイヤーのオフセット
			ImGui::DragFloat3(u8"プレイヤーのオフセット", m_PlayerStatus.PlayerOffSet, 1.0f, 0.0f, 500.0f);
			// 押し出し用のサイズ
			ImGui::DragFloat(u8"押し出し用のサイズ", &m_PlayerStatus.PushSize, 1.0f, 0.0f, 500.0f);
			// 押し出し用のオフセット
			ImGui::DragFloat3(u8"押し出し用のオフセット", m_PlayerStatus.PushOffSet, 1.0f, 0.0f, 500.0f);
			// 押し出す時間
			ImGui::DragInt(u8"押し出す時間", &m_PlayerStatus.nMaxPowerTime, 1.0f, 0);
			// ブロックに押し出されたとき
			if (ImGui::TreeNode(u8"ブロックに押し出されたときのステータス設定"))
			{
				// スタン時間
				ImGui::DragInt(u8"(b)スタン時間", &m_PlayerStatus.nMaxStanTime[STATUSTYPE_BLOCK], 1.0f, 0);
				// 無敵時間
				ImGui::DragInt(u8"(b)無敵時間", &m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_BLOCK], 1.0f, 0);
				ImGui::TreePop();
			}
			// ジャンプで踏み倒されたとき
			if (ImGui::TreeNode(u8"ジャンプで踏み倒されたときのステータス設定"))
			{
				// スタン時間
				ImGui::DragInt(u8"(j)スタン時間", &m_PlayerStatus.nMaxStanTime[STATUSTYPE_JUMP], 1.0f, 0);
				// 無敵時間
				ImGui::DragInt(u8"(j)無敵時間", &m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_JUMP], 1.0f, 0);
				ImGui::TreePop();
			}
			// 電気で感電したとき
			if (ImGui::TreeNode(u8"電気で感電したときのステータス設定"))
			{
				// スタン時間
				ImGui::DragInt(u8"(e)スタン時間", &m_PlayerStatus.nMaxStanTime[STATUSTYPE_ELECTRIC], 1.0f, 0);
				// 無敵時間
				ImGui::DragInt(u8"(e)無敵時間", &m_PlayerStatus.nMaxInvincibleTime[STATUSTYPE_ELECTRIC], 1.0f, 0);
				ImGui::TreePop();
			}

			// パニック時間
			ImGui::DragInt(u8"パニック時間", &m_PlayerStatus.nMaxPanicTime, 1.0f, 0);
			// 重力を適用するフラグの切り替え
			ImGui::Checkbox(u8"重力", &GetGravity());

			// セーブボタン
			if (ImGui::Button(u8"保存"))
			{
				// プレイヤーのステータスのセーブ
				PlayerStatusSave();
			}

			// 改行キャンセル
			ImGui::SameLine();

			// 初期化
			if (ImGui::Button(u8"初期化"))
			{
				// プレイヤーの初期ステータス代入
				PlayerStatusInitLoad();
			}
			// 区切り線
			ImGui::Separator();
		}
	}
	// End
	ImGui::End();
}
#endif // IMGUI_DEBUG



// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
//	pos	: 位置
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(PLAYER_TAG tag,
	D3DXVECTOR3 const & pos	// 位置
)
{
	// 変数宣言
	CPlayer * pPlayer = nullptr;		// シーン2Dクラス

	switch (tag)
	{
	case PLAYER_TAG::PLAYER_1:
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pPlayer = new CPlayer(CHARACTER_PLAYER_0);
		break;
	case PLAYER_TAG::PLAYER_2:
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pPlayer = new CPlayer(CHARACTER_PLAYER_2);
		break;
	case PLAYER_TAG::PLAYER_3:
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pPlayer = new CPlayer(CHARACTER_PLAYER_1);
		break;
	case PLAYER_TAG::PLAYER_4:
		// メモリの生成(初め->基本クラス,後->派生クラス)
		pPlayer = new CPlayer(CHARACTER_PLAYER_3);
		break;
	}

	if (pPlayer)
	{
		// シーン管理設定
		pPlayer->ManageSetting(CScene::LAYER_CHARACTER);
		// 位置情報
		pPlayer->m_pos = pos;
		// プレイヤータグの設定
		pPlayer->SetPlayerTag(tag);
		// 初期化処理
		pPlayer->Init();
	}

	// 生成したオブジェクトを返す
	return pPlayer;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// リソース情報読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	// プレイヤーのステータスのロード
	PlayerStatusLoad();
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだリソース情報を破棄処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::UnLoad(void)
{
}
