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
#include "stand.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_GRAVITY				(0.1f)		// プレイヤーの重力
#define PLAYER_UPMOVELIMIT			(30.0f)		// プレイヤーの上昇移動量制限
#define PLAYER_UNDERMOVELIMIT		(5.0f)		// プレイヤーの下降移動量制限
#define DASH_TIME_MAX				(30)		// ダッシュしている時間
#define DASH_ENABLE_STICK_RANGE		(0.8f)		// ダッシュを有効にするスティックの傾き
#define PLAYER_STATUS_TXT			("data/LOAD/STATUS/PlayerStatus.txt")	// プレイヤーのステータスのテキスト

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::PLAYER_STATUS CPlayer::m_PlayerStatus = {};

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer::CPlayer(CHARACTER const &character) : CCharacter::CCharacter(character)
{
	m_nCntState			= 0;					// ステートカウント
	m_bDieFlag			= false;				// 死亡フラグ
	m_bDashFlag			= false;				// ダッシュフラグ
	m_nCntDashTime		= 0;					// ダッシュ中の切り替えカウント
	m_PlayerStatusInit	= m_PlayerStatus;		// プレイヤーの初期ステータス
	CScene::SetObj(CScene::OBJ::OBJ_PLAYER);	// オブジェクトタイプの設定
#ifdef _DEBUG
	// 当たり判定ボックスの初期化
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX;nCntCollision++)
	{
		pCollisionBox[nCntCollision] = NULL;
	}
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
	m_pPad = CManager::CManager::GetPad(GetPlayerTag());
#ifdef _DEBUG
	// 当たり判定ボックスの初期化
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		// 当たり判定のタイプがキャラクターなら
		if (nCntCollision == CPlayer::COLLISIONTYPE_CHARACTER)
		{
			// メッシュボックスの生成
			pCollisionBox[nCntCollision] = CMeshBox::Create(m_pos + m_PlayerStatus.PlayerOffSet, m_PlayerStatus.PlayerSize, CMeshBox::COLLISION_TYPE::TYPE_CENTER);
			// 薄青色に
			pCollisionBox[nCntCollision]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
		// 当たり判定のタイプが押し出しなら
		else if (nCntCollision == CPlayer::COLLISIONTYPE_PUSH)
		{
			// メッシュボックスの生成
			pCollisionBox[nCntCollision] = CMeshBox::Create(m_pos + m_PlayerStatus.PushOffSet, m_PlayerStatus.PushSize, CMeshBox::COLLISION_TYPE::TYPE_CENTER);
			// 薄青色に
			pCollisionBox[nCntCollision]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	// キャラクター終了処理
	CCharacter::Uninit();
#ifdef _DEBUG
	// 当たり判定ボックスの初期化
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		pCollisionBox[nCntCollision] = NULL;
	}
#endif // _DEBUG
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// 自キャラの行動処理
	MyAction();

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
		// NULLチェック
		if (pCollisionBox[nCntCollision] == NULL) continue;
		// 当たり判定のタイプがキャラクターなら
		if (nCntCollision == CPlayer::COLLISIONTYPE_CHARACTER)
		{
			// 位置設定
			pCollisionBox[nCntCollision]->SetPos(m_pos + m_PlayerStatus.PlayerOffSet);
			// サイズ設定
			pCollisionBox[nCntCollision]->SetSize(m_PlayerStatus.PlayerSize);
		}
		// 当たり判定のタイプが押し出しなら
		else if (nCntCollision == CPlayer::COLLISIONTYPE_PUSH)
		{
			// 位置設定
			pCollisionBox[nCntCollision]->SetPos(m_pos + m_PlayerStatus.PushOffSet);
			// サイズ設定
			pCollisionBox[nCntCollision]->SetSize(m_PlayerStatus.PushSize);
		}
		// 頂点座標の設定
		pCollisionBox[nCntCollision]->SetVtxPos();
	}
