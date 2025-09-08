#include "SceneMain.h"
#include "DxLib.h"
#include "Bg.h"
#include "player.h"
#include "Trap.h"
#include "Shot.h"
#include "Game.h"
#include "TrapManager.h"
#include "Shuriken.h"
#include "Rock.h"
#include <dinput.h>

namespace
{
	constexpr int kShotMax = 5;	//1�x�Ɍ��Ă�ő�e��
	constexpr int kFadeFrame = 30; //�t�F�[�h�ɂ�����t���[����
	constexpr int kMainBgmVolume = 50; //BGM�̉���
	constexpr int kClearSeVolume = 60;	//�N���A�̉���
	constexpr int kFireSpikeSeVolume = 100;	//�g�Q���ˎ�����
	constexpr int kDeadSeVolume = 100;	//���S������
}

SceneMain::SceneMain() :
	m_frame(0),
	m_clearFrame(0),
	m_playerIdleGraphHandle(-1),
	m_playerWalkGraphHandle(-1),
	m_playerShotGraphHnadle(-1),
	m_playerJumpGraphHandle(-1),
	m_playerDJumpGraphHandle(-1),
	m_tileGraphHandle(-1),
	m_bgGraphHandle(-1),
	m_bulletGraphHandle(-1),
	m_RtrapGraphHandle(-1),
	m_UtrapGraphHandle(-1),
	m_LtrapGraphHandle(-1),
	m_BtrapGraphHandle(-1),
	m_platformGraphHandle(-1),
	m_moveSpikeGraphHandle(-1),
	m_shurikenGraphHandle(-1),
	m_goalGraphHandle(-1),
	m_clearFontHandle(-1),
	m_backTitleFontHandle(-1),
	m_gameoverFontHandle(-1),
	m_mainBgmHandle(-1),
	m_clearSeHandle(-1),
	m_fireSpikeSeHandle(-1),
	m_deadSeHandle(-1),
	m_mainBgmVolume(0),
	m_clearSeVolume(0),
	m_fireSpikeSeVolume(0),
	m_deadSeVolume(0),
	m_isStartPressed(false),
	m_isRtrapFired(false),
	m_isPlatformSpawned(false),
	m_isUtrapSpawned(false),
	m_isLtrapSpawned(false),
	m_isBtrapSpawned(false),
	m_isMoveSpikeSpawned(false),
	m_isDead(false),
	m_isDeadActive(false),
	m_isRockFired(false),
	m_gameSeq(SeqTitle),
	m_frameCount(0),
	m_fadeFrame(0)
{
	m_pPlayer = new Player;
	for (int i = 0; i < kShotMax; i++)
	{
		m_pShot[i] = nullptr;
	}

	m_pShuriken = new Shuriken;

	m_pBg = new Bg;

	//m_pRock = new Rock;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	//�t���[�����̏�����
	m_frame = 0;

	//�����V�[�P���X�̌���
	m_gameSeq = SeqTitle;

	m_frameCount = 0;

	//�t�F�[�h��Ԃ̏�����
	m_fadeFrame = 0;

	//�O���t�B�b�N�����[�h����
	m_playerIdleGraphHandle = LoadGraph("data/idle.png");
	m_playerWalkGraphHandle = LoadGraph("data/Run.png");
	m_playerShotGraphHnadle = LoadGraph("data/Shoot.png");
	m_playerJumpGraphHandle = LoadGraph("data/Jump.png");
	m_playerDJumpGraphHandle = LoadGraph("data/Jump.png");
	m_tileGraphHandle = LoadGraph("data/tileset.png");
	m_bgGraphHandle = LoadGraph("data/BackGround.png");
	m_RtrapGraphHandle = LoadGraph("data/SpikeTrap.png");
	m_UtrapGraphHandle = LoadGraph("data/SpikeTrap.png");
	m_LtrapGraphHandle = LoadGraph("data/SpikeTrap.png");
	m_BtrapGraphHandle = LoadGraph("data/SpikeTrap.png");
	m_platformGraphHandle = LoadGraph("data/fakeTileset.png");
	m_moveSpikeGraphHandle = LoadGraph("data/MoveSpike.png");
	m_shurikenGraphHandle = LoadGraph("data/Shuriken.png");
	m_goalGraphHandle = LoadGraph("data/Goal.png");
	m_rockGraphHandle = LoadGraph("data/Rock.png");

	//�t�H���g�̃��[�h
	m_clearFontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 120, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_backTitleFontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 20, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_gameoverFontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 140, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//�T�E���h�̃��[�h
	m_mainBgmHandle = LoadSoundMem("data/MainBgm.mp3");
	m_clearSeHandle = LoadSoundMem("data/ClearSe.mp3");
	m_fireSpikeSeHandle = LoadSoundMem("data/SpikeFireSe.mp3");
	m_deadSeHandle = LoadSoundMem("data/DeadSe.mp3");

	//�v���C���[�̏�����
	m_pPlayer->Init(
		m_playerIdleGraphHandle, m_playerIdleGraphHandle,
		m_playerWalkGraphHandle, m_playerShotGraphHnadle,
		m_playerJumpGraphHandle, m_playerDJumpGraphHandle);

	//�^�C�g���V�[���̏�����
	m_sceneTitle.Init();

	//�w�i�̏�����
	m_pBg->Init();

	//�g���b�v�̏�����
	m_trapManager.Init(
		m_RtrapGraphHandle, m_UtrapGraphHandle,
		m_LtrapGraphHandle, m_BtrapGraphHandle);

	//������n�ʂ̏�����
	m_platformManager.Init(m_platformGraphHandle);

	//�v���C���[�Ƀv���b�g�t�H�[���}�l�[�W���[���Z�b�g
	m_pPlayer->SetPlatformManager(&m_platformManager);

	//�v���C���[�̃|�C���^���擾
	m_goal.SetPlayer(m_pPlayer);
	m_trapManager.SetPlayer(m_pPlayer);

	//���̃V�[���̃|�C���^��n��
	m_sceneTitle.SetSceneMain(this);

	//�ړ��g�Q�̏�����
	m_moveSpikeMgr.Init(m_moveSpikeGraphHandle);

	//�藠���̏�����
	m_pShuriken->Init(m_shurikenGraphHandle);

	//�S�[���̏�����
	m_goal.Init(m_goalGraphHandle);

	//��̏�����
	//m_pRock->Init(m_rockGraphHandle);

	//�藠���̃|�C���^���v���C���[�ɃZ�b�g
	m_pShuriken->SetPlayer(m_pPlayer);

	//��Ƀv���C���[�̃|�C���^���Z�b�g
	//m_pRock->SetPlayer(m_pPlayer);

	//�t���O�̏�����
	m_isStartPressed = (false);
	m_isRtrapFired = (false);
	m_isPlatformSpawned = (false);
	m_isUtrapSpawned = (false);
	m_isLtrapSpawned = (false);
	m_isBtrapSpawned = (false);
	m_isMoveSpikeSpawned = (false);
	m_isDead = (false);
	m_isDeadActive = (false);
	m_isRockFired = false;

	//�g�Q���˃C�x���g(X:1000,Y:300���z������)
	if (m_pPlayer->GetPos().x > 1100.0f &&
		m_pPlayer->GetPos().y > 200.0f &&
		m_pPlayer->GetPos().y <= 300.0f &&
		!m_isRtrapFired)
	{
		m_trapManager.SpawnTrap(
			{ 0.0f, 300.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //�E�ɔ�ԃg�Q

		m_trapManager.SpawnTrap(
			{ 0.0f, 280.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //�E�ɔ�ԃg�Q

		m_isRtrapFired = true; // �g�Q�𔭎ˍς݃t���O�𗧂Ă�
	}

	//������g�Q��ݒu
	if (!m_isUtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 848.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 670.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 900.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 176.0f, 624.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_isUtrapSpawned = true; // �g�Q��ݒu�ς݃t���O�𗧂Ă�
	}

	//�������g�Q��ݒu
	if (!m_isLtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 368.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 400.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 432.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 464.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 492.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_isLtrapSpawned = true; // �g�Q��ݒu�ς݃t���O�𗧂Ă�
	}

	//�������g�Q��ݒu
	if (!m_isBtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ -15.0f,  632.0f }, { 240.0f,  528.0f },
			{ 0.0f,   9.0f }); // �������g�Q

		m_isBtrapSpawned = true; // �g�Q��ݒu�ς݃t���O�𗧂Ă�
	}

	//�v���b�g�t�H�[������
	if (!m_isPlatformSpawned)
	{
		//�v���b�g�t�H�[���𐶐�
		m_platformManager.SpawnPlatform({ 736.0f, 168.0f }, 0.1f); // 0.1�t���[����ɗ����J�n
		m_isPlatformSpawned = true; // �v���b�g�t�H�[���𐶐��ς݃t���O�𗧂Ă�
	}

	//�ړ��g�Q����
	if (!m_isMoveSpikeSpawned)
	{
		//�ړ��g�Q�𐶐�
		m_moveSpikeMgr.SpawnSpike({ 400.0f, 363.0f }, { 0.0f, 0.0f }); // �����ʒu�Ƒ��x

		m_moveSpikeMgr.SpawnSpike({ 630.0f, 518.0f }, { 0.0f, 1.0f }); // �����ʒu�Ƒ��x

		m_moveSpikeMgr.SpawnSpike({ 700.0f, 630.0f }, { 0.0f, 1.0f }); // �����ʒu�Ƒ��x

		m_isMoveSpikeSpawned = true; // �ړ��g�Q�𐶐��ς݃t���O�𗧂Ă�
	}

	int x = 20;
	int y = 3;
	DrawStringToHandle(
		x, y, "OPSION�Ń^�C�g���ɂ��ǂ�",
		GetColor(255, 255, 255),
		m_backTitleFontHandle);
}

void SceneMain::End()
{
	//BGM�̒�~
	StopSoundMem(m_mainBgmHandle);

	m_pPlayer->End();
	m_pBg->End();
	m_moveSpikeMgr.End();
	//m_pRock->End();

	//�O���t�B�b�N���J��
	DeleteGraph(m_playerIdleGraphHandle);
	DeleteGraph(m_playerWalkGraphHandle);
	DeleteGraph(m_playerShotGraphHnadle);
	DeleteGraph(m_playerJumpGraphHandle);
	DeleteGraph(m_playerDJumpGraphHandle);
	DeleteGraph(m_tileGraphHandle);
	DeleteGraph(m_bgGraphHandle);
	DeleteGraph(m_bulletGraphHandle);
	DeleteGraph(m_RtrapGraphHandle);
	DeleteGraph(m_UtrapGraphHandle);
	DeleteGraph(m_LtrapGraphHandle);
	DeleteGraph(m_BtrapGraphHandle);
	DeleteGraph(m_platformGraphHandle);
	DeleteGraph(m_moveSpikeGraphHandle);
	DeleteGraph(m_shurikenGraphHandle);
	DeleteGraph(m_goalGraphHandle);
	DeleteGraph(m_rockGraphHandle);

	//�t�H���g�̍폜
	DeleteFontToHandle(m_clearFontHandle);
	DeleteFontToHandle(m_backTitleFontHandle);
	DeleteFontToHandle(m_gameoverFontHandle);

	//�T�E���h�̍폜
	DeleteSoundMem(m_mainBgmHandle);
}

void SceneMain::Update()
{
	/*printfDx("���݂̃V�[��:%d\n", m_gameSeq);*/
	//���݂̃V�[�P���X�̌o�߃t���[�������J�E���g
	m_frameCount++;
	switch (m_gameSeq)
	{
	case SeqTitle:
		UpdateTitle();
		break;
	case SeqFadeIn:
		UpdateFadeIn();
		break;
	case SeqGame:
		UpdateGame();
		break;
	case SeqClear:
		UpdateClear();
		//BGM�̒�~
		StopSoundMem(m_mainBgmHandle);
		break;
	case SeqGameOver:
		UpdateGameOver();
		break;
	}
}

void SceneMain::Draw()
{
	switch (m_gameSeq)
	{
	case SeqTitle:
		m_sceneTitle.Draw();
		break;
	case SeqFadeIn:
	case SeqGame:
	case SeqClear:
	{
		// �w�i�̕`��
		m_pBg->Draw();

		// �S�[���̕`��
		m_goal.Draw();

		// �v���C���[�̕`��
		if (!m_isDead)
		{
			m_pPlayer->Draw();
		}

		//�藠���̕`��
		m_pShuriken->Draw();

		// �e�̕`��
		if (!m_pShot) return;
		for (int i = 0; i < kShotMax; i++)
		{
			if (!m_pShot[i]) continue;
			m_pShot[i]->Draw();
		}

		// �g���b�v�̕`��
		m_trapManager.Draw();
		m_platformManager.Draw();
		m_moveSpikeMgr.Draw();
		//m_pRock->Draw();

		//�p�[�e�B�N���̕`��
		m_particleMgr.Draw();


		// �t�F�[�h�̕`��
		int fadeAlpha = 0;

		//�t���[���J�E���g���t�F�[�h�̐i�s�x�i����)�ɕϊ�
		float fadeProgress = static_cast<float>(m_fadeFrame) / kFadeFrame;
		//255 -> 0 �ɕω����������̂Ŋ������t�]������
		fadeProgress = 1.0f - fadeProgress;
		//�����������邱�ƂŌ��݂̃t�F�[�h�l�����肷��
		fadeAlpha = 255 * fadeProgress;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���[�h�����ɖ߂�

		//�����𒆉��ɕ\������
		int strWidth = GetDrawFormatStringWidthToHandle(
			m_clearFontHandle,
			"CLEAR!");

		//�N���A�\��
		if (m_goal.IsClear())
		{
			int clearX = (Game::kScreenWidth / 2 - strWidth / 2);
			int clearY = (Game::kScreenHeight / 2 - 60);
			DrawStringToHandle(
				clearX, clearY, "CLEAR!",
				GetColor(2, 155, 1),
				m_clearFontHandle);
		}

		//�^�C�g���ɖ߂�\��
		int titleX = 3;
		int titleY = 3;
		DrawStringToHandle(
			titleX, titleY, "OPSION�Ń^�C�g���ɂ��ǂ�",
			GetColor(255, 255, 255),
			m_backTitleFontHandle);
		break;
	}
	case SeqGameOver:
		// �w�i�̕`��
		m_pBg->Draw();

		// �S�[���̕`��
		m_goal.Draw();

		// �v���C���[�̕`��
		if (!m_isDead)
		{
			m_pPlayer->Draw();
		}

		//�藠���̕`��
		m_pShuriken->Draw();

		// �e�̕`��
		if (!m_pShot) return;
		for (int i = 0; i < kShotMax; i++)
		{
			if (!m_pShot[i]) continue;
			m_pShot[i]->Draw();
		}

		// �g���b�v�̕`��
		m_trapManager.Draw();
		m_platformManager.Draw();
		m_moveSpikeMgr.Draw();
		//m_pRock->Draw();

		//�p�[�e�B�N���̕`��
		m_particleMgr.Draw();

		//�Q�[���I�[�o�[�\��
		//�����𒆉��ɕ\������
		int strGameoverWidth = GetDrawFormatStringWidthToHandle(
			m_gameoverFontHandle,
			"GAMEOVER");

			int gameoverX = (Game::kScreenWidth / 2 - strGameoverWidth / 2);
			int gameoverY = (Game::kScreenHeight / 2 - 60);
			DrawStringToHandle(
				gameoverX, gameoverY, "GAMEOVER",
				GetColor(255, 3, 1),
				m_gameoverFontHandle);
		break;
	}
}

void SceneMain::UpdateShot()
{
	Shot* newShot = nullptr;
	if (m_pPlayer)
	{
		newShot = m_pPlayer->CreateShot();
	}
	if (newShot != nullptr)
	{
		for (int i = 0; i < kShotMax; i++)
		{
			if (m_pShot[i] == nullptr)
			{
				m_pShot[i] = newShot;
				break;
			}
		}
	}

	for (int i = 0; i < kShotMax; i++)
	{
		if (!m_pShot[i]) continue;

		m_pShot[i]->Update();

		// ��ʊO�ɏo���ꍇ
		bool isOffScreen = m_pShot[i]->GetPos().x < 0 || m_pShot[i]->GetPos().x > Game::kScreenWidth;
		if (isOffScreen) DeleteShot(i);
	}
}

void SceneMain::DeleteShot(int index)
{
	if (!m_pShot[index]) return;

	delete m_pShot[index];
	m_pShot[index] = nullptr;
}

void SceneMain::UpdateTitle()
{
	m_sceneTitle.Update();

	//�^�C�g���V�[�P���X�̍X�V����
	if (m_isStartPressed)
	{
		m_sceneTitle.End();
		m_gameSeq = SeqFadeIn; //�V�[�P���X���t�F�[�h�C���ɕύX
	}
	m_isStartPressed = false; //�X�^�[�g�{�^���������ꂽ���̃t���O�����Z�b�g
}

void SceneMain::UpdateFadeIn()
{
	//�t�F�[�h�C���̐i�s
	m_fadeFrame++;
	if (m_fadeFrame > kFadeFrame)
	{
		//�t�F�[�h�C������
		m_fadeFrame = kFadeFrame;
		m_gameSeq = SeqGame; //�V�[�P���X���Q�[���v���C�ɕύX
		m_frameCount = 0; //�t���[���J�E���g�����Z�b�g

		if (CheckSoundMem(m_mainBgmHandle) == 0)
		{
			//BGM�̍Đ��J�n
			m_mainBgmVolume = kMainBgmVolume; //BGM�̉��ʐݒ�

			if (m_gameSeq == SeqGame)
			{
				//BGM�̍Đ�
				PlaySoundMem(m_mainBgmHandle, DX_PLAYTYPE_LOOP);
				ChangeVolumeSoundMem(m_mainBgmVolume, m_mainBgmHandle);
			}
		}
	}
}

void SceneMain::UpdateGame()
{
	DINPUT_JOYSTATE joyState{};

	//PAD��OPSION�{�^���Ń^�C�g���ɖ߂�
	if (GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &joyState) == 0)
	{
		if (((joyState.Buttons[9]) != 0) || (joyState.Buttons[7]) != 0) // OPTION�{�^���������ꂽ��(�R���g���[���[��2����ꍇ���l��)
		{
			//BGM�̒�~
			StopSoundMem(m_mainBgmHandle);
			m_sceneTitle.Init();
			m_gameSeq = SeqTitle; //�V�[�P���X���^�C�g���ɕύX
		}
	}
	
#ifdef _DEBUG
	//�{�^���ꔭ�ŃN���A�ł���f�o�b�O�@�\

	//X�{�^������������N���A����
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//pad��X�{�^���������̓L�[�{�[�h��C���������Ƃ�
	if ((pad & PAD_INPUT_4) != 0)
	{
		//���������𖞂����悤�ȏ���������
		m_goal.m_isClear = true;
		m_gameSeq = SeqClear; //�V�[�P���X���N���A�ɕύX
	}
#endif // _DEBUG

	if (!m_isDead)
	{
		m_pPlayer->Update();
	}
	m_trapManager.Update();
	m_platformManager.Update(m_pPlayer->GetColRect());
	m_moveSpikeMgr.Update();
	m_pShuriken->Update();
	m_goal.Update();
	//m_pRock->Update();
	m_particleMgr.Update();

	//�g�Q���˃C�x���g(X:1000,Y:300���z������)
	if (m_pPlayer->GetPos().x > 1100.0f &&
		m_pPlayer->GetPos().y > 200.0f &&
		m_pPlayer->GetPos().y <= 300.0f &&
		!m_isRtrapFired)
	{
		//�g�Q���ˉ�
		m_fireSpikeSeVolume = kFireSpikeSeVolume; //���ʂ�ݒ�
		PlaySoundMem(m_fireSpikeSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_fireSpikeSeVolume, m_fireSpikeSeHandle);

		m_trapManager.SpawnTrap(
			{ 0.0f, 300.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //�E�ɔ�ԃg�Q

		m_trapManager.SpawnTrap(
			{ 0.0f, 280.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //�E�ɔ�ԃg�Q

		m_isRtrapFired = true; // �g�Q�𔭎ˍς݃t���O�𗧂Ă�
	}

	//������g�Q��ݒu
	if (!m_isUtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 848.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 670.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 900.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 176.0f, 624.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // ������g�Q

	}

	//�������g�Q��ݒu
	if (!m_isLtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 368.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 400.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 432.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 464.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 492.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_isLtrapSpawned = true; // �g�Q��ݒu�ς݃t���O�𗧂Ă�
	}

	//�������g�Q��ݒu
	if (!m_isBtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ -15.0f,  632.0f }, { 240.0f,  528.0f },
			{ 0.0f,   0.0f }); // �������g�Q

		m_isBtrapSpawned = true; // �g�Q��ݒu�ς݃t���O�𗧂Ă�
	}

	//�v���C���[�Ƃ̓����蔻��
	if (m_trapManager.CheckCollision(m_pPlayer->GetColRect()) ||
		m_moveSpikeMgr.CheckCollision(m_pPlayer->GetColRect()) ||
		m_pShuriken->CheckCollision(m_pPlayer->GetColRect()))
	{
		if (!m_isDeadActive)
		{
			//�v���C���[���g�Q�ɓ��������ꍇ�̏���
#ifdef _DEBUG

			printfDx("�g�Q�ɓ��������I\n");
#endif
			//���S����
			m_deadSeVolume = kDeadSeVolume; //���ʂ�ݒ�
			PlaySoundMem(m_deadSeHandle, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(m_deadSeVolume, m_deadSeHandle);
			m_isDeadActive = true;
			m_isDead = true;
		}

		if (m_frame <= 1.0f)
		{
			//���S���o�p��
			m_particleMgr.Spawn(m_pPlayer->GetPos(), 200);
		}
	}

	//�v���b�g�t�H�[������
	if (!m_isPlatformSpawned)
	{
		//�v���b�g�t�H�[���𐶐�
		m_platformManager.SpawnPlatform({ 736.0f, 168.0f }, 10.0f); // 60�t���[����ɗ����J�n
		m_isPlatformSpawned = true; // �v���b�g�t�H�[���𐶐��ς݃t���O�𗧂Ă�
	}

	//�ړ��g�Q����
	if (!m_isMoveSpikeSpawned)
	{
		//�ړ��g�Q�𐶐�
		m_moveSpikeMgr.SpawnSpike({ 400.0f, 363.0f }, { 0.0f, 0.0f }); // �����ʒu�Ƒ��x

		m_moveSpikeMgr.SpawnSpike({ 630.0f, 518.0f }, { 0.0f, 1.0f }); // �����ʒu�Ƒ��x

		m_moveSpikeMgr.SpawnSpike({ 700.0f, 630.0f }, { 0.0f, 1.0f }); // �����ʒu�Ƒ��x

		m_isMoveSpikeSpawned = true; // �ړ��g�Q�𐶐��ς݃t���O�𗧂Ă�
	}

	//�N���A�������ǂ���
	if (m_goal.IsClear())
	{
		m_gameSeq = SeqClear; //�V�[�P���X���N���A�ɕύX
		m_frameCount = 0; //�t���[���J�E���g�����Z�b�g

		//�N���A��
		m_clearSeVolume = kClearSeVolume; //���ʂ�ݒ�
		PlaySoundMem(m_clearSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_clearSeVolume, m_clearSeHandle);
	}

	if (m_isDead)
	{
		m_frame++;
#ifdef _DEBUG
		printfDx("%d\n", m_frame);
#endif
	}
	//2�b�o������V�[���ύX
	if (m_frame >= 120.0f)
	{
		m_frame = 0;
		//�V�[�����Q�[���I�[�o�[�ɕύX
		m_gameSeq = SeqGameOver;
	}

	if (!m_pShot) return;

	UpdateShot();
}

void SceneMain::UpdateClear()
{
	m_pPlayer->Update();
	m_clearFrame++;
	//�N���A�����Ƃ��̏���
	//�N���A�\��
	if (m_clearFrame > 180)
	{
		//�V�[�����^�C�g���ɕύX
		m_sceneTitle.Init();
		m_gameSeq = SeqTitle;
		m_clearFrame = 0;
	}
}

void SceneMain::UpdateGameOver()
{
	//���ׂĂ����Z�b�g����
	m_isDead = false;
	m_isDeadActive = false;
	End();
	Init();

	//�ŏ�����
	m_gameSeq = SeqFadeIn;

}
