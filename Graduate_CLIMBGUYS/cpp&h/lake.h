// ==========================================================
//
// ��[lake.h]
// Author : masayasu wakita
//
// ==========================================================
#ifndef _LAKE_H_
#define _LAKE_H_

#include "main.h"
#include "scene_three.h"

#define	TEXTURE_BULLET	"data/TEXTURE/coin.png"			// �ǂݍ��ރe�N�X�`���t�@�C����

// ==========================================================
//
// �N���X�̒�`
//
// ==========================================================
class CLake : public CScene
{
public:

	CLake();												// �R���X�g���N�^
	~CLake();												// �f�X�g���N�^

	void Init(void);										// ������
	void Uninit(void);										// �I��
	void Update(void);										// �X�V
	void Draw(void);										// �`��

	static CLake *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// ����
	static HRESULT Load(void);													// ���[�h
	static void Unload(void);													// �A�����[�h

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
	// �������瓖����Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_NoMyCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};
	// ����ɓ��Ă��Ȃ�������̏���
	//	nObjType	: �I�u�W�F�N�g�^�C�v
	//	pScene		: ����̃V�[�����
	void Scene_NoOpponentCollision(
		int const &nObjType = 0,	// �I�u�W�F�N�g�^�C�v
		CScene * pScene = NULL		// ����̃V�[�����
	) {};

	// �|�C���^�[�ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPos(void) { return &m_pos; };
	void Scene_SetPos(D3DXVECTOR3 &pos) { m_pos = pos; };
	// �|�C���^�[�ߋ��̈ʒu�����擾
	D3DXVECTOR3 * Scene_GetPPosold(void) { return NULL; };
	// �|�C���^�[�ړ��ʏ��̎擾
	D3DXVECTOR3 * Scene_GetPMove(void) { return NULL; };

protected:

private:
	void MakeVertex(void);

	static LPDIRECT3DTEXTURE9 m_pTex;			// �e�N�X�`��
	static LPDIRECT3DTEXTURE9 m_pBumpTex;
	LPD3DXEFFECT m_pEffect = NULL;
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	VERTEX_3D *m_pVtx;							// ���_���ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X

	D3DXVECTOR3	m_vtxMin;						// �ŏ��l
	D3DXVECTOR3	m_vtxMax;						// �ő�l
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_rot;							// ��]
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_size;							// �T�C�Y
	D3DXCOLOR m_col;							// �F
	float m_fAnim;								// �A�j���[�V�����ړ���
	float m_fHeight;
};
#endif