// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �d�C�u���b�N���� [electricblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _ELECTRICBLOCK_H_
#define _ELECTRICBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "baseblock.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CElectricblock : public CBaseblock
{
public:
	/* �񋓌^ */

	/* �\���� */

	/* �֐� */
	// �R���X�g���N�^
	CElectricblock();
	// �f�X�g���N�^
	virtual ~CElectricblock();
	// ����������
	virtual void Init(void);
	// �X�V����
	virtual void Uninit(void);
	// �X�V����
	virtual void Update(void);
	// �`�揈��
	virtual void Draw(void);

	// ����������̏���
	//	nObjType	: ����I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: ����I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	);
	// �������瓖����Ȃ�������̏���
	//	nObjType	: ����I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: ����I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};

	// ����������̔���
	//	Obj		: �I�u�W�F�^�C�v
	//	pScene	: �V�[�����
	void HitCollision(
		COLLISIONDIRECTION const &Direct,	// �O�㍶�E�㉺
		CScene::OBJ const & Obj,			// �I�u�W�F�^�C�v
		CScene * pScene = NULL				// �V�[�����
	);
	// �d�C������Ԑݒ�
	void SetElectric(bool const & bElect) { m_bElect = bElect; };
	// �����o���ꂽ�Ƃ��̃v���C���[�ԍ��ݒ�
	void SetPlayerTag(PLAYER_TAG const & Tag) { m_PushPlayer = Tag; };
	// �d�C�u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �d�C�u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);
	// �쐬(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	//	pCol		: �F���
	//	layer		: ���C���[
	static CElectricblock * Create(
		int				const & nModelId,							// ���f���ԍ�
		GRID			const & Grid,								// �s�񍂂��ԍ�
		D3DXCOLOR		* pCol,										// �F���
		float			const & fGravity,							// �d��
		CScene::LAYER	const & layer = CScene::LAYER_3DBLOCK		// ���C���[
	);
	// �쐬(�l�Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	//	pCol		: �F���
	static CElectricblock * Create_Self(
		int				const & nModelId,								// ���f���ԍ�
		GRID			const & Grid,									// �s�񍂂��ԍ�
		D3DXCOLOR		* pCol											// �F���
		);
	// unique_ptr�쐬(�l�Ǘ�unique)
	// ���߂�l��std::move�Ŏ󂯎��
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	//	pCol		: �F���
	static std::unique_ptr<CElectricblock> Creat_Unique(
		int				const & nModelId,								// ���f���ԍ�
		GRID			const & Grid,									// �s�񍂂��ԍ�
		D3DXCOLOR		* pCol											// �F���
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

	/* �ϐ� */
	bool		m_bElect;		// �d�C�������
	bool		m_bPlayerPush;	// �v���C���[�ɓ���������
	PLAYER_TAG	m_PushPlayer;	// �����ꂽ�Ƃ��̃v���C���[�ԍ�

};

#endif