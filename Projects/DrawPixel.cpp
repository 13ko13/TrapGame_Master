#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);

	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight,0);

	SetMainWindowText("TrapGame");

	SetWindowSizeChangeEnableFlag(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�`���𗠉�ʂɐݒ�
	SetDrawScreen(DX_SCREEN_BACK);

	SceneMain* m_pScene = new SceneMain;
	m_pScene->Init();

	while (ProcessMessage() == 0)
	{
		//���̃t���[���̊J�n���Ԃ��擾
		LONGLONG start = GetNowHiPerformanceCount();

		//��ʂ��N���A
		ClearDrawScreen();

		//�����ɃQ�[���̏����Ȃǂ�����
		m_pScene->Update();
		
		//�`��
		m_pScene->Draw();

		//����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();

		//�t���[�����[�g60�ɌŒ�
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}

		//ESC�L�[�������ꂽ��E�B���h�E�����
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}
	//��������̃O���t�B�b�N���J��
	m_pScene->End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}