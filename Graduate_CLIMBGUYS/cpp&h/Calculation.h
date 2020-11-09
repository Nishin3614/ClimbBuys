// ----------------------------------------------------------------------------------------------------
//
// Calculation�w�b�_�[�����̐���[calculation.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"
#include "shape.h"

// ----------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------
// D3DVECTOR2�^�̏�����(1.0f)
#define D3DVECTOR2_INI			D3DXVECTOR2(1.0f,1.0f)
// D3DVECTOR2�^�̏�����(0.0f)
#define D3DVECTOR2_ZERO			D3DXVECTOR2(0.0f,0.0f)
// D3DVECTOR3�^�̏�����(0.0f)
#define D3DVECTOR3_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)
// D3DVECTOR3�^�̏�����(1.0f)
#define D3DVECTOR3_ONE			D3DXVECTOR3(1.0f,1.0f,1.0f)
// D3DVECTOR4�^�̏�����(0.0f)
#define D3DVECTOR4_ZERO			D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)
// D3DXCOLOR�^�̏�����
#define D3DXCOLOR_INI			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
// D3DXCOLOR�^�̏�����(��{�F)
#define D3DXCOLOR_C(col)		D3DXCOLOR(col,col,col,1.0f)
// D3DXCOLOR�^�̏�����(��{�F,�����x)
#define D3DXCOLOR_CA(col,alpha)	D3DXCOLOR(col,col,col,alpha)
// �t�@�C���ǂݍ��ݍő�s��
#define FILELINE_ERROW (1048576)
// ��ʂ̃T�C�Y
#define SCREEN_SIZE				(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT))
// ��ʒ����̍��W
#define SCREEN_CENTER_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f ,0.0f))

/* �e�L�X�g�p */
// ���s
#define NEWLINE				("\n")
// �e�L�X�g�p�@�R�����g�@���s
#define COMMENT01			("//\n")
// �e�L�X�g�p�@��
#define COMMENT02			("//------------------------------------------------------------\n")
// �e�L�X�g�p�@�C�R�[��
#define EQUAL				("=")

// ----------------------------------------------------------------------------------------------------
//
// Calculation���
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �񋓌^��`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// ���O��`
//
// ----------------------------------------------------------------------------------------------------
namespace MYLIBLARY
{
	void in(void);
	// ���������s�̕����|�C���^�[
	typedef const char * P_CST_CHAR;

}

// ����
enum class DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

// �����蔻��̌���
enum class COLLISIONDIRECTION
{
	NONE,	// �Ȃ�
	FRONT,	// �O
	BACK,	// ��
	LEFT,	// ��
	RIGHT,	// �E
	UP,		// ��
	DOWN	// ��
};

// �^�O
enum class PLAYER_TAG
{
	NONE = -1,
	PLAYER_1,			// �v���C���[1
	PLAYER_2,			// �v���C���[2
	PLAYER_3,			// �v���C���[3
	PLAYER_4,			// �v���C���[4
	PLAYER_MAX,			// �ő吔
};
// �^�O���Ǘ�����N���X
class CPlayerTAG
{
public:
	void				SetPlayerTag(PLAYER_TAG Tag)	{ m_PlayerTag = Tag; };		// �R���g���[���[�^�O�̐ݒ�
	PLAYER_TAG			GetPlayerTag()					{ return m_PlayerTag; };	// �R���g���[���[�^�O�̎擾

private:
	PLAYER_TAG			m_PlayerTag;	// �R���g���[���[�^�O
};

// �����^2��
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

// �����^3��
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
	// �L���X�g
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
	// ������Z�q
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
	// 2�����Z�q
	INTEGER3 operator + (CONST INTEGER3& Source) const
	{
		// �l��n���ϐ�
		INTEGER3 variable;
		variable.nX = nX + Source.nX;
		variable.nY = nY + Source.nY;
		variable.nZ = nZ + Source.nZ;
		return variable;
	}
	INTEGER3 operator - (CONST INTEGER3& Source) const
	{
		// �l��n���ϐ�
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

	int nX,		// X������
		nY,		// Y������
		nZ;		// Z������
} INTEGER3, *PINTEGER3;

// �����^4��
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

// �z�u�I�u�W�F�N�g���(1:�^�C�v,2:�ʒu,3:��])
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

// �z�u�|���S�����(1:�^�C�v,2:�ʒu,3:��],4:�T�C�YA,5:�T�C�YB)
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

// �z�u���b�V�����(1:�^�C�v,2:�ʒu,3:��]4:�c�u���b�N,5:���u���b�N,6:�T�C�YA,7:�T�C�YB)
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

