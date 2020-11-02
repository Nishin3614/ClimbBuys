// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f����񏈗� [model_info.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _MODEL_INFO_H_
#define _MODEL_INFO_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Calculation.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���[�V�����G�t�F�N�g
typedef struct MOTION_EFFECT
{
	MOTION_EFFECT()
	{
		nStart = 0;			// �J�n����
		nEnd = 0;			// �I������
		nKeyID = 0;			// �L�[ID
		nParticleType = 0;	// �p�[�e�B�N���^�C�v
		offset = D3DVECTOR3_ZERO;
	}
	int nStart;			// �J�n����
	int nEnd;			// �I������
	int nKeyID;			// �L�[ID
	int nParticleType;	// �p�[�e�B�N���^�C�v
	D3DXVECTOR3 offset;	// �ʒu
} MOTION_EFFECT;

// ���[�V�����J����
typedef struct MOTION_CAMERA
{
	MOTION_CAMERA()
	{
		offsetR = D3DVECTOR3_ZERO;	// �����_�̃I�t�Z�b�g
		fLength = 0.0f;				// ����
		fHeight = 0.0f;				// ����
		rot = D3DVECTOR3_ZERO;		// ��]
		fIntertia = 0.0f;			// ����
	}
	D3DXVECTOR3 offsetR;	// �����_�̃I�t�Z�b�g
	float		fLength;	// ����
	float		fHeight;	// ����
	D3DXVECTOR3 rot;		// ��]
	float fIntertia;		// ����
} MOTION_CAMERA;

// �O�Ղ̏ڍ׏��
typedef struct MESHOBIT_DETAILS
{
	// �R���X�g���N�^
	MESHOBIT_DETAILS() = default;
	int	nObitID;				// �O�Ղ�ID
	int nBeginFram;				// �n�_�t���[��
	int nEndFram;				// �I�_�t���[��
	D3DXCOLOR BeginCol;			// �n�_�F
	D3DXCOLOR EndCol;			// �I�_�F
} MESHOBIT_DETAILS;

// �U���p�����蔻����
typedef struct COLLISION_DETAILS
{
	// �R���X�g���N�^
	COLLISION_DETAILS()
	{
		nCollisionID = 0;			// �����蔻���ID
		fDouble = 1.0f;				// �{��
		nBeginFrame = 0;			// �n�_�t���[��
		nEndFrame = 0;				// �I�_�t���[��
		nDamage = 0;				// �n�_�F
	};
	int nCollisionID;			// �����蔻���ID
	float fDouble;				// �{��
	int nBeginFrame;			// �n�_�t���[��
	int nEndFrame;				// �I�_�t���[��
	int nDamage;				// �n�_�F
} COLLISION_DETAILS;

// �L�[
typedef struct KEY
{
	KEY()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
	}
	D3DXVECTOR3 pos;						// �ʒu
	D3DXVECTOR3 rot;						// ��]
} KEY;

// �L�[���
typedef struct KEY_INFO
{
	KEY_INFO()
	{
		nFrame = 0;
		nNumCollision = 0;
		nMaxCollisiontime = 1;
		nBlust = 1;
		Key = NULL;
		pMotionCamera = NULL;
	}
	int	nFrame;									// �t���[��
	int nNumCollision;							// �����蔻��̌�
	int nMaxCollisiontime;						// �����蔻��̕p�x
	int nBlust;									// ������ѕ�
	std::vector<MOTION_EFFECT> v_MotionEffect;		// ���[�V�����G�t�F�N�g
	MOTION_CAMERA * pMotionCamera;				// ���[�V�����J����
	KEY *Key;									// �L�[
} KEY_INFO;

// ���[�W�������
typedef struct MOTION
{
	MOTION()
	{
		nLoop = 0;								// ���[�v
		nNumKey = 0;							// �L�[���̐�
		nAllFrame = 0;							// ���[�V�����S�̂̃t���[����
		Collision_nParts = 0;					// �p�[�c�ԍ�
		Collision_Ofset = D3DVECTOR3_ZERO;		// �I�t�Z�b�g
		Collision_Radius = 0;					// �~�̔��a
		Collision_StartFram = 0;				// �J�n�t���[����
		Collision_EndFram = 0;					// �I���t���[����
		Collision_Damage = 0;					// �_���[�W��
		KeyInfo = NULL;							// �L�[���(���[�V������)
	}
	int			nLoop;							// ���[�v
	int			nNumKey;						// �L�[���̐�
	int			nAllFrame = 0;					// ���[�V�����S�̂̃t���[����
	// �����蔻��p //
	int			Collision_nParts;				// �p�[�c�ԍ�
	D3DXVECTOR3	Collision_Ofset;				// �I�t�Z�b�g
	float		Collision_Radius;				// �~�̔��a
	int			Collision_StartFram;			// �J�n�t���[����
	int			Collision_EndFram;				// �I���t���[����
	int			Collision_Damage;				// �_���[�W��

	std::vector<COLLISION_DETAILS> v_Collision;		// �����蔻��ڍ׏��
	std::vector<MESHOBIT_DETAILS> v_MeshObit_detail;	// �O�Ղ̏ڍ�
	KEY_INFO	*KeyInfo;						// �L�[���(���[�V������)
} MOTION;

