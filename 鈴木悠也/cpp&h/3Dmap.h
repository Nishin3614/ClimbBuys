// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�}�b�v�����̐���[3Dmap.h]
// Author : Koki Nishiyama
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DMAP_H_
#define _3DMAP_H_	 // �t�@�C������������߂�

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_X.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C3DMap
{
public:
	/* �񋓌^ */
	typedef enum
	{
		MAP_STAGE_1 = 0,
		MAP_STAGE_2,
		MAP_STAGE_TUTORIAL,
		MAP_MAX
	} MAP;
	/* �񋓌^ */
	// 3D�I�u�W�F�N�g
	typedef struct OBJECT
	{
		OBJECT()
		{
			nModelType = 0;
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
		}
		int nModelType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	} OBJECT;
	// �L�����N�^�[
	typedef struct CHARACTER
	{
		CHARACTER()
		{
			nCharacter = 0;
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
		}
		int nCharacter;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	} CHARACTER;
	// �|���S��
	typedef struct POLYGON
	{
		POLYGON()
		{
			nTexType = 0;
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
			size = D3DVECTOR2_ZERO;
			bBillboard = false;
			bZEneble = false;
			bLighting = false;
			bAlpha = false;
		}
		int nTexType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR2 size;
		bool bBillboard;
		bool bZEneble;
		bool bLighting;
		bool bAlpha;
	} POLYGON;
	// ��
	typedef struct FLOOR
	{
		FLOOR()
		{
			nTexType = 0;
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
			nBlockDepth = 0;
			nBlockWidth = 0;
			size = D3DVECTOR2_ZERO;
		}
		int nTexType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int nBlockDepth;
		int nBlockWidth;
		D3DXVECTOR2 size;
	} FLOOR;
	// ��
	typedef struct WALL
	{
		WALL()
		{
			nTexType = 0;
			pos = D3DVECTOR3_ZERO;
			rot = D3DVECTOR3_ZERO;
			nBlockDepth = 0;
			nBlockWidth = 0;
			size = D3DVECTOR2_ZERO;
		}
		int nTexType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int nBlockDepth;
		int nBlockWidth;
		D3DXVECTOR2 size;
	} WALL;
	// ��`���
	typedef struct RECT
	{
		RECT()
		{
			rot = D3DVECTOR3_ZERO;	// ��]
			size = D3DVECTOR3_ZERO;	// �T�C�Y
		}
		D3DXVECTOR3 rot;	// ��]
		D3DXVECTOR3 size;	// �T�C�Y
	} RECT;
	// �����
	typedef struct SPHERE
	{
		SPHERE()
		{
			fRadius = 0.0f;			// ���a
		}
		float fRadius;		// ���a
	} SPHERE;
	// �~�����
	typedef struct COLUMN
	{
		COLUMN()
		{
			rot = D3DVECTOR3_ZERO;	// ��]
			fRadius = 0.0f;			// ���a
			fVertical = 0.0f;		// �c
		}
		D3DXVECTOR3 rot;	// ��]
		float fRadius;		// ���a
		float fVertical;	// �c
	} COLUMN;
	// �����蔻����
	typedef struct COLLISION
	{
		COLLISION()
		{
			pos = D3DVECTOR3_ZERO;	// �ʒu
			bPush = false;			// �����o������
			nObjType = 0;			// �I�u�W�F�N�g�^�C�v
			uni_Rect = NULL;		// ��`���
			uni_Sphere = NULL;		// �����
			uni_Column = NULL;		// �~�����
		}
		D3DXVECTOR3 pos;					// �ʒu
		bool bPush;							// �����o������
		int nObjType;						// �I�u�W�F�N�g�^�C�v
		std::unique_ptr<RECT>	uni_Rect;	// ��`���
		std::unique_ptr<SPHERE>	uni_Sphere;	// �����
		std::unique_ptr<COLUMN>	uni_Column;	// �~�����
	} COLLISION;

	/* �֐� */
	C3DMap();
	~C3DMap();
#ifdef _DEBUG
	static void Debug(void);
#endif // _DEBUG
	static HRESULT LoadCreate(MAP const &map);					// �ǂݍ��񂾏��𐶐�
	static void LoadScript(char* Add);
	static HRESULT Load(void);						// ���\�[�X���ǂݍ��ސݒ�
	// �}�l�[�W���[�̓ǂݍ���
	static HRESULT ManagerLoad(void);
	// �X�N���v�g�̓ǂݍ���
	static HRESULT ScriptLoad(void);
	static void UnLoad(void);						// �ǂݍ��񂾃��\�[�X�����J������
protected:
private:
	/* �֐� */
	// �L�����N�^�[����
	void CharaCreate(void);
	/* �ϐ� */
	static std::vector<std::vector<OBJECT>> m_vec_obj;			// �I�u�W�F�N�g���
	static std::vector<std::vector<CHARACTER>> m_vec_char;		// �L�����N�^�[���
	static std::vector<std::vector<POLYGON>> m_vec_polygon;		// �|���S�����
	static std::vector<std::vector<FLOOR>> m_vec_floor;			// �����
	static std::vector<std::vector<WALL>> m_vec_wall;			// �Ǐ��
	static std::vector<std::vector<COLLISION>> m_vec_Collision;	// �~�������蔻����
	static std::vector<std::string> m_vec_String;				// �t�@�C�����ǂݏ����p
	int m_nType;												// �^�C�v
};

#endif