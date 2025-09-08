#pragma once
#include "Bg.h"
#include "Player.h"
#include "Shot.h"
#include "TrapManager.h"
#include "PlatformManager.h"
#include "MoveSpikeMgr.h"
#include "Shuriken.h"
#include "Goal.h"
#include "SceneTitle.h"
#include "ParticleMgr.h"

class Player;
class Shot;
class Bg;
class Rock;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	void UpdateShot(); //�e�X�V
	void DeleteShot(int index); //�e�폜

	//�V�[�P���X���Ƃ�Update������؂�ւ���
	void UpdateTitle();		//�^�C�g��
	void UpdateFadeIn();	//�t�F�[�h�C��
	void UpdateGame();		//�Q�[���v���C

	void UpdateClear();		//�N���A
	void UpdateGameOver();	//�Q�[���I�[�o�[

private:
	//�V�[�P���X
	enum Seq
	{
		SeqTitle,	//�^�C�g��
		SeqFadeIn,	//�t�F�[�h�C��
		SeqGame,	//�Q�[���v���C
		SeqClear,	//�N���A
		SeqGameOver,//�Q�[���I�[�o�[

		SeqNum	//�V�[�P���X�̐�
	};

public:
	//�X�^�[�g�����������ǂ����̃t���O
	bool m_isStartPressed;

private:
	//�t���[��
	int m_frame;	//�t���[�����v��
	float m_clearFrame; //�N���A���Ă���̃t���[��

	//�g�p����O���t�B�b�N
	int m_playerIdleGraphHandle;	//�v���C���[�̃A�C�h���O���t�B�b�N
	int m_playerWalkGraphHandle;	//�v���C���[�̕����O���t�B�b�N
	int m_playerShotGraphHnadle;	//�v���C���[�̒e�����O���t�B�b�N
	int m_playerJumpGraphHandle;	//�v���C���[�̃W�����v�O���t�B�b�N
	int m_playerDJumpGraphHandle;	//�v���C���[�̓�i�W�����v�O���t�B�b�N
	int m_tileGraphHandle;			//�}�b�v�`�b�v�̃O���t�B�b�N
	int m_bgGraphHandle;			//�w�i�̃O���t�B�b�N
	int m_bulletGraphHandle;		//�e�̃O���t�B�b�N
	int m_RtrapGraphHandle;			//�E�����g���b�v�̃O���t�B�b�N
	int m_UtrapGraphHandle;			//������g���b�v�̃O���t�B�b�N
	int m_LtrapGraphHandle;			//�������g���b�v�̃O���t�B�b�N
	int m_BtrapGraphHandle;			//�������g���b�v�̃O���t�B�b�N
	int m_platformGraphHandle;		//�v���b�g�t�H�[���̃O���t�B�b�N
	int m_moveSpikeGraphHandle;		//�ړ��g�Q�̃O���t�B�b�N
	int m_shurikenGraphHandle;		//�藠���̃O���t�B�b�N
	int m_goalGraphHandle;			//�S�[���̃O���t�B�b�N
	int m_rockGraphHandle;			//��̃O���t�B�b�N

	//�t�H���g�n���h��
	int m_clearFontHandle;		//�N���A�\���p�t�H���g�n���h��
	int m_backTitleFontHandle;	//�^�C�g���w�i�p�t�H���g�n���h��
	int m_gameoverFontHandle; //�Q�[���I�[�o�[�p�t�H���g�n���h��

	//�T�E���h�n���h��
	int m_mainBgmHandle;		//���C��BGM�n���h��
	int m_clearSeHandle;		//�N���ASE�n���h��
	int m_fireSpikeSeHandle;	//�g�Q���ˎ�SE
	int m_deadSeHandle;			//���S��SE

	//�T�E���h�̉���
	int m_mainBgmVolume;			//���C��BGM�̉���
	int m_clearSeVolume;			//�N���ASE����
	int m_fireSpikeSeVolume;		//�g�Q���ˎ�SE����
	int m_deadSeVolume;				//���S��SE����

	//�t���O�֌W

	//�g�Q���ˍς݃t���O
	bool m_isRtrapFired;			// ��x������������t���O(�E�����g�Q)
	//�v���b�g�t�H�[���t���O
	bool m_isPlatformSpawned;		//��x������������t���O
	//�g�Q�ݒu�ς݃t���O
	bool m_isUtrapSpawned;			//��x������������t���O(�������g�Q)
	bool m_isLtrapSpawned;			//��x������������t���O(�������g�Q)
	bool m_isBtrapSpawned;			//��x������������t���O(�������g�Q)
	//�ړ��g�Q�t���O
	bool m_isMoveSpikeSpawned;		//��x������������t���O
	//�v���C���[���S�t���O
	bool m_isDead;					//����ł��邩����ł��Ȃ���
	//���S�������s�������ǂ���
	bool m_isDeadActive;
	//�┭���ς݃t���O
	bool m_isRockFired;				//��x������������t���O

	//���݂̃V�[�P���X
	Seq m_gameSeq;
	int m_frameCount;

	//�t�F�[�h�̃t���[���� 0:�^����
	int m_fadeFrame;

	//�v���C���[
	Player* m_pPlayer;
	//�w�i
	Bg* m_pBg;
	//�e
	Shot* m_pShot[5];
	//�藠��
	Shuriken* m_pShuriken;
	//�g���b�v�}�l�[�W���[
	TrapManager m_trapManager;
	//�v���b�g�t�H�[���}�l�[�W���[
	PlatformManager m_platformManager;
	//�ړ��g�Q�}�l�[�W���[
	MoveSpikeMgr m_moveSpikeMgr;
	//�S�[��
	Goal m_goal;
	//�^�C�g���V�[��
	SceneTitle m_sceneTitle;
	//�p�[�e�B�N���}�l�[�W���[
	ParticleMgr m_particleMgr;
	//��
	Rock* m_pRock;
};