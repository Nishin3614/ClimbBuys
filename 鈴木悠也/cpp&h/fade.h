// ----------------------------------------
//
// ��ʑJ�ڏ����̐���[fade.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FADE_H_
#define _FADE_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "2Dpresents.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define MAX_FADE (10)

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CFade : C2DPresents
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// �����Ȃ����
		FADE_IN,			// �t�F�[�h�C������
		FADE_OUT,			// �t�F�[�h�A�E�g����
		FADE_MAX
	} FADE;

	/* �֐� */
	CFade();
	~CFade();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// ����������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_MyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă�ꂽ��̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	virtual void Scene_OpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	void SetFade(CManager::MODE const &Mode);			// ��ʑJ�ڐݒ�
	FADE GetFade(void);									// ��ʑJ�ڏ��擾
	static CFade * Create(CManager::MODE const &mode);	// �쐬
	static HRESULT Load(void);
	static void UnLoad(void);
protected:

private:
	FADE					m_fade;		// �t�F�[�h���
	CManager::MODE			m_modeNext;	// ���̉�ʁi���[�h�j
};

#endif