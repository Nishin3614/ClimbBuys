// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル情報処理 [model_info.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _MODEL_INFO_H_
#define _MODEL_INFO_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Calculation.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モーションエフェクト
typedef struct MOTION_EFFECT
{
	MOTION_EFFECT()
	{
		nStart = 0;			// 開始時間
		nEnd = 0;			// 終了時間
		nKeyID = 0;			// キーID
		nParticleType = 0;	// パーティクルタイプ
		offset = D3DVECTOR3_ZERO;
	}
	int nStart;			// 開始時間
	int nEnd;			// 終了時間
	int nKeyID;			// キーID
	int nParticleType;	// パーティクルタイプ
	D3DXVECTOR3 offset;	// 位置
} MOTION_EFFECT;

// モーションカメラ
typedef struct MOTION_CAMERA
{
	MOTION_CAMERA()
	{
		offsetR = D3DVECTOR3_ZERO;	// 注視点のオフセット
		fLength = 0.0f;				// 長さ
		fHeight = 0.0f;				// 高さ
		rot = D3DVECTOR3_ZERO;		// 回転
		fIntertia = 0.0f;			// 慣性
	}
	D3DXVECTOR3 offsetR;	// 注視点のオフセット
	float		fLength;	// 長さ
	float		fHeight;	// 高さ
	D3DXVECTOR3 rot;		// 回転
	float fIntertia;		// 慣性
} MOTION_CAMERA;

// 軌跡の詳細情報
typedef struct MESHOBIT_DETAILS
{
	// コンストラクタ
	MESHOBIT_DETAILS() = default;
	int	nObitID;				// 軌跡のID
	int nBeginFram;				// 始点フレーム
	int nEndFram;				// 終点フレーム
	D3DXCOLOR BeginCol;			// 始点色
	D3DXCOLOR EndCol;			// 終点色
} MESHOBIT_DETAILS;

// キー
typedef struct KEY
{
	KEY()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
	}
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 rot;						// 回転
} KEY;

// キー情報
typedef struct KEY_INFO
{
	KEY_INFO()
	{
		nFrame = 0;
		Key = NULL;
		pMotionCamera = NULL;
	}
	int	nFrame;									// フレーム
	std::vector<MOTION_EFFECT> v_MotionEffect;		// モーションエフェクト
	MOTION_CAMERA * pMotionCamera;				// モーションカメラ
	KEY *Key;									// キー
} KEY_INFO;

// モージョン情報
typedef struct MOTION
{
	MOTION()
	{
		nLoop = 0;								// ループ
		nNumKey = 0;							// キー情報の数
		nAllFrame = 0;							// モーション全体のフレーム数
		KeyInfo = NULL;							// キー情報(モーション数)
	}
	int			nLoop;							// ループ
	int			nNumKey;						// キー情報の数
	int			nAllFrame = 0;					// モーション全体のフレーム数
	std::vector<MESHOBIT_DETAILS> v_MeshObit_detail;	// 軌跡の詳細
	KEY_INFO	*KeyInfo;						// キー情報(モーション数)
} MOTION;

// オフセットの情報
typedef struct MODEL_OFFSET
{
	MODEL_OFFSET()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
		nParent = 0;
	}
	D3DXVECTOR3	pos;						// 位置
	D3DXVECTOR3	rot;						// 回転
	int			nParent;					// 親番号
} MODEL_OFFSET;

// 軌跡の基本情報
typedef struct MESHOBIT_BASIC
{
	// コンストラクタ
	MESHOBIT_BASIC() = default;
	int nLine;					// 線の数
	int nPart;					// パーツ
	D3DXVECTOR3 BeginOffset;	// 始点オフセット
	D3DXVECTOR3 EndOffset;		// 終点オフセット
	int nTexType;				// テクスチャータイプ
} MESHOBIT_BASIC;

// モデル・モーションのの情報
typedef struct MODEL_ALL
{
	// コンストラクタ
	MODEL_ALL()
	{
		pModel_offset = NULL;	// モデルのオフセット
	}
	MODEL_OFFSET			*pModel_offset;			// モデルのオフセット
	std::vector<MOTION *>		pMotion;				// モーション
	std::vector<MESHOBIT_BASIC>	v_MeshObitLoad;			// 軌跡の情報
} MODEL_ALL;

// モデル情報
typedef struct XMODEL
{
	// コンストラクタ
	XMODEL()
	{
		pMesh = NULL;
		pBuffMat = NULL;
		ppTexture = NULL;
		nNumMat = 0;
		vtxMinMaterials = D3DVECTOR3_ZERO;
		vtxMaxMaterials = D3DVECTOR3_ZERO;
		pos = D3DVECTOR3_ZERO;
		posDest = D3DVECTOR3_ZERO;
		posDiff = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
		rotDest = D3DVECTOR3_ZERO;
		rotDiff = D3DVECTOR3_ZERO;
		size = D3DVECTOR3_ZERO;
		nParent = 0;
	}
	LPD3DXMESH			pMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;			// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9	*ppTexture;			// テクスチャへのダブルポインター
	DWORD				nNumMat;			// マテリアルの数
	D3DXVECTOR3			vtxMinMaterials;	// モデル情報の位置の最小値
	D3DXVECTOR3			vtxMaxMaterials;	// モデル情報の位置の最大値
	D3DXVECTOR3			pos;				// 位置情報
	D3DXVECTOR3			posDest;			// 目的位置
	D3DXVECTOR3			posDiff;			// 差位置
	D3DXVECTOR3			rot;				// 回転情報
	D3DXVECTOR3			rotDest;			// 目的回転
	D3DXVECTOR3			rotDiff;			// 差回転
	D3DXVECTOR3			size;				// サイズ
	int					nParent;			// 親番号
} XMODEL;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CModel_info
{
public:
	/* 関数 */
	CModel_info();
	~CModel_info();
	// モデルのテキストデータ読込
	static void TextLoad(
		MODEL_ALL	*pmodelAll,					// モデルとモーションの
		std::vector<int> &ModelFile,					// モデルファイルの情報
		int const &nMaxMotion,					// モーション数
		int &nMaxkey,							// キー数
		int &nMaxParts,							// 動かすモデル数
		const char * file_name					// ファイル名
	);
	// モデルのテキストデータ読込
	static void TextLoad(
		MODEL_ALL	*pmodelAll,					// モデルとモーションの
		std::vector<int> &ModelFile,					// モデルファイルの情報
		int &nMaxkey,							// キー数
		int &nMaxParts,							// 動かすモデル数
		const char * file_name					// ファイル名
	);
	// 読み込んだモデルのテキストデータの破棄
	static void TextUnload(
		MODEL_ALL *pmodelAll					// モデルとモーションの
		);
protected:
private:
};

#endif
