// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �x�[�X�u���b�N���� [baseblock.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _BASEBLOCK_H_
#define _BASEBLOCK_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene_x.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �}�N����`
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BASEBLOCK_RANGE			(50.0f)				// �u���b�N�͈̔�
#define BASEBLOCK_XYZTOPOS(X)	(BASEBLOCK_RANGE * X)	// �s�񍂂�����̈ʒu
#define BASEBLOCK_MINUSTOPLUS	(3)						// �s����v���X��

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �O���錾
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �\����
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �N���X
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CBaseblock : public CScene_X
{
public:
	/* �񋓌^ */
	// �x�[�X�u���b�N
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_FIELD,			// �t�B�[���h�u���b�N
		TYPE_MAX,			// �^�C�v�S�̐�
	} TYPE;

	/* �\���� */
	// �Ֆʏ��
	typedef struct GRID
	{
		// �R���X�g���N�^
		GRID() {}
		// �R���X�g���N�^
		//	Column	: ��
		//	Height	: ����
		//	Line	: �s
		GRID(int const &Column, int const &Height, int const &Line)
		{
			nColumn = Column;
			nLine = Line;
			nHeight = Height;
		}
		// �s�񍂂��̑����Z�̃I�y���[�V����
		GRID operator + (CONST GRID& Source) const
		{
			// �l��n���ϐ�
			GRID variable;
			variable.nColumn = nColumn + Source.nColumn;
			variable.nLine = nLine + Source.nLine;
			variable.nHeight = nHeight + Source.nHeight;
			return variable;
		}
		// �s�񍂂��̊|���Z�̃I�y���[�V����
		GRID operator * (CONST int& Source) const
		{
			// �l��n���ϐ�
			GRID variable;
			variable.nColumn = nColumn + Source;
			variable.nLine = nLine + Source;
			variable.nHeight = nHeight + Source;
			return variable;
		}
		// �s�񍂂�����ʒu���߂��I�y���[�V����
		operator D3DXVECTOR3 ()
		{
			D3DXVECTOR3 pos;
			pos.x = nColumn * BASEBLOCK_RANGE;
			pos.z = nLine * BASEBLOCK_RANGE;
			pos.y = nHeight * BASEBLOCK_RANGE;
			return pos;
		}
		int nColumn;	// ��
		int nLine;		// �s
		int nHeight;	// ����
	} GRID;
	/* �֐� */
	// �R���X�g���N�^
	CBaseblock();
	// �f�X�g���N�^
	virtual ~CBaseblock();
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

	// �x�[�X�u���b�N
	void SetType(TYPE const type)		{ m_type = type; };
	// �x�[�X�u���b�N
	TYPE GetType(void) const			{ return m_type; };
	// �������Ԑݒ�
	void SetFall(bool const & bFall)	{ m_bFall = bFall; };
	// �������Ԏ擾
	bool & GetFall(void)				{ return m_bFall; };
	// �Ֆʏ��擾
	GRID & GetGrid(void)				{ return m_grid; };
	// �Ֆʏ��ݒ�
	void SetGrid(GRID const &grid)		{ m_grid = grid; };

	// �x�[�X�u���b�N�S�\�[�X�̓ǂݍ���
	static HRESULT Load(void);
	// �x�[�X�u���b�N�S�\�[�X�̊J��
	static void UnLoad(void);
	// �쐬(�V�[���Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	//	layer		: ���C���[
	static CBaseblock * Create(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId,								// ���f���ԍ�
		CScene::LAYER	const & layer = CScene::LAYER_UI				// ���C���[
	);
	// �쐬(�l�Ǘ�)
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	static CBaseblock * Create_Self(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId								// ���f���ԍ�
		);
	// unique_ptr�쐬(�l�Ǘ�unique)
	// ���߂�l��std::move�Ŏ󂯎��
	//	pos			: �ʒu
	//	nModelId	: ���f���ԍ�
	static std::unique_ptr<CBaseblock> Creat_Unique(
		D3DXVECTOR3		const & pos,									// �ʒu
		int				const & nModelId								// ���f���ԍ�
	);
	// �^�C�g���p �u���b�N����Ăɐ���
	static void CreateInBulkBlock();

	// �u���b�N�̈ʒu�v�Z
	//static D3DXVECTOR3 CaluBlockPos(GRID const & Grid)
	//{ return D3DXVECTOR3() }

	/* �����p */
	// ���ݐςݏd�Ȃ��Ă���u���b�N�̍������擾
	//	nColumn	: ��
	//	nLine	: �s
	static int GetHeight(
		int const & nColumn,
		int const & nLine
	) { return m_anHeight[nColumn][nLine]; };
	// ���ݐςݏd�Ȃ��Ă���u���b�N�̍�����ݒ�
	//	nColumn	: ��
	//	nLine	: �s
	//	nHeight	: ����
	static void SetHeight(
		int const & nColumn,
		int const & nLine,
		int const & nHeight
	) { m_anHeight[nColumn][nLine] = nHeight; };
	// ���ݐςݏd�Ȃ��Ă���u���b�N�̍�����ݒ�
	//	Grid	: ��
	static void SetHeight(
		GRID const & Grid
	) {
		m_anHeight[Grid.nColumn][Grid.nLine] = Grid.nHeight;
	};
#ifdef _DEBUG
	// �f�o�b�O����
	virtual void  Debug(void);
#endif // _DEBUG
protected:
	/* �֐� */
	//D3DXVECTOR3 & GetGridToGrid(GRID const & Grid) {}
	// �ݒ� //
private:
	/* �֐� */
	// ������X�V����
	void Update_Fall(void);
	/* �ϐ� */
	TYPE	m_type;		// �x�[�X�u���b�N
	GRID	m_grid;		// �Ֆʏ��
	bool	m_bFall;	// ��������

	// �����p
	static int m_anHeight[10][10];
};

#endif