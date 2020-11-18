// ----------------------------------------------------------------------------------------------------
//
// Shape�w�b�_�[�����̐���[shape.h]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------

#ifndef _SHAPE_H_
#define _SHAPE_H_	 // �t�@�C������������߂�

// ----------------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"

// ----------------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// Shape���
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �\���̒�`
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �O���錾
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���̌`�N���X
class CShape
{
public:
	/* �񋓌^ */
	// �`�̎��
	typedef enum
	{
		SHAPETYPE_RECT = 0,
		SHAPETYPE_SPHERE,
		SHAPETYPE_COLUMN,
		SHAPETYPE_MAX
	} SHAPETYPE;
	/* �֐� */
	CShape();
	virtual ~CShape() {};
	// �X�V����
	virtual void Update(void) = 0;
	// �`�̎��
	virtual SHAPETYPE GetType(void) const = 0;
	// �s����󂯓n��
	// 1:�s����
	virtual void PassMatrix(D3DXMATRIX const &mtx);
	// �ʒu�Ɖ�]���󂯓n��
	// 1:�ʒu���,2:�J�X���
	virtual void PassPos(D3DXVECTOR3 const & rot);
	// �ʒu���󂯓n��
	// 1:�ʒu���
	// �I�t�Z�b�g�ʒu�ݒ�
	void SetOffset(D3DXVECTOR3 const &offset) { m_offset = offset; };
	// �I�t�Z�b�g�ʒu�擾
	D3DXVECTOR3 const &GetOffset(void) { return m_offset; };
	// �|�C���^�[�ʒu���̐ݒ�
	inline void Set_PPos(D3DXVECTOR3 * pPos) { m_pCorePos = m_pPos = pPos; };
	// �|�C���^�[�ʒu���̎擾
	D3DXVECTOR3 * Get_PPos(void) { return m_pPos; };
	// �|�C���^�[�ʒu���̐ݒ�
	void Set_PPosold(D3DXVECTOR3 * pPosold) { m_pCorePosOld = m_pPosold = pPosold; };
	// �|�C���^�[�ʒu���̎擾
	D3DXVECTOR3 * Get_PPosold(void) { return m_pPosold; };
	// �ʒu���̐ݒ�
	void Set_Pos(D3DXVECTOR3 pos) { m_pos = pos; };
	// �ʒu���̎擾
	D3DXVECTOR3 & Get_Pos(void) { return m_pos; };
	// �ߋ��̈ʒu���̐ݒ�
	void Set_Posold(D3DXVECTOR3 posold) { m_posold = posold; };
	// �ߋ��̈ʒu���̎擾
	D3DXVECTOR3 & Get_Posold(void) { return m_posold; };
	// ��ΓI�ߋ��̈ʒu���̐ݒ�
	D3DXVECTOR3 * Get_PosCore(void) { return m_pCorePos; };
	// ��ΓI�ʒu���̎擾
	D3DXVECTOR3 * Get_PosCoreold(void) { return m_pCorePosOld; };
	// �ŏI�I�Ȉʒu���ݒ�
	void Set_DestPos(void)
	{
		m_DestPos = *m_pCorePos + m_offset;
		m_DestPosOld = *m_pCorePosOld + m_offset;
	};

	// �ϐ� //
	D3DXVECTOR3 * m_pmove;			// �ړ���
	D3DXVECTOR3 m_DestPos;			// �I�t�Z�b�g�𑫂����A�ŏI�I�Ȉʒu���
	D3DXVECTOR3 m_DestPosOld;		// �I�t�Z�b�g�𑫂����A�ߋ��̍ŏI�I�Ȉʒu���
	D3DXVECTOR3 * m_pCorePos;		// �ʒu���̐�ΓI�ϐ�
	D3DXVECTOR3 * m_pCorePosOld;	// �ߋ��̈ʒu���̐�ΓI�ϐ�
	bool		m_bPush;			// �����o�����������邩�Ȃ�
	bool		m_bOpponentPush;	// ����������o�����ǂ���
protected:

private:
	D3DXVECTOR3 * m_pPos;			// �|�C���^�[�ʒu���
	D3DXVECTOR3 m_pos;				// �ʒu���
	D3DXVECTOR3 * m_pPosold;		// �ߋ��̃|�C���^�[�ʒu���
	D3DXVECTOR3 m_posold;			// �ߋ��̈ʒu���
	D3DXVECTOR3 m_offset;			// �I�t�Z�b�g���W
};

// ���̃N���X
class CSphereShape : public CShape
{
public:
	/* �񋓌^ */

