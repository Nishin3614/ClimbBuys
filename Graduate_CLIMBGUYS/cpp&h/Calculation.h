// ----------------------------------------------------------------------------------------------------
//
// Calculationヘッダー処理の説明[calculation.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // ファイル名を基準を決める

// ----------------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"
#include "shape.h"

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
// D3DVECTOR2型の初期化(1.0f)
#define D3DVECTOR2_INI			D3DXVECTOR2(1.0f,1.0f)
// D3DVECTOR2型の初期化(0.0f)
#define D3DVECTOR2_ZERO			D3DXVECTOR2(0.0f,0.0f)
// D3DVECTOR3型の初期化(0.0f)
#define D3DVECTOR3_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)
// D3DVECTOR3型の初期化(1.0f)
#define D3DVECTOR3_ONE			D3DXVECTOR3(1.0f,1.0f,1.0f)
// D3DVECTOR4型の初期化(0.0f)
#define D3DVECTOR4_ZERO			D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)
// D3DXCOLOR型の初期化
#define D3DXCOLOR_INI			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
// D3DXCOLOR型の初期化(基本色)
#define D3DXCOLOR_C(col)		D3DXCOLOR(col,col,col,1.0f)
// D3DXCOLOR型の初期化(基本色,透明度)
#define D3DXCOLOR_CA(col,alpha)	D3DXCOLOR(col,col,col,alpha)
// ファイル読み込み最大行数
#define FILELINE_ERROW (1048576)
// 画面のサイズ
#define SCREEN_SIZE				(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT))
// 画面中央の座標
#define SCREEN_CENTER_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f ,0.0f))

/* テキスト用 */
// 改行
#define NEWLINE				("\n")
// テキスト用　コメント　改行
#define COMMENT01			("//\n")
// テキスト用　線
#define COMMENT02			("//------------------------------------------------------------\n")
// テキスト用　イコール
#define EQUAL				("=")

// ----------------------------------------------------------------------------------------------------
//
// Calculation種類
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 列挙型定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// 名前定義
//
// ----------------------------------------------------------------------------------------------------
namespace MYLIBLARY
{
	void in(void);
	// 書き換え不可の文字ポインター
	typedef const char * P_CST_CHAR;

}

// 方向
enum class DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

// 当たり判定の向き
enum class COLLISIONDIRECTION
{
	NONE,	// なし
	FRONT,	// 前
	BACK,	// 後
	LEFT,	// 左
	RIGHT,	// 右
	UP,		// 上
	DOWN	// 下
};

// タグ
enum class PLAYER_TAG
{
	NONE = -1,
	PLAYER_1,			// プレイヤー1
	PLAYER_2,			// プレイヤー2
	PLAYER_3,			// プレイヤー3
	PLAYER_4,			// プレイヤー4
	PLAYER_MAX,			// 最大数
};
// タグを管理するクラス
class CPlayerTAG
{
public:
	void				SetPlayerTag(PLAYER_TAG Tag)	{ m_PlayerTag = Tag; };		// コントローラータグの設定
	PLAYER_TAG			GetPlayerTag()					{ return m_PlayerTag; };	// コントローラータグの取得

private:
	PLAYER_TAG			m_PlayerTag;	// コントローラータグ
};

// 整数型2個
typedef struct INTEGER2
{
	INTEGER2() {}
	INTEGER2(int X, int Y)
	{
		nMax = X;
		nMin = Y;
	}
	int nMax;
	int	nMin;
}INTEGER2, *PINTEGER2;

