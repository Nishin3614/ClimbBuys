// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���o�u���b�N���� [stagingblock.h]
// Author : fujiwaramasato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _STAGINGBLOCK_H_
#define _STAGINGBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "scene_x.h"
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_STAGINGBLOCK (9)

// ��
#define SPACE_Y (500)
// �^�C�g�����S�`���p �u���b�N���W
// CLIMBGUYS
#define STAGINGBLOCK_POS_C (D3DXVECTOR3(-600.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_L (D3DXVECTOR3(-450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_I (D3DXVECTOR3(-300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_M (D3DXVECTOR3(-150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_B (D3DXVECTOR3(0.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_G (D3DXVECTOR3(150.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_U (D3DXVECTOR3(300.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_Y (D3DXVECTOR3(450.0f, 0.0f + SPACE_Y, 0));
#define STAGINGBLOCK_POS_S (D3DXVECTOR3(600.0f, 0.0f + SPACE_Y, 0));

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CStagingBlock : public CScene_X
{
public:

	enum class STAGING_BLOCKTYPE
	{
		NORMAL,			// �ʏ�
		LEVITATION,		// �󒆕��V
		ASCENT,			// �㏸
		DESCENT,		// ���~
		MAX
	};

	struct Block_Condition
	{
		int	nPerFrame[static_cast<int>(STAGING_BLOCKTYPE::MAX)];		// ���t���[�����s���邩
		int	nFrameBetween[static_cast<int>(STAGING_BLOCKTYPE::MAX)];	// ���t���[�����ɏo�������邩
		int	nToOnes[static_cast<int>(STAGING_BLOCKTYPE::MAX)];			// ���ɂ����o�������邩
	};

	/* �֐� */
	// �R���X�g���N�^
	CStagingBlock();
	// �f�X�g���N�^
	~CStagingBlock();
	// ����������
	void Init(void);
	// �X�V����
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);

	// �x�[�X�u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �x�[�X�u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);

	// �������̊m��
	static CStagingBlock *Create(
		D3DXVECTOR3 const &pos,						// �ʒu
		D3DXVECTOR3 const &rot,						// ��]
		D3DXVECTOR3 const &size,					// �T�C�Y�{��
		D3DXCOLOR color,							// �J���[
		int const &nModelId,						// ���f���ԍ�
		bool const &bShadowMap,						// �V���h�E�}�b�s���O���
		STAGING_BLOCKTYPE type
	);

	void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// �������瓖����Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);

	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void);
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void);
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void);

	// �u���b�N�̔ԍ��擾
	int GetBlockNum() {return m_nBlockNun;};
	// �u���b�N�̔ԍ��ݒ�
	void SetBlockNum(int num) { m_nBlockNun = num; };
	// �u���b�N�����ł����x�N�g���̐ݒ�
	void SetBurstVector(D3DXVECTOR3 vector) { m_Vector = vector; };
	// �u���b�N�̃o�[�X�g�x�N�g���̎擾
	D3DXVECTOR3 GetBurstVector() { return m_Vector; };
	// �u���b�N���~������ݒ�
	void SetMoveStop(bool stop) { m_bStopMove = stop; };
	// �u���b�N�̒�~��Ԃ��擾
	bool GetMoveStop() { return m_bStopMove; };

	// ���o�I���t���O�̐ݒ�
	static void SetEnd(bool end) { m_bEnd = end; };
	// ���o�I���t���O�̎擾
	static bool GetEnd() { return m_bEnd; };

	/////// ��Z�b�g ///////
	// ��������u���b�N�̏����̐ݒ�
	static void SetCondition(const int PerFrame, const int FrameBetween, const int ToOnes , STAGING_BLOCKTYPE type);
	// ��莞�Ԗ��ɉ��o�u���b�N�𐶐�
	static void Create_Block(D3DXVECTOR3 Originpos,D3DXVECTOR3 Range, int const &nModelId, STAGING_BLOCKTYPE type);
	// �����̏�����
	static void InitCondition();

	////////////////////////////

	// �u���b�N�̈ړ�����
	void BlockFall();
	// �u���b�N�̔j��
	void BlockBurst();

protected:
private:
	/* �֐� */
	void				Levitating();							// �󒆕��V �^�C�g���Ŏg��
	void				Descent();								// ���~		�^�C�g���Ŏg��



	/* �ÓI�ϐ� */
	static bool			m_bEnd;									// ���o�I���t���O

	/* �ϐ� */
	STAGING_BLOCKTYPE	m_BlockType = STAGING_BLOCKTYPE::NORMAL;// ���o�u���b�N�̎��
	static Block_Condition m_Condition;							// �����̏��

	float				m_fSpeed;								// �ړ���
	float				m_fBurstSpeed;							// �ړ���

	D3DXVECTOR3			m_Vector;								// �u���b�N�����ł��������x�N�g��
	int					m_nBlockNun;							// �u���b�N�̔ԍ�
	bool				m_bStopMove;							// ���W�̍X�V���~�߂�
	int					m_nAngle;								// �p�x
	static int			m_nFrame;
};

#endif