// �t�@�C���i�[�p�ϐ�
typedef struct CHARFILE
{
	char		cFile
		[64];		// ���f���̃t�@�C����
} CHARFILE;

// �A�j���[�V����
typedef struct ANIMATION
{
	ANIMATION()
	{
		nCntAnim = 0;			// �A�j���J�E���g
		nMaxCntAnim = 0;		// �ő�A�j���J�E���g
		nHorizonAnim = 0;		// �����̃A�j���[�V�����p�^�[��
		nVirticalAnim = 0;		// �����̃A�j���[�V�����p�^�[��
		nMaxHorizonAnim = 1;	// �ő吅���A�j���[�V�����p�^�[����
		nMaxVirticalAnim = 1;	// �ő吂���̃A�j���[�V�����p�^�[����
		fHorizonSize = 1.0f;	// �����̃A�j���[�V����1�̃T�C�Y
		fVirticalSize = 1.0f;	// �����̃A�j���[�V����1�̃T�C�Y
		bLoop = true;			// ���[�v���
	}
	int					nCntAnim;			// �A�j���J�E���g
	int					nMaxCntAnim;		// �ő�A�j���J�E���g
	int					nHorizonAnim;		// �����̃A�j���[�V�����p�^�[��
	int					nVirticalAnim;		// �����̃A�j���[�V�����p�^�[��
	int					nMaxHorizonAnim;	// �ő吅���A�j���[�V�����p�^�[����
	int					nMaxVirticalAnim;	// �ő吂���̃A�j���[�V�����p�^�[����
	float				fHorizonSize;		// �����̃A�j���[�V����1�̃T�C�Y
	float				fVirticalSize;		// �����̃A�j���[�V����1�̃T�C�Y
	bool				bLoop;				// ���[�v���
} ANIMATION, *PANIMATION;

// ----------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------
#define INTEGER4_INI			INTEGER4(1,1,1,1)

// ----------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------
class CCalculation
{
public:
	/* �֐� */
	// unique_ptr[int]�^�̏���Ԃ�
	// ����:�Ԃ�����std::move���g�p���邱��
	static std::unique_ptr<int> int_ptr(
		int const nNum = 0
	);
	// unique_ptr[bool]�^�̏���Ԃ�
	// ����:�Ԃ�����std::move���g�p���邱��
	static std::unique_ptr<bool> bool_ptr(void);
	// �����̌v�Z
	static D3DXVECTOR3 Difference_Between(
		D3DXVECTOR3 &posA,
		D3DXVECTOR3 &posB
	);
	// 2�_�̋������Βl���ŋ��߂�
	static float Range_Absolute(
		const float & posA,
		const float & posB
	);