#endif // _DEBUG

	// 死亡判定が出たらリザルトに遷移する
	if (GetDie())
	{	// ゲームのとき
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			// 死亡関数
			Die();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 自キャラ行動処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::MyAction(void)
{
	// 自キャラの移動処理
	MyMove();

	// 試験的ダッシュの切り替え
	if (m_bDashFlag)
	{
		m_nCntDashTime++;

		if (m_nCntDashTime > DASH_TIME_MAX)
		{
			// 初期化
			m_nCntDashTime	= 0;
			m_bDashFlag		= false;
		}
	}
	CDebugproc::Print("\nPlayerダッシュ %d\n", m_bDashFlag);
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

	// 移動 //
	/* キーボード */
	// 左
	if (pKeyboard->GetKeyboardPress(DIK_A))
	{
		// 奥
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = -D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.75f + fRot), 0.0f, cosf(D3DX_PI * 0.75f + fRot));
			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// 手前
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = -D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.25f + fRot), 0.0f, cosf(D3DX_PI * 0.25f + fRot));
			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// 左
		else
		{
			rot.y = -D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(D3DX_PI * 0.5f + fRot), 0.0f, cosf(D3DX_PI * 0.5f + fRot));
			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
	}
	// 右
	else if (pKeyboard->GetKeyboardPress(DIK_D))
	{

		// 奥
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			rot.y = D3DX_PI * 0.25f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f + fRot), 0.0f, cosf(-D3DX_PI * 0.75f + fRot));

			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// 手前
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			rot.y = D3DX_PI * 0.75f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f + fRot), 0.0f, cosf(-D3DX_PI * 0.25f + fRot));


			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
		// 右
		else
		{
			rot.y = D3DX_PI * 0.5f + fRot;
			vec = D3DXVECTOR3(sinf(-D3DX_PI * 0.5f + fRot), 0.0f, cosf(-D3DX_PI * 0.5f + fRot));

			move.x += vec.x * m_PlayerStatus.fMove;
			move.z += vec.z * m_PlayerStatus.fMove;
		}
	}
	// 奥に行く
	else if (pKeyboard->GetKeyboardPress(DIK_W))
	{
		rot.y = D3DX_PI * 0.0f + fRot;
		vec = D3DXVECTOR3(sinf(-D3DX_PI * 1.0f + fRot), 0.0f, cosf(-D3DX_PI * 1.0f + fRot));
		move.x += vec.x * m_PlayerStatus.fMove;
		move.z += vec.z * m_PlayerStatus.fMove;
	}
	// 手前に行く
	else if (pKeyboard->GetKeyboardPress(DIK_S))
	{
		rot.y = D3DX_PI * 1.0f + fRot;
		vec = D3DXVECTOR3(sinf(D3DX_PI * 0.0f + fRot), 0.0f, cosf(D3DX_PI * 0.0f + fRot));
		move.x += vec.x * m_PlayerStatus.fMove;
		move.z += vec.z * m_PlayerStatus.fMove;
	}
	// それ以外
	else
	{

	}

	// 試験的キーボードジャンプ
	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) && GetJumpAble())
	{
		move.y += m_PlayerStatus.fJump;
		SetJumpAble(false);
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

		// ダッシュしていないとき
		if (!m_bDashFlag)
		{

			// プレイヤー移動
			// ゲームパッド移動
			if (fValueH != 0 || fValueV != 0)
			{
				// 角度の計算
				fAngle = atan2f((float)fValueH, (float)fValueV);

				if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}
				else if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				rot.y = fAngle + fRot;
				vec = D3DXVECTOR3(sinf(fAngle + fRot), 0.0f, cosf(fAngle + fRot));
				// スティックの角度によってプレイヤー移動
				move.x -= sinf(fAngle + fRot) * (m_PlayerStatus.fMove);
				move.z -= cosf(fAngle + fRot) * (m_PlayerStatus.fMove);
			}

			// 試験的ジャンプ ( のちに中身変わる予定 多分 )
			if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_A, 1) && GetJumpAble())
			{
				move.y += m_PlayerStatus.fJump;
				SetJumpAble(false);
			}

			// スティックをある程度傾けたとき
			if (fabs(fValueH / STICK_MAX_RANGE) > DASH_ENABLE_STICK_RANGE || fabs(fValueV / STICK_MAX_RANGE) > DASH_ENABLE_STICK_RANGE)
			{
				// 試験的タックル ( のちに中身変わる予定 多分 )
				if (m_pPad->GetTrigger(CXInputPad::XINPUT_KEY::JOYPADKEY_X, 1))
				{
					m_bDashFlag = true;

					switch (CCalculation::CheckPadStick())
					{
					case DIRECTION::LEFT:
					case DIRECTION::RIGHT:
					case DIRECTION::UP:
					case DIRECTION::DOWN:
						move.x -= vec.x * m_PlayerStatus.fDash;
						move.z -= vec.z * m_PlayerStatus.fDash;
						break;
					}
				}
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

	if (vec.x < 0)
	{
		vec.x *= -1;
	}
	if (vec.z < 0)
	{
		vec.z *= -1;
	}
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

	// キャラクターの重力加算処理
	CCharacter::AddGravity(PLAYER_GRAVITY);
	CCharacter::SetRotDest(rot);

	//if (pKeyboard->GetKeyboardTrigger(DIK_C))
	//{
	//	CGame::GetPlayer(1)->SetRotDest(D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));
	//}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 状態によってのモーション設定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StatusMotion(void)
{
	/*
	// ジャンプ中
	if (!CCharacter::GetbLanding())
	{
		SetMotion(MOTIONTYPE_JAMP);
	}
	// 移動中
	else if (CCharacter::GetbMove())
	{
		// モーション設定(移動)
		SetMotion(MOTIONTYPE_MOVE);
	}
	// 待機
	else
	{
		SetMotion(MOTIONTYPE_NEUTRAL);
	}
	*/
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 足場判定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StandJudg(
	CScene_X * pScene_X,
	bool const & bJudg
)
{
	// 足場オブジェクトなら
	if (pScene_X->GetObj() == CScene::OBJ_STAND)
	{
		// プレイヤータグが1プレイヤーなら
		if (this->GetPlayerTag() == PLAYER_TAG::PLAYER_1)
		{
			CStand * pStand = (CStand *)pScene_X;
			pStand->SetDetermination(bJudg);
		}
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
	// 足場の当たり判定
	StandCollision();
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 足場ブロックとの判定
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::StandCollision(void)
{
	// 変数宣言
	CStand * pStand;									// シーンX情報
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;	// 当たり判定の方向
	bool bOn = false;										// 上の当たり判定
	bool bUnder = false;									// 下の当たり判定
															// ブロックループ
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_3DSTAND); nCntBlock++)
	{
		// 当たり判定の初期化
		Direct = COLLISIONDIRECTION::NONE;
		// NULL代入
		pStand = NULL;
		// 情報取得
		pStand = (CStand *)CScene::GetScene(CScene::LAYER_3DSTAND, nCntBlock);
		// NULLなら
		// ->関数を抜ける
		if (pStand == NULL)
		{
			continue;
		}
		/*
		// ダッシュ状態なら
		if (m_bDashFlag)
		{
			// 当たり判定
			Direct = pStand->PushCollision(
				CCharacter::GetObj(),
				&CCharacter::GetPos(),
				&CCharacter::GetPosOld(),
				&CCharacter::GetMove(),
				&m_PlayerStatus.PushSize,
				m_PlayerStatus.PushOffSet
			);
		}
		if (Direct == COLLISIONDIRECTION::NONE)
		{
		*/
			// 当たり判定
			Direct = pStand->PushCollision(
				CCharacter::GetObj(),
				&CCharacter::GetPos(),
				&CCharacter::GetPosOld(),
				&CCharacter::GetMove(),
				&m_PlayerStatus.PlayerSize,
				m_PlayerStatus.PlayerOffSet
			);
		//}
		// ブロックの判定
		// 上
		if (Direct == COLLISIONDIRECTION::UP)
		{
			// ジャンプ可能設定
			SetJumpAble(true);
			// 足場判定
			StandJudg(pStand, true);
			// プレイヤーが下のブロックに当たっている
			bOn ^= true;
		}
		else
		{
			// 足場判定
			StandJudg(pStand, false);
		}
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ブロックの当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::BlockCollision(void)
{
	// 変数宣言
	CBaseblock * pBaseBlock;									// シーンX情報
	COLLISIONDIRECTION Direct = COLLISIONDIRECTION::NONE;	// 当たり判定の方向
	bool bOn = false;										// 上の当たり判定
	bool bUnder = false;									// 下の当たり判定
	int nDieState = 0b000000;
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
		if (pBaseBlock == NULL)
		{
			continue;
		}
		// ダッシュ状態なら
		if (m_bDashFlag)
		{
			// 当たり判定
			Direct = pBaseBlock->PushCollision(
				CCharacter::GetObj(),
				&CCharacter::GetPos(),
				&CCharacter::GetPosOld(),
				&CCharacter::GetMove(),
				&m_PlayerStatus.PushSize,
				m_PlayerStatus.PushOffSet
			);
			// ブロックの判定
			// 前
			if (Direct == COLLISIONDIRECTION::FRONT)
			{
				PushBlock(pBaseBlock, CBaseblock::GRID(0, 0, -1));
			}
			// 後
			else if (Direct == COLLISIONDIRECTION::BACK)
			{
				PushBlock(pBaseBlock, CBaseblock::GRID(0, 0, 1));
			}
			// 左
			else if (Direct == COLLISIONDIRECTION::LEFT)
			{
				PushBlock(pBaseBlock, CBaseblock::GRID(1, 0, 0));
			}
			// 右
			else if (Direct == COLLISIONDIRECTION::RIGHT)
			{
				PushBlock(pBaseBlock, CBaseblock::GRID(-1, 0, 0));
			}
			// 上
			else if (Direct == COLLISIONDIRECTION::UP)
			{
				// ジャンプ可能設定
				SetJumpAble(true);
				// プレイヤーが下のブロックに当たっている
				bOn ^= true;
			}
			// 下
			else if (Direct == COLLISIONDIRECTION::DOWN)
			{
				// プレイヤーがしたブロックに当たっている
				bUnder ^= true;
			}
		}
		if (Direct == COLLISIONDIRECTION::NONE)
		{
			// 当たり判定
			Direct = pBaseBlock->PushCollision(
				CCharacter::GetObj(),
				&CCharacter::GetPos(),
				&CCharacter::GetPosOld(),
				&CCharacter::GetMove(),
				&m_PlayerStatus.PlayerSize,
				m_PlayerStatus.PlayerOffSet
			);
		}
		// ブロックの判定
		// 前
		if (Direct == COLLISIONDIRECTION::FRONT)
		{
			nDieState |= DIESTATE_FRONT;
		}
		// 後
		else if (Direct == COLLISIONDIRECTION::BACK)
		{
			nDieState |= DIESTATE_BACK;
		}
		// 左
		else if (Direct == COLLISIONDIRECTION::LEFT)
		{
			nDieState |= DIESTATE_LEFT;
		}
		// 右
		else if (Direct == COLLISIONDIRECTION::RIGHT)
		{
			nDieState |= DIESTATE_RIGHT;
		}
		// 上
		else if (Direct == COLLISIONDIRECTION::UP)
		{
			// ジャンプ可能設定
			SetJumpAble(true);
			nDieState |= DIESTATE_UP;
		}
		// 下
		else if (Direct == COLLISIONDIRECTION::DOWN)
		{
			nDieState |= DIESTATE_UNDER;
		}
	}
	// 挟まったら死ぬ処理
	if (nDieState == (DIESTATE_UP + DIESTATE_UNDER)		||
		nDieState == (DIESTATE_LEFT + DIESTATE_RIGHT)	||
		nDieState == (DIESTATE_FRONT + DIESTATE_BACK)
		)
	{
		Die();
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// キャラクター同士の当たり判定処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::CharacterCollision(void)
{
	// 変数宣言
	CCharacter * pCharacter;	// キャラクター情報
	COLLISIONDIRECTION Direct;	// 当たり判定の方向
	bool bOn = false;			// 上の当たり判定
	bool bUnder = false;		// 下の当たり判定
								// ブロックループ
	for (int nCntBlock = 0; nCntBlock < CScene::GetMaxLayer(CScene::LAYER_CHARACTER); nCntBlock++)
	{
		// NULL代入
		pCharacter = NULL;
		// 情報取得
		pCharacter = (CCharacter *)CScene::GetScene(CScene::LAYER_CHARACTER, nCntBlock);
		// NULLなら
		// ->関数を抜ける
		if (pCharacter == NULL)
		{
			continue;
		}
		// 当たり判定
		Direct = pCharacter->PushCollision(
			CCharacter::GetObj(),
			&CCharacter::GetPos(),
			&CCharacter::GetPosOld(),
			&CCharacter::GetMove(),
			&m_PlayerStatus.PlayerSize,
			m_PlayerStatus.PlayerOffSet
		);
	}
	// 上も下もブロックに当たっていたら
	if (bOn && bUnder)
	{
		// プレイヤーは死ぬ
		Die();
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
						// Dashが来たら
						else if (strcmp(cHeadText, "Dash") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDie, &cDie, &m_PlayerStatus.fDash);
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
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PushSize.x, &m_PlayerStatus.PushSize.y, &m_PlayerStatus.PushSize.z);
						}
						// PushOffSetが来たら
						else if (strcmp(cHeadText, "PushOffSet") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie, &m_PlayerStatus.PushOffSet.x, &m_PlayerStatus.PushOffSet.y, &m_PlayerStatus.PushOffSet.z);
						}
						else if (strcmp(cHeadText, "END_BULLETSET") == 0)
						{
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
		fprintf(pFile, "	Dash			= %.1f\n", m_PlayerStatus.fDash);
		fprintf(pFile, "	NormalInertia	= %.2f\n", m_PlayerStatus.fNormalInertia);
		fprintf(pFile, "	JumpInertia		= %.2f\n", m_PlayerStatus.fJumpInertia);
		fprintf(pFile, "	PlayerSize		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PlayerSize.x, m_PlayerStatus.PlayerSize.y, m_PlayerStatus.PlayerSize.z);
		fprintf(pFile, "	PlayerOffSet	= %.1f	%.1f	%.1f\n", m_PlayerStatus.PlayerOffSet.x, m_PlayerStatus.PlayerOffSet.y, m_PlayerStatus.PlayerOffSet.z);
		fprintf(pFile, "	PushSize		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PushSize.x, m_PlayerStatus.PushSize.y, m_PlayerStatus.PushSize.z);
		fprintf(pFile, "	PushOffSet		= %.1f	%.1f	%.1f\n", m_PlayerStatus.PushOffSet.x, m_PlayerStatus.PushOffSet.y, m_PlayerStatus.PushOffSet.z);
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
// ブロックの押し出し処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPlayer::PushBlock(
	CScene_X * pScene_X,			// シーンX情報
	CBaseblock::GRID const & Grid	// 方向
)
{
	// 足場オブジェクトなら
	// ->関数を抜ける
	if (pScene_X->GetObj() != CScene::OBJ_BLOCK) return;
	// プレイヤーがダッシュ時以外なら
	// ->関数を抜ける
	if (!CPlayer::GetDashFlag()) return;

	// 変数宣言
	CBaseblock * pBlock = (CBaseblock *)pScene_X;					// ベースブロックの情報
	this->SetPos(this->GetPos() + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	// 落ちている状態なら
	if (pBlock->GetFall() ||
		pBlock->GetType() == CBaseblock::TYPE_FIELD) return;
	CBaseblock::GRID MyGrid = pBlock->GetGrid();					// 押し出しブロックの行列高情報
	int nHeight;													// 高さ
	int nFeedValue = CBaseblock::GetFeedValue(CGame::GetStage());	// フェードの値
	// 行列高さの方向を押し出しブロックの行列高さに加算
	MyGrid += Grid;
	// その行列の積み重なっている高さを取得
	nHeight = CBaseblock::GetHeight(MyGrid.nColumn + nFeedValue, MyGrid.nLine + nFeedValue);
	if (MyGrid.nHeight <= CBaseblock::GetHeight(MyGrid.nColumn + nFeedValue, MyGrid.nLine + nFeedValue))
	{
		return;
	}

	// 変数宣言
	CBaseblock::GRID FallGrid = pBlock->GetGrid();	// 行列高
	CBaseblock::GRID PlayerGrid = pBlock->GetGrid() - Grid;
	// 押す前のブロックの上にあったブロックを落とさせる
	CBaseblock::FallBlock_Grid(FallGrid);
	// 押したブロックの現在までいた行列の高さ情報を更新
	CBaseblock::SetHeight(pBlock->GetGrid() + CBaseblock::GRID(nFeedValue,-1, nFeedValue));

	// 動いた後の行列高を設定
	pBlock->SetGrid(MyGrid);
	// 動いた後の位置設定
	pBlock->SetPos(MyGrid.GetPos(CBaseblock::GetSizeRange()));
	// 押し出した後のブロックに当たったプレイヤーの押し出し判定処理
	PushAfter_Collision(pBlock, Grid);
	// 落ちる処理
	pBlock->SetFall(true);
	// 押したブロックの動いた後の行列の高さ情報を更新
	//CBaseblock::SetHeight(MyGrid + CBaseblock::GRID(nFeedValue, 0, nFeedValue));
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
	D3DXVECTOR3 MyPos = CCharacter::GetPos();
	CGame::STAGE Stage = CGame::GetStage();						// ステージ

	D3DXVECTOR3 Kari_Size = { 25.0f,50.0f,25.0f };	// 仮サイズ
		// 素材のZ範囲
	if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
	{
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// 当たり判定(下)
			if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= MyPos.y - Kari_Size.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;

				// 素材状の左に
				pos->y = MyPos.y - Kari_Size.y * 0.5f - size->y * 0.5f - OffsetPos.y;

				// 移動量の初期化
				move->y = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= MyPos.y + Kari_Size.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
				// 素材状の左に
				pos->y = MyPos.y + Kari_Size.y * 0.5f + size->y * 0.5f - OffsetPos.y;
				// 移動量の初期化
				move->y = 0.0f;
				// 押し出し状態がtrue
				bPush = true;

			}
			// 当たり判定(下)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.z * 0.5f&&
				pos->y + OffsetPos.y <= MyPos.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				pos->y + OffsetPos.y - size->y > MyPos.y + Kari_Size.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// 当たった方向に情報が入っているなら
	//if (bPush) return Direct;
	// 素材のY範囲
	if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
		pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f)
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
				// 素材状の左に
				pos->x = MyPos.x - Kari_Size.x * 0.5f - size->x * 0.5f - OffsetPos.x;
				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;

				// 素材状の左に
				pos->x = MyPos.x + Kari_Size.x * 0.5f + size->x * 0.5f - OffsetPos.x;

				// 移動量の初期化
				move->x = 0.0f;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(左)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// 当たった方向に情報が入っているなら
		//if (bPush) return Direct;
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// 当たり判定(手前)
			if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;

				// 素材状の左に
				pos->z = MyPos.z - Kari_Size.z * 0.5f - size->z * 0.5f - OffsetPos.z;

				// 移動量の初期化
				move->z = 0.0f;
			}

			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;

				// 素材状の左に
				pos->z =
					MyPos.z + Kari_Size.z * 0.5f +
					size->z * 0.5f - OffsetPos.z;

				// 移動量の初期化
				move->z = 0.0f;
			}
			// 当たり判定(手前)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
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

	D3DXVECTOR3 Kari_Size = { 25.0f,50.0f,25.0f };	// 仮サイズ
													// 素材のZ範囲
	if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
		pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
	{
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// 当たり判定(下)
			if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y + size->y * 0.5f <= MyPos.y - Kari_Size.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
				// 押し出し状態がtrue
				bPush = true;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				posOld->y + OffsetPos.y - size->y * 0.5f >= MyPos.y + Kari_Size.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
				// 押し出し状態がtrue
				bPush = true;

			}
			// 当たり判定(下)
			else if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.z * 0.5f&&
				pos->y + OffsetPos.y <= MyPos.y)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::DOWN;
			}

			// 当たり判定(上)
			else if (pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f&&
				pos->y + OffsetPos.y - size->y > MyPos.y + Kari_Size.y * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::UP;
			}
		}
	}
	// 当たった方向に情報が入っているなら
	//if (bPush) return Direct;
	// 素材のY範囲
	if (pos->y + OffsetPos.y + size->y * 0.5f > MyPos.y - Kari_Size.y * 0.5f&&
		pos->y + OffsetPos.y - size->y * 0.5f < MyPos.y + Kari_Size.y * 0.5f)
	{
		// 素材のZ範囲
		if (pos->z + OffsetPos.z + size->z * 0.5f >= MyPos.z - Kari_Size.z * 0.5f&&
			pos->z + OffsetPos.z - size->z * 0.5f <= MyPos.z + Kari_Size.z * 0.5f)
		{
			// 当たり判定(左)
			if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
				// 押し出し状態がtrue
				bPush = true;
			}
			// 当たり判定(左)
			else if (pos->x + OffsetPos.x + size->x * 0.5f > MyPos.x - Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x + size->x * 0.5f <= MyPos.x - Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::LEFT;
			}

			// 当たり判定(右)
			else if (pos->x + OffsetPos.x - size->x * 0.5f < MyPos.x + Kari_Size.x * 0.5f&&
				posOld->x + OffsetPos.x - size->x * 0.5f >= MyPos.x + Kari_Size.x * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::RIGHT;
			}
		}
		// 当たった方向に情報が入っているなら
		//if (bPush) return Direct;
		// 素材のX範囲
		if (pos->x + OffsetPos.x + size->x * 0.5f >= MyPos.x - Kari_Size.x * 0.5f&&
			pos->x + OffsetPos.x - size->x * 0.5f <= MyPos.x + Kari_Size.x * 0.5f)
		{
			// 当たり判定(手前)
			if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;

			}

			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::FRONT;

			}
			// 当たり判定(手前)
			else if (pos->z + OffsetPos.z + size->z * 0.5f > MyPos.z - Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z + size->z * 0.5f <= MyPos.z - Kari_Size.z * 0.5f)
			{
				// めり込んでいる
				Direct = COLLISIONDIRECTION::BACK;
			}
			// 当たり判定(奥)
			else if (pos->z + OffsetPos.z - size->z * 0.5f < MyPos.z + Kari_Size.z * 0.5f&&
				posOld->z + OffsetPos.z - size->z * 0.5f >= MyPos.z + Kari_Size.z * 0.5f)
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
	// 死亡処理
	CCharacter::Die();
