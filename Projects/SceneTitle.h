#pragma once

class SceneMain;

class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	void End();
	void Update();
	void Draw();

	void SetSceneMain(SceneMain* pSceneMain);

	void UpdateTitle();		//�^�C�g��
	void UpdateFadeIn();	//�t�F�[�h�C��

	//�V�[�P���X
	enum Seq
	{
		SeqTitle,	//�^�C�g��
		SeqFadeIn,	//�t�F�[�h�C��

		SeqNum	//�V�[�P���X�̐�
	};

private:

	//�g�p����O���t�B�b�N
	int m_titleGraphHandle; //�^�C�g���̃O���t�B�b�N�n���h��
	int m_bgGraphHandle;    //�w�i�̃O���t�B�b�N�n���h��
	
	//�t�H���g�n���h��
	int m_startFontHandle; //PressStart�̃O���t�B�b�N�n���h��

	//�T�E���h�n���h��
	int m_titleBgmHandle;
	int m_startGameSeHandle;		//�Q�[�����X�^�[�g�������̉�

	//�T�E���h�̉���
	int m_titleBgmVolume;	//BGM�̉���
	int m_startSeVolume;	//�Q�[���X�^�[�g�̉���
	
	//�V�[�P���X�Ǘ�
	int m_currentSeq;	//���݂̃V�[�P���X

	//�t�F�[�h�֌W
	int m_fadeAlpha;	//�t�F�[�h�p�̃A���t�@�l

	//����
	int m_time;			//���Ԃ��v��

	//�t���O�֌W
	bool m_isFadeIn;	//�t�F�[�h�C�������ǂ����̃t���O
	bool m_isPressStart;//�X�^�[�g�����������ǂ���

	//�A�j���[�V�����֌W
	int m_pressFrame;   //PressStart�̓_�ŗp�t���[���J�E���g

	//�V�[�����C��
	SceneMain* m_pSceneMain;
};