// 整数型3個
typedef struct INTEGER3
{
public:
	INTEGER3() {}
	INTEGER3(int X, int Y, int Z)
	{
		nX = X;
		nY = Y;
		nZ = Z;
	}
	// キャスト
	inline operator int* ()
	{
		(int *)nX;
		(int *)nY;
		(int *)nX;
		return *this;
	}
	inline operator CONST int* () const
	{
		(CONST int *)nX;
		(CONST int *)nY;
		(CONST int *)nZ;
		return *this;
	}
	// 代入演算子
	INTEGER3& operator += (CONST INTEGER3& Source)
	{
		nX += Source.nX;
		nY += Source.nY;
		nZ += Source.nZ;
		return *this;
	}
	INTEGER3& operator -= (CONST INTEGER3& Source)
	{
		nX -= nX;
		nY -= nY;
		nZ -= nZ;
		return *this;
	}
	INTEGER3& operator *= (int Source)
	{
		nX *= Source;
		nY *= Source;
		nZ *= Source;
		return *this;
	}
	INTEGER3& operator /= (int Source)
	{
		nX /= Source;
		nY /= Source;
		nZ /= Source;
		return *this;
	}
	// 2項演算子
	INTEGER3 operator + (CONST INTEGER3& Source) const
	{
		// 値を渡す変数
		INTEGER3 variable;
		variable.nX = nX + Source.nX;
		variable.nY = nY + Source.nY;
		variable.nZ = nZ + Source.nZ;
		return variable;
	}
	INTEGER3 operator - (CONST INTEGER3& Source) const
	{
		// 値を渡す変数
		INTEGER3 variable;
		variable.nX = nX - Source.nX;
		variable.nY = nY - Source.nY;
		variable.nZ = nZ - Source.nZ;
		return variable;
	}
	bool operator == (CONST INTEGER3& Source) const
	{
		return Source == INTEGER3(nX, nY, nZ);
	}
	bool operator != (CONST INTEGER3& Source) const
	{
		return Source != INTEGER3(nX, nY, nZ);
	}

	int nX,		// X軸成分
		nY,		// Y軸成分
		nZ;		// Z軸成分
} INTEGER3, *PINTEGER3;

// 整数型4個
typedef struct INTEGER4
{
	INTEGER4() {}
	INTEGER4(
		int d_X,
		int d_Y,
		int d_Z,
		int d_W
	)
	{
		X = d_X;
		Y = d_Y;
		Z = d_Z;
		W = d_W;
	}
	int X;
	int	Y;
	int Z;
	int	W;
}INTEGER4, *PINTEGER4;

