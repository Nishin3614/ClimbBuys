// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �����t�B�[���h�u���b�N���� [connect_fieldblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CONNECT_FIELDBLOCK_H_
#define _CONNECT_FIELDBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "game.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseblock;	// �x�[�X�u���b�N�N���X

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CConnect_fieldblock : public CScene
{
public:
	/* �񋓌^ */

	/* �\���� */
	// �ǂݍ��ݏ��
	typedef struct
	{
		int		nFailId;		// ������ԍ�
		bool	bUse;			// �g�p��
	} FIELDINFO;
	// �ǂݍ��ݏ��
	// �ǂݍ��ݏ��
	typedef struct
	{
		int									nFeed;				// �t�B�[�h
		std::vector<std::vector<FIELDINFO>>	Dvec_pFileLoad;		// �t�@�C���̓ǂݍ��ݏ��
	} LOAD;

	/* �֐� */
	// �R���X�g���N�^
	CConnect_fieldblock();
	// �f�X�g���N�^
	virtual ~CConnect_fieldblock();
	// ����������
	virtual void Init(void);
	// �X�V����
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);

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
	) {};
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void) { return NULL; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };
	// �����t�B�[���h�u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �����t�B�[���h�u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);

	/* �����Ă���u���b�N�̐��� */
	// �쐬(�V�[���Ǘ�)
	//	stage		: �X�e�[�W���
	//	layer		: ���C���[
	static CConnect_fieldblock * Create(
		CGame::STAGE	const & stage,									// �X�e�[�W���
		CScene::LAYER	const & layer = CScene::LAYER_3DOBJECT			// ���C���[
	);
	// �쐬(�l�Ǘ�)
	//	stage		: �X�e�[�W���
	static CConnect_fieldblock * Create_Self(
		CGame::STAGE			const & stage									// �X�e�[�W���
		);
	// unique_ptr�쐬(�l�Ǘ�unique)
	// ���߂�l��std::move�Ŏ󂯎��
	//	stage		: �X�e�[�W���
	static std::unique_ptr<CConnect_fieldblock> Creat_Unique(
		CGame::STAGE			const & stage									// �X�e�[�W���
	);
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void  Debug(void);
#endif // _DEBUG
protected:
	/* �֐� */
	// �ݒ� //
private:
	/* �֐� */
	// �u���b�N�������鏈��
	void BlockFall(void);
	/* �ϐ� */
	static std::vector<LOAD>					m_vpLoad;			// �t�@�C���̓ǂݍ��ݏ��
	std::vector<CBaseblock *>					m_Dvec_pFieldBlock;	// �t�B�[���h�u���b�N���
	int											m_nMaxField;		// �t�B�[���h�̍ő吔
};

#endif