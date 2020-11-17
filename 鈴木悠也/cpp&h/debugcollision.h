// ----------------------------------------
//
// デバッグ用の当たり判定表示処理の説明[debugcollision.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _DEBUGCOLLISION_H_
#define _DEBUGCOLLISION_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_DEBUGCOLLISION (10)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CDebugcollision
{
public:
	/* 列挙型 */
	typedef enum
	{
		COLLISIONTYPE_RECT = 0,
		COLLISIONTYPE_SPHERE,
		COLLISIONTYPE_MAX
	} COLLISIONTYPE;

	/* 関数 */
	CDebugcollision();
	~CDebugcollision();

	void Init(void);
	static void Create_Rect(
		D3DXVECTOR3 * pos,
		D3DXVECTOR3 const &size
	);
	static void Create_Sphere(
		D3DXVECTOR3 * pos,
		float const fRadius
	);

protected:

private:
	//void BoxCollision(void);


	D3DXVECTOR3 * m_ppos;
	D3DXVECTOR3 * m_size;
	COLLISIONTYPE m_type;
};

#endif