	// �Ԃ̍��̌v�Z(3D�p:�ʒu�A�ʒu)
	static float DiffPoint(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// �Ԃ̍��̌v�Z(2D�p:�ʒu�A�ʒu)
	static float DiffPoint(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);
	// �Ԃ̍���2��v�Z(3D�p:�ʒu�A�ʒu)
	static float DiffPointSquare(
		D3DXVECTOR3 const & posA,
		D3DXVECTOR3 const & posB
	);
	// �Ԃ̍���2��v�Z(2D�p:�ʒu�A�ʒu)
	static float DiffPointSquare(
		D3DXVECTOR2 const & posA,
		D3DXVECTOR2 const & posB
	);

	// �O�ς̌v�Z
	static D3DXVECTOR3 Cross_product(
		D3DXVECTOR3 &VecA,
		D3DXVECTOR3 &VecB
	);
	// ���ς̌v�Z
	static float Dot_product(
		D3DXVECTOR3 const & VecA,	// VectorA
		D3DXVECTOR3 const & VecB	// VectorB
	);
	// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
	static bool SquareBottom_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// �l�p�`�̒��ɓ����Ă��邩���Ȃ���
	static bool SquareSide_Judg(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 size,
		D3DXVECTOR3 rot,
		int			nBlock_W,
		int			nBlock_H,
		D3DXVECTOR3 worldpos
	);

	// ���̑����Z
	static float Addition_Sphere(
		float Length_x,
		float Length_y,
		float Length_z
	);
	// �~�̓����蔻��
	static bool Collision_Circle(
		D3DXVECTOR2 &Me,
		float		fMeSize,
		D3DXVECTOR2 &Opponent,
		float		fOppSize
	);
	// ���̓����蔻��
	static bool Collision_Sphere(
		D3DXVECTOR3 const &Me,
		float const &fMeSize,
		D3DXVECTOR3 const &Opponent,
		float const &fOppSize
	);
	// ��]�ʂ̏�� //
	static float Rot_One_Limit(
		float &fRot
	);
	// �X�N���[�����W�����[���h���W�ɕϊ�
	static D3DXVECTOR3* CalcScreenToWorld(
		D3DXVECTOR3* pout,		// �}�E�X�|�C���g
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj	// �v���W�F�N�V�����}�g���b�N�X
	);
	// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
	static D3DXVECTOR3* CalcScreenToXZ(
		D3DXVECTOR3* pout,		// �}�E�X�|�C���g
		float Sx,				// �X�N���[��X���W
		float Sy,				// �X�N���[��Y���W
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj,		// �v���W�F�N�V�����}�g���b�N�X
		D3DXVECTOR3 obj			// �I�u�W�F�N�g�̈ʒu
	);
	// ���[���h���W���X�N���[�����W�ɕϊ�(����)
	static D3DXVECTOR3* CalcWorldToScreen(
		D3DXVECTOR3* Obj,		// �I�u�W�F�N�g�̈ʒu
		float Sx,					// �X�N���[��X���W
		float Sy,					// �X�N���[��Y���W
		float fZ,				// �ˉe��Ԃł�Z�l�i0�`1�j
		float Screen_w,			// �X�N���[���̉���
		float Screen_h,			// �X�N���[���̍���
		D3DXMATRIX* mtxView,	// �r���[�}�g���b�N�X
		D3DXMATRIX* mtxPrj	// �v���W�F�N�V�����}�g���b�N�X
	);
	// �ꎟ�֐�
	static D3DXVECTOR2 Linear_function(
		D3DXVECTOR3 Apos,			// �n�_
		D3DXVECTOR3 Bpos			// �I�_
	);
	// 2�����̌�_(�A��������)
	static D3DXVECTOR3 TwoLine_Inse(
		D3DXVECTOR2 ALinear,	// ���@
		D3DXVECTOR2 BLinear	// ���A
	);
	// �|���S���Ɛ����̓����蔻�菈��
	//	PolygonVtxA	: �|���S�����_
	//	PolygonVtxB	: �|���S�����_
	//	PolygonVtxC	: �|���S�����_
	//	PolygonVtxD	: �|���S�����_
	// 	PolygonNor	: �|���S���@��
	// 	LineBegin	: ���̎n�_
	// 	LineEnd		: ���̏I�_
	//	fDistance	: ����
	//	Direction	: ����
	static bool PolygonToLineCollision(
		D3DXVECTOR3 const &			PolygonVtxA,	//	PolygonVtx	: �|���S�����_
		D3DXVECTOR3 const &			PolygonVtxB,	//	PolygonVtx	: �|���S�����_
		D3DXVECTOR3 const &			PolygonVtxC,	//	PolygonVtx	: �|���S�����_
		D3DXVECTOR3 const &			PolygonVtxD,	//	PolygonVtx	: �|���S�����_
		D3DXVECTOR3 const &			PolygonNor,		// 	PolygonNor	: �|���S���@��
		D3DXVECTOR3 const &			LineBegin,		// 	LineBegin	: ���̎n�_
		D3DXVECTOR3 const &			LineEnd,		// 	LineEnd		: ���̏I�_
		float &						fDistance,		//	fDistance	: ����
		COLLISIONDIRECTION const &	Direction		//	Direction	: ����
	);
	// ��`�Ƌ��̓����蔻�菈��
	//	RectPos				: ��`�̈ʒu
	// 	RectOffset			: ��`�̃I�t�Z�b�g�ʒu
	//	RectSize			: ��`�̃T�C�Y
	// 	pSpherePos			: ���̈ʒu
	//	SphereOffset		: ���̃I�t�Z�b�g�ʒu
	// 	SphereRange			: ���̔��a
	static bool RectAndSphere(
		D3DXVECTOR3 const & RectPos,		//	RectPos				: ��`�̈ʒu
		D3DXVECTOR3 const & RectOffset,		// 	RectOffset			: ��`�̃I�t�Z�b�g�ʒu
		D3DXVECTOR3 const & RectSize,		//	RectSize			: ��`�̃T�C�Y
		D3DXVECTOR3 *		pSpherePos,		// 	pSpherePos			: ���̈ʒu
		D3DXVECTOR3 const & SphereOffset,	//	SphereOffset		: ���̃I�t�Z�b�g�ʒu
		float		const & SphereRange		// 	SphereRange			: ���̔��a
	);

	static D3DXVECTOR3 GetClosestpoint(
		D3DXVECTOR3 const & Pos,
		D3DXVECTOR3 const & OppPos,
		D3DXVECTOR3	const & Size
	);

	// �r���̌v�Z
	static bool TlyCollision(
		D3DXVECTOR3 ObjectPos,
		D3DXVECTOR3,
		...
	);
	// �O�ς̓����蔻��
	static bool CrossCollision(
		D3DXVECTOR3 *ObjectPos,
		D3DXVECTOR3 *PosA,
		D3DXVECTOR3 *PosB,
		D3DXVECTOR3 *PosC
	);
	// �O�ς̓����蔻��
	//	ObjectPos	: �I�u�W�F�N�g�̈ʒu
	//	PosA		: �ʒuA
	//	PosB		: �ʒuB
	//	PosC		: �ʒuC
	//	PosD		: �ʒuD
	static bool PolygonToPointIn(
		D3DXVECTOR3 const & ObjectPos,	// �I�u�W�F�N�g�̈ʒu
		D3DXVECTOR3 const & PosA,		// �ʒuA
		D3DXVECTOR3 const & PosB,		// �ʒuB
		D3DXVECTOR3 const & PosC,		// �ʒuC
		D3DXVECTOR3 const & PosD		// �ʒuD
	);
	// �s��̈ʒu���擾
	// 1:�ʒu���,2:�s��
	static D3DXVECTOR3 Vector3ToMatrix(
		D3DXVECTOR3 &vector3Out,
		D3DXMATRIX const &mtx
	);
	// �s��̈ʒu���擾
	// 1:�s��
	static D3DXVECTOR3 Vector3ToMatrix(
		D3DXMATRIX const &mtx
	);
	// �����𑪂�(10�Ŋ�����@:�������Ȃ����ɗ��p)
	static unsigned GetDigit_Division(unsigned nNum);
	// �����𑪂�(��p�ΐ����@:�����s����ő傫�����ɗ��p)
	static unsigned GetDigit_CommonLogarithm(unsigned nNum);
	// �����𑪂�(��������@:�R�[�f�B���O������)
	static unsigned GetDigit_Char(unsigned nNum);
	// �r���{�[�h�̐ݒ�
	static void SetBillboard(
		D3DXMATRIX *Originmtx,	// ���ɂȂ�s����
		D3DXMATRIX *Viewmtx		// �r���[�s����
	);
	// ��������(���ʂȂ� = ������1)
	// ����1:A�̃X�s�[�h
	// ����2:B�̃X�s�[�h
	static void CollisionAfter_VerticalComponent(
		float & fAfterSpeedA,	// �Փˌ�̃X�s�[�hA
		float & fAfterSpeedB,	// �Փˌ�̃X�s�[�hB
		float const & fSpeedA,	// �X�s�[�hA
		float const & fSpeedB,	// �X�s�[�hB
		float const & e = 1		// �����W��
	);

	// �����m�̏Փˌ㑬�x�ʒu�Z�o
	// ����1:�Փ˒��̋�A�̒��S�ʒu
	// ����2:�Փ˂̏u�Ԃ̋�A�̑��x
	// ����3:�Փ˒��̋�B�̒��S�ʒu
	// ����4:�Փ˒��̋�B�̑��x
	// ����5:��A�̎���
	// ����6:��B�̎���
	// ����7:��A�̔�����
	// ����8:��B�̔�����
	// ����9:��A�̔��ˌ�̑��x�x�N�g��
	// ����10:��B�̔��ˌ�̑��x�x�N�g��
	static bool SquarColiAfterVec(
		D3DXVECTOR3 const & ColliPos_A,	// �Փ˒��̋�A�̒��S�ʒu
		D3DXVECTOR3 const & ColliVec_A,	// �Փ˂̏u�Ԃ̋�A�̑��x
		D3DXVECTOR3 const & ColliPos_B,	// �Փ˒��̋�B�̒��S�ʒu
		D3DXVECTOR3 const & ColliVec_B,	// �Փ˂̏u�Ԃ̋�B�̑��x
		float const & fWeight_A,		// ��A�̎���
		float const & fWeight_B,		// ��B�̎���
		float const & fRes_A,			// ��A�̔�����
		float const & fRes_B,			// ��B�̔�����
		D3DXVECTOR3 & pOut_Vec_A,		// ��A�̔��ˌ�̑��x�x�N�g��
		D3DXVECTOR3 & pOut_Vec_B		// ��B�̔��ˌ�̑��x�x�N�g��
	);

	// ���b�Z�[�W����
	static void Messanger(
		const char * cMessa,	// ���b�Z�[�W���e
		HWND hWnd = NULL
	);
	// �L�[�����̔��f
	static void KeyString(
		int nKeyID,				// �L�[ID
		char *KeyName = NULL	// ����
	);
	// �t�@�C�������݂��Ă��邩
	static bool Existenceofscript(FILE * pFile);
	// �e�N�X�`���[��񂩂�e�N�X�`���[�T�C�Y�̎擾(2D)
	static bool GetTexSize(
		LPDIRECT3DTEXTURE9 pTex,
		D3DXVECTOR2 *pSize
	);
	// �e�N�X�`���[��񂩂�e�N�X�`���[�T�C�Y�̎擾(3D)
	static bool GetTexSize(
		LPDIRECT3DTEXTURE9 pTex,
		D3DXVECTOR3 * pSize
	);
	// ���������̋�؂育�ƂɎ擾����(1�s����)
	static std::vector<std::string> split(
		std::string& input,		// 1�s�̃X�g���[��
		char delimiter		// ��؂蕶��
	);
	// ���������̋�؂育�ƂɎ擾����(�t�@�C���S��)
	static std::vector<std::vector<std::string>> FileContens(
		char const * cns_cFile,	// �t�@�C����
		char delimiter			// ��؂蕶��
	);

	// 2D �O�όv�Z
	static float Vec2Cross(D3DXVECTOR2 const &rVecA, D3DXVECTOR2 const &rVecB);
	// ���[���h�}�g���b�N�X
	static void CalcMatrix(D3DXMATRIX *pMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rRot);
	// �r���{�[�h�ݒ�@X��Z�̂�
	static void SetBillboard_XZ_Only(D3DXMATRIX *pMtx);
	// -3.14����3.14�܂ł̃����_���ŕԂ�
	static float Random_PI();
	// ���͂��ꂽ�l��+-�����_���Ȓl�ŕԂ�
	static float Random(float fInputValue);
	// �����_����vector3�^�Œl��Ԃ�
	static D3DXVECTOR3 RandomVector3(float Max);
	// �͈͂̒����烉���_���ɒl�����߂�
	static uint64_t GetRandomRange(uint64_t min_value, uint64_t max_value);
	// �����_���ɃJ���[�����߂�
	static D3DXCOLOR GetRandomColor(D3DXCOLOR &col);
	// ��]��360�x�ȓ��ɂ���v�Z
	static void CalcRotation(float &fRot);
	// ��]��360�x�ȓ��ɂ���v�Z
	static void CalcRotation_XYZ(D3DXVECTOR3 &rot);

	/* XInput�̃p�b�h�p�֐� */
	// �O��̃X�e�B�b�N���
	static void SaveLastStickInfo();
	// �X�e�B�b�N��|���Ă���������`�F�b�N
	static DIRECTION CheckPadStick();
	// �����ꂩ�̃{�^�����������Ƃ�
	static bool PressAnyButton(void);
	// �p�b�h�̓��͕����̔���
	static bool PadMoveInput(D3DXVECTOR3 & rMove, DIRECTION & direction, bool bJump, PLAYER_TAG PlayerTag);

	// �p�b�h�̃X�e�B�b�N�̏��
	typedef struct
	{
		float		fLeftStickValue_X;			//L�X�e�B�b�N( �� )
		float		fLeftStickValue_Y;			//L�X�e�B�b�N( �c )
		bool		bLeftStickDown_X;			//L�X�e�B�b�N( �� )�����͂���Ă��邩
		bool		bLeftStickDown_Y;			//L�X�e�B�b�N( �c )�����͂���Ă��邩
	}PAD_STICK;

	static PAD_STICK	m_PadStick[(int)PLAYER_TAG::PLAYER_MAX];		// �R���g���[���[�̃X�e�B�b�N���
	static DIRECTION	m_direction;					//����

	/* ImGui�p�֐� */
	// ImGui�ɂ��f�o�b�O���
	static void ImG_DebugInfo(void);

	// ImGui�ɂ����񏈗�
	static void ImG_Parallel(void);

	// ImGui�̃R���{�{�b�N�X
	static bool ImGui_Combobox(std::vector<std::string> aItemNameList, std::string aTitle, int &nValue);

protected:

private:
	/* �ϐ� */
	static bool m_bWire;
};

// ----------------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ----------------------------------------------------------------------------------------------------

#endif