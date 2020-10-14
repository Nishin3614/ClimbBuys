// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f������ [player.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "character.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PLAYER_FRONTFORCE		(10)
#define PLAYER_G				(0.5f)								// �d��
#define PLAYER_RESISTANCE		(0.5f)								// ��R��
#define PLAYER_MOTIONFILE		"data/LOAD/PLAYER/Tricker.txt"		// ���[�V�����̃t�@�C����
#define PLAYER_FALL				(-20.0f)							// ������ʒu����
#define PLAYER_UI_MP_POS		(D3DXVECTOR3(78.5f, 690.0f, 0.0f))	// UI_MP�̈ʒu
#define FISH_APPONENTPOS		(-40.0f)							// ���o���ʒu
#define FISH_APPONENTTIME		(300)								// ���o���^�C��
#define MPUP_BREAKBALLOON		(100)								// ���D������������MPUP
#define MAX_DIFFERENCE			(1000)								// �ړ����E����
#define MAX_FISHAPPONENT		(3)									// ���̏o���ꏊ

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �񋓑̒�`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------�L�����N�^�[�^�C�v---------- //
typedef enum
{
	CHARATYPE_SPEED_UP = 0,
	CHARATYPE_SCORE_UP,
	CHARATYPE_INVISIBLE,
	CHARATYPE_REVIVAL,
	CHARATYPE_MAX
} CHARATYPE;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C2DGauge;		// 2D�Q�[�W�N���X
class CRank;		// �����N�N���X
class CFramework;	// �t���[�����[�N�N���X
class CMeshdome;	// ���b�V���h�[���N���X

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CPlayer : public CCharacter, public CPlayerTAG
{
public:
	// ---------���[�V�����^�C�v---------- //
	typedef enum
	{
		MOTIONTYPE_MOVE = CCharacter::MOTIONTYPE_MAX,
		MOTIONTYPE_JAMP,
		MOTIONTYPE_DANCE,
		MOTIONTYPE_MAX
	} MOTIONTYPE;
	/* �֐� */
	CPlayer(CCharacter::CHARACTER const &character);
	virtual ~CPlayer();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	// �v���C���[�����񂾂Ƃ��̊֐�
	virtual void Die(void);
	void OtherDie(void);
	// ����������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// �������瓖����Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	void Camera(void);					// �J��������
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);			// �ǂݍ���
	static void UnLoad(void);			// UnLoad����
	// MP�グ����
	//	nMpUp	: MP�グ�l
	void MpUp(int const & nMpUp);
	// �Q�[�W����������
	void GaugeStatusInit(void);
protected:
private:
	/* �\���� */
	/* �֐� */
	// ���L�����ړ�����
	void MyMove(void);
	// ���L�����s������
	void MyAction(void);
	// ��Ԃɂ���Ẵ��[�V�����ݒ菈��
	void StatusMotion(void);
	/* �ϐ� */
	C2DGauge	*m_p2DMPGauge;						// MP�Q�[�W
	int m_nMP;										// MP
	CXInputPad		*m_pPad;						// �p�b�h�̃|�C���^

};
#endif