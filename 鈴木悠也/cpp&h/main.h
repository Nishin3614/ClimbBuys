// ------------------------------------------
//
// gametly�����̐���[main.h]
// Author : Koki Nishiyama
//
// ------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_	 // �t�@�C������������߂�

// ------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>	// �E�B���h�E�A�v���P�[�V����
#include "d3dx9.h"
/* C����̃��C�u���� */
#include <stdlib.h>		// �V�X�e���w�b�_�[
#include <stdbool.h>	// �^�U�w�b�_�[
#include <stdio.h>		// ��ʓI�ȃw�b�_�[
#include <time.h>		// �^�C���w�b�_�[
#include <math.h>		// ���w�֐��w�b�_�[
/* C++�̃��C�u���� */
#include <iostream>
#include <fstream>
#include <sstream>
#include <tchar.h>
#include <vector>		// ���I�z��w�b�_�[
#include <memory>		// �������w�b�_�[
#include <future>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"						// ���͋@��p
#include <Xinput.h>
#include "xaudio2.h"					// BGM

// ------------------------------------------
//
// ���C�u�����̃����N
//
// ------------------------------------------
#pragma comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g(���i)
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "ws2_32.lib")

// ------------------------------------------
//
// �}�N���֐���`
//
// ------------------------------------------
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)
// 3�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� /���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SCREEN_WIDTH (1280)			// ��
#define SCREEN_HEIGHT (720)			// �c
#define INTERVAL_TIME (60 * 10)		// �C���^�[�o������
#define ERROW_ACTION (0)			// �f�o�b�O���
#define SAFE_RELEASE(pObj) { if(pObj != NULL) { (pObj)->Release(); (pObj) = NULL; }}

// ------------------------------------------
//
// �\����
//
// ------------------------------------------
// �Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float       rhw;	// 1.0�ŌŒ�
	D3DCOLOR    col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`���[
} VERTEX_2D;

// 3�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;		// �e�N�X�`���[
} VERTEX_3D;

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------
#include "Calculation.h"

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------
class CRenderer;

// ------------------------------------------
//
// �v���g�^�C�v�錾
//
// ------------------------------------------
// �I����Ԑݒ�
void SetDestWind(bool const &bDest);
#if _DEBUG
int GetFPS(void);
void DispConsol();
#endif // _DEBUG

#endif // !_MAIN_H_