// 配置オブジェクト情報(1:タイプ,2:位置,3:回転)
typedef struct ARRANGEMENTOBJ
{
	ARRANGEMENTOBJ() {}
	ARRANGEMENTOBJ(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} ARRANGEMENTOBJ, *pARRANGEMENTOBJ;

// 配置ポリゴン情報(1:タイプ,2:位置,3:回転,4:サイズA,5:サイズB)
typedef struct ARRANGEMENTPOLYGON
{
	ARRANGEMENTPOLYGON() {}
	ARRANGEMENTPOLYGON(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot,
		float		d_fSizeA,
		float		d_fSizeB
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
		fSizeA = d_fSizeA;
		fSizeB = d_fSizeB;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	float		fSizeA;
	float		fSizeB;
} ARRANGEMENTPOLYGON, *pARRANGEMENTPOLYGON;

// 配置メッシュ情報(1:タイプ,2:位置,3:回転4:縦ブロック,5:横ブロック,6:サイズA,7:サイズB)
typedef struct ARRANGEMENTMESH
{
	ARRANGEMENTMESH() {}
	ARRANGEMENTMESH(
		int d_nType,
		D3DXVECTOR3 d_pos,
		D3DXVECTOR3 d_rot,
		int			d_nBlockDepth,
		int			d_nBlockWidth,
		float		d_fSizeA,
		float		d_fSizeB
	)
	{
		nType = d_nType;
		pos = d_pos;
		rot = d_rot;
		nBlockDepth = d_nBlockDepth;
		nBlockWidth = d_nBlockWidth;
		fSizeA = d_fSizeA;
		fSizeB = d_fSizeB;
	}
	int			nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int			nBlockDepth;
	int			nBlockWidth;
	float		fSizeA;
	float		fSizeB;
} ARRANGEMENTMESH, *pARRANGEMENTMESH;

// ファイル格納用変数
typedef struct CHARFILE
{
	char		cFile
		[64];		// モデルのファイル名
} CHARFILE;

// アニメーション
typedef struct ANIMATION
{
	ANIMATION()
	{
		nCntAnim = 0;			// アニメカウント
		nMaxCntAnim = 0;		// 最大アニメカウント
		nHorizonAnim = 0;		// 水平のアニメーションパターン
		nVirticalAnim = 0;		// 垂直のアニメーションパターン
		nMaxHorizonAnim = 1;	// 最大水平アニメーションパターン数
		nMaxVirticalAnim = 1;	// 最大垂直のアニメーションパターン数
		fHorizonSize = 1.0f;	// 水平のアニメーション1つのサイズ
		fVirticalSize = 1.0f;	// 垂直のアニメーション1つのサイズ
		bLoop = true;			// ループ状態
	}
	int					nCntAnim;			// アニメカウント
	int					nMaxCntAnim;		// 最大アニメカウント
	int					nHorizonAnim;		// 水平のアニメーションパターン
	int					nVirticalAnim;		// 垂直のアニメーションパターン
	int					nMaxHorizonAnim;	// 最大水平アニメーションパターン数
	int					nMaxVirticalAnim;	// 最大垂直のアニメーションパターン数
	float				fHorizonSize;		// 水平のアニメーション1つのサイズ
	float				fVirticalSize;		// 垂直のアニメーション1つのサイズ
	bool				bLoop;				// ループ状態
} ANIMATION, *PANIMATION;

// ----------------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ----------------------------------------------------------------------------------------------------
#define INTEGER4_INI			INTEGER4(1,1,1,1)

// ----------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------
class CCalculation
{
public:
	/* 関数 */
	// unique_ptr[int]型の情報を返す
	// 注意:返し元はstd::moveを使用すること
	static std::unique_ptr<int> int_ptr(
		int const nNum = 0
	);
	// unique_ptr[bool]型の情報を返す
	// 注意:返し元はstd::moveを使用すること
	static std::unique_ptr<bool> bool_ptr(void);
	// 距離の計算
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,
		D3DXVECTOR3 &posB
	);
	// 2点の距離を絶対値をで求める
	static float Range_Absolute(
		const float & posA,
		const float & posB
	);

	// 間の差の計算(3D用:位置、位置)
	static float DiffPoint(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// 間の差の計算(2D用:位置、位置)
	static float DiffPoint(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);
	// 間の差の2乗計算(3D用:位置、位置)
	static float DiffPointSquare(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// 間の差の2乗計算(2D用:位置、位置)
	static float DiffPointSquare(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);

	// 外積の計算
	static D3DXVECTOR3 Cross_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// 内積の計算
	static float Dot_product(
		D3DXVECTOR3 const & VecA,	// VectorA
		D3DXVECTOR3 const & VecB	// VectorB
	);
	// 四角形の中に入っているかいないか
	static bool SquareBottom_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// 四角形の中に入っているかいないか
	static bool SquareSide_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// 球の足し算
	static float Addition_Sphere(
		float Length_x,
		float Length_y,
		float Length_z
	);
	// 円の当たり判定
	static bool Collision_Circle(
		D3DXVECTOR2 &Me,
		float		fMeSize,
		D3DXVECTOR2 &Opponent,
		float		fOppSize
	);
	// 球の当たり判定
	static bool Collision_Sphere(
		D3DXVECTOR3 const &Me,
		float const &fMeSize,
		D3DXVECTOR3 const &Opponent,
		float const &fOppSize
	);
	// 回転量の上限 //
	static float Rot_One_Limit(
		float &fRot
	);
	// スクリーン座標をワールド座標に変換
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj	// プロジェクションマトリックス
	);
	// スクリーン座標をXZ平面のワールド座標交点算出
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,		// マウスポイント
		float Sx,				// スクリーンX座標
		float Sy,				// スクリーンY座標
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj,		// プロジェクションマトリックス
		D3DXVECTOR3 obj			// オブジェクトの位置
	);
	// ワールド座標をスクリーン座標に変換(多分)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// オブジェクトの位置
		float Sx,					// スクリーンX座標
		float Sy,					// スクリーンY座標
		float fZ,				// 射影空間でのZ値（0～1）
		float Screen_w,			// スクリーンの横幅
		float Screen_h,			// スクリーンの高さ
		D3DXMATRIX* mtxView,	// ビューマトリックス
		D3DXMATRIX* mtxPrj	// プロジェクションマトリックス
	);
	// 一次関数
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 Apos,			// 始点
		D3DXVECTOR3 Bpos			// 終点
	);
	// 2線分の交点(連立方程式)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 ALinear,	// 線①
		D3DXVECTOR2 BLinear	// 線②
	);
	// ポリゴンと線分の当たり判定処理
	//	PolygonVtxA	: ポリゴン頂点
	//	PolygonVtxB	: ポリゴン頂点
	//	PolygonVtxC	: ポリゴン頂点
	//	PolygonVtxD	: ポリゴン頂点
	// 	PolygonNor	: ポリゴン法線
	// 	LineBegin	: 線の始点
	// 	LineEnd		: 線の終点
	//	fDistance	: 距離
	//	Direction	: 方向
	static bool PolygonToLineCollision(
		D3DXVECTOR3 const &			PolygonVtxA,	//	PolygonVtx	: ポリゴン頂点
		D3DXVECTOR3 const &			PolygonVtxB,	//	PolygonVtx	: ポリゴン頂点
		D3DXVECTOR3 const &			PolygonVtxC,	//	PolygonVtx	: ポリゴン頂点
		D3DXVECTOR3 const &			PolygonVtxD,	//	PolygonVtx	: ポリゴン頂点
		D3DXVECTOR3 const &			PolygonNor,		// 	PolygonNor	: ポリゴン法線
		D3DXVECTOR3 const &			LineBegin,		// 	LineBegin	: 線の始点
		D3DXVECTOR3 const &			LineEnd,		// 	LineEnd		: 線の終点
		float &						fDistance,		//	fDistance	: 距離
		COLLISIONDIRECTION const &	Direction		//	Direction	: 方向
	);
	// 矩形と球の当たり判定処理
	//	RectPos				: 矩形の位置
	// 	RectOffset			: 矩形のオフセット位置
	//	RectSize			: 矩形のサイズ
	// 	pSpherePos			: 球の位置
	//	SphereOffset		: 球のオフセット位置
	// 	SphereRange			: 球の半径
	static bool RectAndSphere(
		D3DXVECTOR3 const & RectPos,		//	RectPos				: 矩形の位置
		D3DXVECTOR3 const & RectOffset,		// 	RectOffset			: 矩形のオフセット位置
		D3DXVECTOR3 const & RectSize,		//	RectSize			: 矩形のサイズ
		D3DXVECTOR3 *		pSpherePos,		// 	pSpherePos			: 球の位置
		D3DXVECTOR3 const & SphereOffset,	//	SphereOffset		: 球のオフセット位置
		float		const & SphereRange		// 	SphereRange			: 球の半径
	);

	static D3DXVECTOR3 GetClosestpoint(
		D3DXVECTOR3 const & Pos,
		D3DXVECTOR3 const & OppPos,
		D3DXVECTOR3	const & Size
	);

	// 途中の計算
	static bool TlyCollision(
		D3DXVECTOR3 ObjectPos,
		D3DXVECTOR3,
		...
	);
	// 外積の当たり判定
	static bool CrossCollision(
		D3DXVECTOR3 *ObjectPos,
		D3DXVECTOR3 *PosA,
		D3DXVECTOR3 *PosB,
		D3DXVECTOR3 *PosC
	);
	// 外積の当たり判定
	//	ObjectPos	: オブジェクトの位置
	//	PosA		: 位置A
	//	PosB		: 位置B
	//	PosC		: 位置C
	//	PosD		: 位置D
	static bool PolygonToPointIn(
		D3DXVECTOR3 const & ObjectPos,	// オブジェクトの位置
		D3DXVECTOR3 const & PosA,		// 位置A
		D3DXVECTOR3 const & PosB,		// 位置B
		D3DXVECTOR3 const & PosC,		// 位置C
		D3DXVECTOR3 const & PosD		// 位置D
	);
	// 行列の位置情報取得
	// 1:位置情報,2:行列
	static D3DXVECTOR3 Vector3ToMatrix(
		D3DXVECTOR3 &vector3Out,
		D3DXMATRIX const &mtx
	);
	// 行列の位置情報取得
	// 1:行列
	static D3DXVECTOR3 Vector3ToMatrix(
		D3DXMATRIX const &mtx
	);
	// 桁数を測る(10で割る方法:数が少ない時に利用)
	static unsigned GetDigit_Division(unsigned nNum);
	// 桁数を測る(常用対数方法:数が不特定で大きい時に利用)
	static unsigned GetDigit_CommonLogarithm(unsigned nNum);
	// 桁数を測る(文字列方法:コーディングが早い)
	static unsigned GetDigit_Char(unsigned nNum);
	// ビルボードの設定
	static void SetBillboard(
		D3DXMATRIX *Originmtx,	// 元になる行列情報
		D3DXMATRIX *Viewmtx		// ビュー行列情報
	);
	// 垂直成分(質量なし = 等しく1)
	// 引数1:Aのスピード
	// 引数2:Bのスピード
	static void CollisionAfter_VerticalComponent(
		float & fAfterSpeedA,	// 衝突後のスピードA
		float & fAfterSpeedB,	// 衝突後のスピードB
		float const & fSpeedA,	// スピードA
		float const & fSpeedB,	// スピードB
		float const & e = 1		// 反発係数
	);

	// 球同士の衝突後速度位置算出
	// 引数1:衝突中の球Aの中心位置
	// 引数2:衝突の瞬間の球Aの速度
	// 引数3:衝突中の球Bの中心位置
	// 引数4:衝突中の球Bの速度
	// 引数5:球Aの質量
	// 引数6:球Bの質量
	// 引数7:球Aの反発率
	// 引数8:球Bの反発率
	// 引数9:球Aの反射後の速度ベクトル
	// 引数10:球Bの反射後の速度ベクトル
	static bool SquarColiAfterVec(
		D3DXVECTOR3 const & ColliPos_A,	// 衝突中の球Aの中心位置
		D3DXVECTOR3 const & ColliVec_A,	// 衝突の瞬間の球Aの速度
		D3DXVECTOR3 const & ColliPos_B,	// 衝突中の球Bの中心位置
		D3DXVECTOR3 const & ColliVec_B,	// 衝突の瞬間の球Bの速度
		float const & fWeight_A,		// 球Aの質量
		float const & fWeight_B,		// 球Bの質量
		float const & fRes_A,			// 球Aの反発率
		float const & fRes_B,			// 球Bの反発率
		D3DXVECTOR3 & pOut_Vec_A,		// 球Aの反射後の速度ベクトル
		D3DXVECTOR3 & pOut_Vec_B		// 球Bの反射後の速度ベクトル
	);

	// メッセージ発生
	static void Messanger(
		const char * cMessa,	// メッセージ内容
		HWND hWnd = NULL
	);
	// キー文字の反映
	static void KeyString(
		int nKeyID,				// キーID
		char *KeyName = NULL	// 文字
	);
	// ファイルが存在しているか
	static bool Existenceofscript(FILE * pFile);
	// テクスチャー情報からテクスチャーサイズの取得(2D)
	static bool GetTexSize(
		LPDIRECT3DTEXTURE9 pTex,
		D3DXVECTOR2 *pSize
	);
	// テクスチャー情報からテクスチャーサイズの取得(3D)
	static bool GetTexSize(
		LPDIRECT3DTEXTURE9 pTex,
		D3DXVECTOR3 * pSize
	);
	// 文字を特定の区切りごとに取得する(1行ごと)
	static std::vector<std::string> split(
		std::string& input,		// 1行のストリーム
		char delimiter		// 区切り文字
	);
	// 文字を特定の区切りごとに取得する(ファイル全体)
	static std::vector<std::vector<std::string>> FileContens(
		char const * cns_cFile,	// ファイル名
		char delimiter			// 区切り文字
	);

	// 2D 外積計算
	static float Vec2Cross(D3DXVECTOR2 const &rVecA, D3DXVECTOR2 const &rVecB);
	// ワールドマトリックス
	static void CalcMatrix(D3DXMATRIX *pMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rRot);
	// ビルボード設定　XとZのみ
	static void SetBillboard_XZ_Only(D3DXMATRIX *pMtx);
	// -3.14から3.14までのランダムで返す
	static float Random_PI();
	// 入力された値の+-ランダムな値で返す
	static float Random(float fInputValue);
	// ランダムなvector3型で値を返す
	static D3DXVECTOR3 RandomVector3(float Max);
	// 範囲の中からランダムに値を求める
	static uint64_t GetRandomRange(uint64_t min_value, uint64_t max_value);
	// ランダムにカラーを求める
	static D3DXCOLOR GetRandomColor(D3DXCOLOR &col);
	// 回転を360度以内にする計算
	static void CalcRotation(float &fRot);
	// 回転を360度以内にする計算
	static void CalcRotation_XYZ(D3DXVECTOR3 &rot);

	/* XInputのパッド用関数 */
	// 前回のスティック情報
	static void SaveLastStickInfo();
	// スティックを倒している方向をチェック
	static DIRECTION CheckPadStick();
	// いずれかのボタンを押したとき
	static bool PressAnyButton(void);
	// パッドの入力方向の判定
	static bool PadMoveInput(D3DXVECTOR3 & rMove, DIRECTION & direction, bool bJump, PLAYER_TAG PlayerTag);

	// パッドのスティックの情報
	typedef struct
	{
		float		fLeftStickValue_X;			//Lスティック( 横 )
		float		fLeftStickValue_Y;			//Lスティック( 縦 )
		bool		bLeftStickDown_X;			//Lスティック( 横 )が入力されているか
		bool		bLeftStickDown_Y;			//Lスティック( 縦 )が入力されているか
	}PAD_STICK;

	static PAD_STICK	m_PadStick[(int)PLAYER_TAG::PLAYER_MAX];		// コントローラーのスティック情報
	static DIRECTION	m_direction;					//方向

	/* ImGui用関数 */
	// ImGuiによるデバッグ情報
	static void ImG_DebugInfo(void);

	// ImGuiによる並列処理
	static void ImG_Parallel(void);

	// ImGuiのコンボボックス
	static bool ImGui_Combobox(std::vector<std::string> aItemNameList, std::string aTitle, int &nValue);

protected:

private:
	/* 変数 */
	static bool m_bWire;
};

// ----------------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ----------------------------------------------------------------------------------------------------

#endif