// �I�t�Z�b�g�̏��
typedef struct MODEL_OFFSET
{
	MODEL_OFFSET()
	{
		pos = D3DVECTOR3_ZERO;
		rot = D3DVECTOR3_ZERO;
		nParent = 0;
	}
	D3DXVECTOR3	pos;						// �ʒu
	D3DXVECTOR3	rot;						// ��]
	int			nParent;					// �e�ԍ�
} MODEL_OFFSET;

// �O�Ղ̊�{���
typedef struct MESHOBIT_BASIC
{
	// �R���X�g���N�^
	MESHOBIT_BASIC() = default;
	int nLine;					// ���̐�
	int nPart;					// �p�[�c
	D3DXVECTOR3 BeginOffset;	// �n�_�I�t�Z�b�g
	D3DXVECTOR3 EndOffset;		// �I�_�I�t�Z�b�g
	int nTexType;				// �e�N�X�`���[�^�C�v
} MESHOBIT_BASIC;

// ��`���
typedef struct RECTINFO
{
	// �R���X�g���N�^
	RECTINFO() = default;
	D3DXVECTOR3 size;
} RECTINFO;
// �����
typedef struct SPHEREINFO
{
	// �R���X�g���N�^
	SPHEREINFO() = default;
	float fRadius;
} SPHEREINFO;
// �~�����
typedef struct COLUMNINFO
{
	// �R���X�g���N�^
	COLUMNINFO() = default;
	float fRadius;
	float fVertical;
} COLUMNINFO;

// �L�����N�^�[�p�����蔻��̊�{���
typedef struct CHARACTERCOLLISION
{
	// �R���X�g���N�^
	CHARACTERCOLLISION() = default;
	D3DXVECTOR3 Offset;				// �I�t�Z�b�g
	std::unique_ptr<RECTINFO> RectInfo;				// ��`���
	std::unique_ptr<SPHEREINFO> p_uni_SphereInfo;	// �����
} CHARACTERCOLLISION;

// �U���p�����蔻��̊�{���
typedef struct ATTACKCOLLISION
{
	// �R���X�g���N�^
	ATTACKCOLLISION() = default;
	int			nParts;							// �p�[�c
	D3DXVECTOR3 Offset;							// �I�t�Z�b�g
	std::unique_ptr<RECTINFO> p_uni_RectInfo;		// ��`���
	std::unique_ptr<SPHEREINFO> p_uni_SphereInfo;	// �����
	std::unique_ptr<COLUMNINFO> p_uni_ColumnInfo;	// �~�����
} ATTACKCOLLISION;

// ���f���E���[�V�����̂̏��
typedef struct MODEL_ALL
{
	// �R���X�g���N�^
	MODEL_ALL()
	{
		pModel_offset = NULL;	// ���f���̃I�t�Z�b�g
	}
	MODEL_OFFSET			*pModel_offset;			// ���f���̃I�t�Z�b�g
	std::vector<MOTION *>		pMotion;				// ���[�V����
	std::vector<MESHOBIT_BASIC>	v_MeshObitLoad;			// �O�Ղ̏��
} MODEL_ALL;

// ���f�����
typedef struct XMODEL
{
	// �R���X�g���N�^
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
	LPD3DXMESH			pMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		pBuffMat;			// �}�e���A�����ւ̃|�C���^
	LPDIRECT3DTEXTURE9	*ppTexture;			// �e�N�X�`���ւ̃_�u���|�C���^�[
	DWORD				nNumMat;			// �}�e���A���̐�
	D3DXVECTOR3			vtxMinMaterials;	// ���f�����̈ʒu�̍ŏ��l
	D3DXVECTOR3			vtxMaxMaterials;	// ���f�����̈ʒu�̍ő�l
	D3DXVECTOR3			pos;				// �ʒu���
	D3DXVECTOR3			posDest;			// �ړI�ʒu
	D3DXVECTOR3			posDiff;			// ���ʒu
	D3DXVECTOR3			rot;				// ��]���
	D3DXVECTOR3			rotDest;			// �ړI��]
	D3DXVECTOR3			rotDiff;			// ����]
	D3DXVECTOR3			size;				// �T�C�Y
	int					nParent;			// �e�ԍ�
} XMODEL;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CModel_info
{
public:
	/* �֐� */
	CModel_info();
	~CModel_info();
	// ���f���̃e�L�X�g�f�[�^�Ǎ�
	static void TextLoad(
		MODEL_ALL	*pmodelAll,					// ���f���ƃ��[�V������
		std::vector<int> &ModelFile,					// ���f���t�@�C���̏��
		int const &nMaxMotion,					// ���[�V������
		int &nMaxkey,							// �L�[��
		int &nMaxParts,							// ���������f����
		const char * file_name					// �t�@�C����
	);
	// ���f���̃e�L�X�g�f�[�^�Ǎ�
	static void TextLoad(
		MODEL_ALL	*pmodelAll,					// ���f���ƃ��[�V������
		std::vector<int> &ModelFile,					// ���f���t�@�C���̏��
		int &nMaxkey,							// �L�[��
		int &nMaxParts,							// ���������f����
		const char * file_name					// �t�@�C����
	);
	// �ǂݍ��񂾃��f���̃e�L�X�g�f�[�^�̔j��
	static void TextUnload(
		MODEL_ALL *pmodelAll					// ���f���ƃ��[�V������
		);
protected:
private:
};

#endif