	/* �֐� */
	CSphereShape() {};
	virtual ~CSphereShape() {};
	// �X�V����
	virtual void Update(void) {};
	// �쐬
	static std::unique_ptr<CSphereShape> Create(
		D3DXVECTOR3 const &offset,
		float const &radius,
		bool const &bPush = false,
		bool const &bOpponentPush = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// �s����󂯓n��
	// 1:�s����
	void PassMatrix(D3DXMATRIX const &mtx);
	// �ʒu�Ɖ�]���󂯓n��
	// 1:�ʒu���,2:�J�X���
	void PassPos(
		D3DXVECTOR3 const & rot
	);
	// �ݒ� //
	// ���a�ݒ�
	void SetRadius(float const radius) { m_radius = radius; };
	// �擾 //
	// �`�̎�ގ擾
	SHAPETYPE GetType(void) const { return SHAPETYPE_SPHERE; };
	// ���a�擾
	float GetRadius(void) const { return m_radius; };
protected:

private:
	float m_radius;
};

// ��`�̃N���X
class CRectShape : public CShape
{
public:
	/* �񋓌^ */

	/* �^�錾 */

	/* �֐� */
	CRectShape()
	{
		m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Min.x = m_Min.y = m_Min.z = FLT_MAX;
		m_Max.x = m_Max.y = m_Max.z = -FLT_MAX;
		m_MinOld.x = m_MinOld.y = m_MinOld.z = FLT_MAX;
		m_MaxOld.x = m_MaxOld.y = m_MaxOld.z = -FLT_MAX;
	};
	virtual ~CRectShape() {};
	// �X�V����
	virtual void Update(void) {};
	// �ő���W�l�ƍŏ����W�l�̌v�Z
	// 1:�s����
	void PassMatrix(D3DXMATRIX const &mtx);
	// �ő���W�l�ƍŏ����W�l�̌v�Z
	// 1:�ʒu���
	void PassPos(
		D3DXVECTOR3 const & rot
	);
	// ������W����ł��߂��A�{�b�N�X��̍��W��Ԃ�
	D3DXVECTOR3 GetClosestpoint(D3DXVECTOR3 const &pos);
	// �쐬
	static std::unique_ptr<CRectShape> Create(
		D3DXVECTOR3 const &offset,
		D3DXVECTOR3 const &size,
		bool const &bPush = false,
		bool const &bOpponentPush = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// �ݒ� //
	// ���a�ݒ�
	void SetSize(D3DXVECTOR3 const size) { m_size = size; };
	// �擾 //
	// �`�̎�ގ擾
	SHAPETYPE GetType(void) const { return SHAPETYPE_RECT; };
	// ���a�擾
	D3DXVECTOR3 GetSize(void) const { return m_size; };
	// �ő���W�l�擾
	D3DXVECTOR3 GetMax(void) const & { return m_Max; };
	// �ŏ����W�l�擾
	D3DXVECTOR3 GetMin(void) const & { return m_Min; };
	// �ߋ��̍ő���W�l�擾
	D3DXVECTOR3 GetMaxOld(void) const & { return m_MaxOld; };
	// �ߋ��̍ŏ����W�l�擾
	D3DXVECTOR3 GetMinOld(void) const & { return m_MinOld; };
protected:

private:
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_Min;
	D3DXVECTOR3 m_Max;
	D3DXVECTOR3 m_MinOld;
	D3DXVECTOR3 m_MaxOld;
};

// �~���̃N���X
class CColumnShape : public CShape
{
public:
	/* �񋓌^ */
	/* �֐� */
	CColumnShape() {};
	virtual ~CColumnShape() {};
	// �X�V����
	virtual void Update(void) {};
	// �쐬
	static std::unique_ptr<CColumnShape> Create(
		D3DXVECTOR3 const &offset,
		float const & fRadius,
		float const & fVertical,
		bool const &bPush = false,
		bool const &bOpponentPush = false,
		D3DXVECTOR3 * pPos = NULL,
		D3DXVECTOR3 * pPosold = NULL
	);
	// �s����󂯓n��
	// 1:�s����
	// ������
	void PassMatrix(D3DXMATRIX const &mtx) {};
	// �ʒu�Ɖ�]���󂯓n��
	// 1:�ʒu���,2:�J�X���
	// ������
	void PassPos(
		D3DXVECTOR3 const & rot
	) {};
	// �ݒ� //
	// ���a�ݒ�
	void SetRadius(float const & fRadius) { m_fRadius = fRadius; };
	// �c���ݒ�
	void SetVertical(float const & fVertical) { m_fVertical = fVertical; };
	// �擾 //
	// �`�̎�ގ擾
	SHAPETYPE GetType(void) const { return SHAPETYPE_COLUMN; };
	// ���a�擾
	float GetRadius(void) const { return m_fRadius; };
	// �c���擾
	float GetVertical(void) const { return m_fVertical; };
protected:

private:
	float m_fRadius;
	float m_fVertical;
};

#endif