#ifdef _DEBUG
	// 当たり判定ボックスの開放
	for (int nCntCollision = 0; nCntCollision < CPlayer::COLLISIONTYPE_MAX; nCntCollision++)
	{
		// NULLチェック
		if (pCollisionBox[nCntCollision] == NULL) continue;
		pCollisionBox[nCntCollision]->Release();
		pCollisionBox[nCntCollision] = NULL;
	}
#endif // _DEBUG
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
	// プレイヤー1人だけ通す(ステータスが共通のため)
	if (GetPlayerTag() == PLAYER_TAG::PLAYER_1)
	{
		if (ImGui::Begin(u8"プレイヤーのステータス"))
		{
			//if (ImGui::CollapsingHeader(u8"プレイヤーのステータス"))
			{
				// 移動量
				ImGui::DragFloat(u8"移動量", &m_PlayerStatus.fMove, 0.1f, 0.1f, 100.0f);						/* 3.0f */
				// ジャンプ力
				ImGui::DragFloat(u8"ジャンプ力", &m_PlayerStatus.fJump, 1.0f, 1.0f, 50.0f);						/* 10.0f */
				// ダッシュの移動量
				ImGui::DragFloat(u8"ダッシュの移動量", &m_PlayerStatus.fDash, 1.0f, 1.0f, 100.0f);				/* 30.0f */
				// 通常時の慣性
				ImGui::DragFloat(u8"通常時の慣性", &m_PlayerStatus.fNormalInertia, 0.01f, 0.01f, 10.0f);		/* 0.7f */
				// ジャンプ時の慣性
				ImGui::DragFloat(u8"ジャンプ時の慣性", &m_PlayerStatus.fJumpInertia, 0.01f, 0.01f, 10.0f);		/* 1.6f */
				// プレイヤーのサイズ
				ImGui::DragFloat3(u8"プレイヤーのサイズ", m_PlayerStatus.PlayerSize, 1.0f, 0.0f, 500.0f);
				// プレイヤーのオフセット
				ImGui::DragFloat3(u8"プレイヤーのオフセット", m_PlayerStatus.PlayerOffSet, 1.0f, 0.0f, 500.0f);
				// 押し出し用のサイズ
				ImGui::DragFloat3(u8"押し出し用のサイズ", m_PlayerStatus.PushSize, 1.0f, 0.0f, 500.0f);
				// 押し出し用のオフセット
				ImGui::DragFloat3(u8"押し出し用のオフセット", m_PlayerStatus.PushOffSet, 1.0f, 0.0f, 500.0f);

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
}
#endif // _DEBUG

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
//	pos	: 位置
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(PLAYER_TAG tag,
	D3DXVECTOR3 const & pos	// 位置
)
{
	// 変数宣言
	CPlayer * pPlayer;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pPlayer = new CPlayer(CHARACTER_PLAYER);
	// シーン管理設定
	pPlayer->ManageSetting(CScene::LAYER_CHARACTER);
	// 位置情報
	pPlayer->m_pos = pos;
	// プレイヤータグの設定
	pPlayer->SetPlayerTag(tag);
	// 初期化処理
	pPlayer->Init();
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
