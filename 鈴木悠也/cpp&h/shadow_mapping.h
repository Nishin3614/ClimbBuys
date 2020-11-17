// ------------------------------------------
//
// �V���h�E�}�b�s���O���� [shadow_mapping.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _SHADOWMAPPING_H_
#define _SHADOWMAPPING_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------
#include "manager.h"
#include "scene_X.h"

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CShadowmapping
{
public:
	/* �֐� */
	// �e�`��(���f��)
	static void Draw(
		LPDIRECT3DDEVICE9	&pDevice,	// �f�o�C�X���
		CScene_X::MODEL_LOAD *model,	// ���f�����
		D3DXMATRIX			&mtx		// �}�g���b�N�X���
	);
	// �e�`��(���b�V��)
	static void Draw(
		LPDIRECT3DDEVICE9	&pDevice,		// �f�o�C�X���
		D3DXMATRIX			&mtx,			// �}�g���b�N�X���
		LPDIRECT3DINDEXBUFFER9	&pIndex,	// �C���f�b�N�X���
		int	&nNumberVertex,					// �C���f�b�N�X���
		int	&nNumPolygon					// �C���f�b�N�X���
	);
protected:
private:
};
#endif