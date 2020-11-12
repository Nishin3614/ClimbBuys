// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���U���g��UI[resultUI.h]
// Author : Yoshiki Sato
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _RESULTUI_H_
#define _RESULTUI_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CScene_TWO;		// �V�[��2D
class CNumber;			// �i���o�[

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
class CResultUI
{
public:
	/* ���U���gUI */
	enum class RESULT_UI
	{
		UI_NONE = -1,
		FRAME_1P,			// 1P�̘g
		FRAME_2P,			// 2P�̘g
		FRAME_3P,			// 3P�̘g
		FRAME_4P,			// 4P�̘g
		RANK_01,			// 1��
		RANK_02,			// 2��
		RANK_03,			// 3��
		RANK_04,			// 4��
		UI_MAX
	};

	/* �֐� */
	CResultUI();
	~CResultUI();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �쐬(�V�[���p������)
	static CResultUI * Create(void);					// �쐬
	static HRESULT Load(void);							// �ǂݍ���
	static void UnLoad(void);							// �j��
	// �ݒ� //

	// �擾 //
protected:
private:
	/* �֐� */
	void			InitSettingFrame(void);					// �g�̏����ݒ�
	void			InitSettingScore(void);					// �e�X�R�A�̏����ݒ�
	void			UpdateSettingFrame(void);				// �g�̍X�V�ݒ�
	void			UpdateSettingScore(void);				// �e�X�R�A�̍X�V�ݒ�
	/* �ϐ� */
	CScene_TWO		*m_pScene2D[(int)RESULT_UI::UI_MAX];	// �V�[��2D
	CNumber			*m_pNumber;								// �i���o�[
	D3DXVECTOR3		m_move;									// �ړ���
	int				m_nMoveCnt;								// �ړ����J�n����J�E���g
	bool			m_bMoveStart;							
};
#endif