// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �J�������� [camera.h]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCamera
{
public:
	/* �񋓌^ */
	// �^�C�v
	typedef enum
	{
		TYPE_TITLE = 0,// �^�C�g��
		TYPE_GAME,	   // �Q�[��
		TYPE_RESULT,   // ���U���g
		TYPE_MAX	   // �ő吔
	} TYPE;
	/* �\���� */
	// �ǂݍ��ݗp
	typedef struct
	{
		D3DXVECTOR3 offset;		// �����_�̃I�t�Z�b�g
		float		fTrunSpeed;	// ��]�X�s�[�h
		float		fLengh;		// �����_�Ǝ��_�̒���
		float		fHeight;	// �����_�Ǝ��_�̍���
	} LOAD;
	// �U�����
	struct SHAKE
	{
		int			nInterbalCnt;			// �U�������
		int			nInterbalCntSave;		// �U������񐔂��J�E���g
		float		fDistance;				// ����
		float		fRange[2];				// �����͈̔�_�ŏ��ő�
		float		fShakeSpeed;			// �U�����鑬��
		D3DXVECTOR3 CameraCurrentPos;		// �J�����̌��ݒn_�U���O�̍��W
		bool		bEnd;					// �I���t���O

		//float fAngle;						// �p�x
	};

	/* �֐� */
	// �R���X�g���N�^
	CCamera();
	// �f�X�g���N�^
	~CCamera();
	// ����������
	void Init(void);
	// �I������
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
#ifdef _DEBUG
	// �f�o�b�O����
	void Debug(void);
#endif // _DEBUG
	// ��������
	static CCamera * Create(void);	// �쐬
	// �ǂݍ���
	static HRESULT Load(void);
	// �j��
	static void Unload(void);
	// �������ݏ���
	static void Save(void);
	// �e���[�h�̃J�����̏����ݒ�
	void ModeCameraInit(void);
	// �J�����̏�񏉊���
	void InfoInit(
		D3DXVECTOR3 const &pos,	// �ʒu���
		D3DXVECTOR3 const &rot	// ��]���
		);
	// �J�������̐ݒ�擾
	bool GetCamera_infoSet(void)  const { return m_bSet; };
	// �J�����ݒ�
	void Set(void);
	// �ړI�����_�̐ݒ�
	void SetPosDestR(
		D3DXVECTOR3 const & pos,	// �ʒu���
		D3DXVECTOR3 const & rot		// ��]���
	);
	// �����_�̐ݒ�
	void SetPosR(
		D3DXVECTOR3 const & pos,	// �ʒu���
		D3DXVECTOR3 const & rot		// ��]���
	);
	// �J�������ݒ�̎g�p��Ԑݒ�
	void SetCamera_InfoSet(bool const &bSet) { m_bSet = bSet; };
	// �J�����^�C�v�̐ݒ�
	void SetType(
		TYPE type
	);
	// ��]���擾
	D3DXVECTOR3 GetRot(void);
	// ��]�X�s�[�h�擾
	D3DXVECTOR2 &GetRotSpeed(void);
	// �J�����ݒ肷�ׂĂ̏�����
	void SetCameraInfo(void);
	// �J�������[�V�����ݒ�
	//	posR		: �ʒuR
	//	rot			: ��]
	//	fLength		: ����
	//	fHeight		: ����
	//	fIntertia	: ����
	void SetCamera_Motion(
		D3DXVECTOR3 const & posR,	// �ʒuR
		D3DXVECTOR3 const & rot,	// ��]
		float const & fLength,		// ����
		float const & fHeight,		// ����
		float const & fIntertia		// ����
	);
	// �J�����̎��_�擾
	D3DXVECTOR3 &GetPosV(void) { return m_posV; };
	//
	D3DXVECTOR3 &GetPosR(void) { return m_posR; };

	// �J�����̐U��
	void CameraShake();

	// �J�����̐U���̏��ݒ� mode�̃J�����̏������ł���
	void SetShakeInfo();

	// �J�����̐U�����I�������ۂ̃t���O�擾bEnd
	bool GetShakeEndFlag() { return m_Shake.bEnd; };

protected:

private:
	/* �֐� */
	void Update_Play(void);						// �v���C���̃J����
	// �I�[�v�j���O����������
	void Init_Opening(void);
	// �`���[�g���A������������
	void Init_Tutorial(void);
	// �^�C�g������������
	void Init_Title(void);
	// �Q�[������������
	void Init_Game(void);
	// ���U���g����������
	void Init_Result(void);
	// �`���[�g���A���X�V����
	void Update_Tutorial(void);
	// �^�C�g���X�V����
	void Update_Title(void);
	// �Q�[���X�V����
	void Update_Game(void);


	// �������̃J�����ݒ�
	void InitCamera(void);
	/* �ϐ� */
	static	LOAD	m_load;						// ���ۑ�
	D3DXVECTOR3		m_posV;						// ���_
	D3DXVECTOR3		m_posVDest;					// �ړI���_
	D3DXVECTOR3		m_posVDiff;					// �ړI���_���王�_
	D3DXVECTOR3		m_posR;						// �����_
	D3DXVECTOR3		m_posRDest;					// �ڕW�����_
	D3DXVECTOR3		m_posRDiff;					// �ړI�����_���璍���_
	D3DXVECTOR3		m_offset;					// �����_�̃I�t�Z�b�g
	D3DXVECTOR3		m_posU;						// ������x�N�g��
	D3DXVECTOR3		m_between;					// ��Ƃ̋���
	D3DXVECTOR3		m_move;						// �ړ���
	D3DXVECTOR3		m_rot;						// ���_�̉�]��
	D3DXVECTOR3		m_rotDest;					// ���_�̖ړI��]�n�_
	D3DXVECTOR3		m_rotDiff;					// �ړI��]�n�_���猻�݉�]�n�_
	D3DXVECTOR3		m_rotBetween;				// �ړI��]�n�_�ƌ��݉�]�n�_�̍���
	D3DXVECTOR3		m_rotOrigin;				// ���̉�]��
	D3DXMATRIX		m_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X(�e���r���_)
	D3DXMATRIX		m_mtxView;					// �r���[�}�g���b�N�X(�J�����}�����_)
	D3DXVECTOR2		m_RotSpeed;					// ��]�X�s�[�h
	TYPE			m_type;						// �^�C�v[
	float			m_fHeight;					// �����_����̎��_�̍���
	float			m_fLength;					// ���_�ƒ����_�̋���
	float			m_fIntertia;				// �����̔䗦
	bool			m_bSet;						// �J�����ݒ�
	SHAKE			m_Shake;					// �U�����
};

#endif
