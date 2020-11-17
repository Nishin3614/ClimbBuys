// ----------------------------------------
//
// �f�o�b�O�p�̓����蔻��\�������̐���[debugcollision.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _DEBUGCOLLISION_H_
#define _DEBUGCOLLISION_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define MAX_DEBUGCOLLISION (10)

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CDebugcollision
{
public:
	/* �񋓌^ */
	typedef enum
	{
		COLLISIONTYPE_RECT = 0,
		COLLISIONTYPE_SPHERE,
		COLLISIONTYPE_MAX
	} COLLISIONTYPE;

	/* �֐� */
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