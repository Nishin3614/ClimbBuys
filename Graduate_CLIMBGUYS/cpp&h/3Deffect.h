// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�G�t�F�N�g�̏���[3Deffect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "effect.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X��`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C3DEffect : public CEffect
{
public:
	/* �֐� */
	// �R���X�g���N�^
	C3DEffect();
	// �f�X�g���N�^
	~C3DEffect();
	// �S���\�[�X���̓ǂݍ��ݏ���
	static HRESULT Load(void);
	// �S���\�[�X���̊J������
	static void Unload(void);
	// ����������
	void Init(void);
	// �I������
	void Uninit(void);
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
	// ��������
	static C3DEffect *Create(void);
#ifdef _DEBUG
	// �f�o�b�O����
	void Debug(void);
#endif // _DEBUG
	// ���_�쐬
	//	pDevice	: �f�o�C�X���
	HRESULT MakeVertex(
		LPDIRECT3DDEVICE9 pDevice	// �f�o�C�X���
	);
	// �S�̂̐ݒ�
	//	EffectType	: �G�t�F�N�g�^�C�v
	//	nTexType	: �e�N�X�`���[�^�C�v
	//	pos			: �ʒu
	//	rot			: ��]
	//	move		: �ړ���
	//	col			: �F
	//	size		: �T�C�Y
	//	nLife		: ���C�t
	//	Blend		: �u�����h�^�C�v
	//	sizeValue	: �T�C�Y�ω�
	static void Set3DEffect(
		EFFECT_TYPE const &EffectType,										// �G�t�F�N�g�^�C�v
		int const &nTexType,												// �e�N�X�`���[�^�C�v
		D3DXVECTOR3 const &pos,												// �ʒu
		D3DXVECTOR3 const &rot,												// ��]
		D3DXVECTOR3 const &move,											// �ړ���
		D3DXCOLOR const &col,												// �F
		D3DXVECTOR2 const &size,											// �T�C�Y
		int const &nLife,													// ���C�t
		CRenderer::BLEND const &Blend = CRenderer::BLEND_ADD_TRANSLUCENT,	// �u�����h�^�C�v
		D3DXVECTOR2 const &sizeValue = D3DVECTOR2_ZERO,						// �T�C�Y�ω��l
		float const &fAlphaValue = 0.0f										// ���ω��l
	);
private:
	/* �֐� */
	// ���_�T�C�Y�ݒ�
	//	pVtx	: 2D���_���
	//	pEffect	: �G�t�F�N�g���
	//	nIndex	: �ԍ����
	static void SetVartexSize(
		VERTEX_3D *pVtx,			// 2D���_���
		EFFECT *pEffect = NULL,		// �G�t�F�N�g���
		const int *nIndex = NULL	// �ԍ����
	);
	// ���_�J���[�ݒ�
	//	pVtx	: 2D���_���
	//	pEffect	: �G�t�F�N�g���
	//	nIndex	: �ԍ����
	static void SetVetexColor(
		VERTEX_3D *pVtx,			// 2D���_���
		EFFECT *pEffect = NULL,		// �G�t�F�N�g���
		const int *nIndex = NULL	// �ԍ����
	);

	/* �ϐ� */
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// �o�b�t�@���
	static EFFECT					m_aEffect[EFFECT_MAX];		// �G�t�F�N�g���
};

#endif