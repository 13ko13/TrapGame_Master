#include "Shuriken.h"
#include "DxLib.h"
#include "Game.h"

namespace
	{
	// Vec2�֌W�̒萔��`
	const Vec2 kInitPos = { 1280.0f, 460.0f }; // �藠���̏����ʒu
	constexpr  float kSpeed = 25.0f; // �藠���̈ړ����x

	// �藠���̒萔��`
	constexpr float kShurikenSize = 32.0f; // �藠���̃T�C�Y
	constexpr float kScale = 1.0f; // �藠���̊g�嗦

	// �A�j���[�V�������
	constexpr int kAnimNum = 8; // �藠���̃A�j���[�V�����t���[����
	constexpr int kAnimWaitFrame = 2; // �藠���̃A�j���[�V����1�t���[��������̑ҋ@����

	//����
	constexpr int kFireSeVolume = 80;	//�藠�����ˎ�SE����
}

Shuriken::Shuriken() :
	m_pos({ 0.0f, 0.0f }),
	m_velocity({ 0.0f, 0.0f }),
	m_handle(-1),
	m_isActive(false),
	m_isFired(false),
	m_animFrame(0),
	m_pPlayer(nullptr) 
{
}

Shuriken::~Shuriken()
{
	// �O���t�B�b�N�n���h���̍폜��DxLib�ōs�����߁A�����ł͍s��Ȃ�
}

void Shuriken::Init(int graphHandle)
{
	m_pos.x = kInitPos.x; 
	m_pos.y = kInitPos.y; 
	m_velocity = { kSpeed, 0.0f };
	m_handle = graphHandle;
	m_isActive = false;
	m_isFired = false; 
	m_animFrame = 0;

	//�������[�h
	m_fireSeHandle = LoadSoundMem("data/SpikeFireSe.mp3");

	// �����蔻��p�̋�`��������
	m_colRect.SetCenter(m_pos.x, m_pos.y, kShurikenSize, kShurikenSize);
}

void Shuriken::Update()
{
	if (m_pPlayer->GetPos().x > 400.0f &&
		m_pPlayer->GetPos().y > 360.0f &&
		!m_isFired)
	{
		m_isActive = true; // �v���C���[������̈ʒu�ɓ��B������藠�����A�N�e�B�u�ɂ���
		m_isFired = true; // �藠�������˂��ꂽ�t���O�𗧂Ă�

		//�藠�����ˉ�
		m_fireSeVolume = kFireSeVolume; //���ʂ�ݒ�
		PlaySoundMem(m_fireSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_fireSeVolume, m_fireSeHandle);
	}

	if (!m_isActive) return; // �藠�����A�N�e�B�u�łȂ��ꍇ�͍X�V���Ȃ�

	m_animFrame++;

	// �藠���̈ʒu���X�V
	m_pos -= m_velocity;

	if(m_animFrame >= kAnimNum * kAnimWaitFrame)
	{
		m_animFrame = 0; // �A�j���[�V�����̃t���[���������Z�b�g
	}

	// ��ʊO�ɏo�����A�N�e�B�u�ɂ���
	if (m_pos.x < -kShurikenSize ||
		m_pos.y < -kShurikenSize ||
		m_pos.y > Game::kScreenHeight)
	{
		m_isActive = false;
		return;
	}

	// �����蔻��p�̋�`���X�V
	m_colRect.SetCenter(m_pos.x, m_pos.y, kShurikenSize, kShurikenSize);
}

void Shuriken::Draw()
{
	if (!m_isActive) return; // �藠�����A�N�e�B�u�łȂ��ꍇ�͕`�悵�Ȃ�

	// �A�j���[�V�����̃t���[��������\���������R�}�ԍ����v�Z
	int animNo = m_animFrame / kAnimWaitFrame;

	// �A�j���[�V�����̐i�s�ɍ��킹�ăO���t�B�b�N�̐؂���ʒu��ύX
	int srcX = kShurikenSize * animNo;
	int srcY = 0;

	// �藠���̕`��
	DrawRectGraph(static_cast<int>(m_pos.x) - kShurikenSize * 0.5f,
		static_cast<int>(m_pos.y) - kShurikenSize * 0.5f,
		srcX, srcY, 
		kShurikenSize, kShurikenSize,
		m_handle, true, false);

#ifdef _DEBUG
	//�����蔻���\��
	m_colRect.Draw(0x0000ff, false);
#endif
}

bool Shuriken::IsActive() const
{
	return m_isActive; // �藠�����A�N�e�B�u���ǂ�����Ԃ�
}

bool Shuriken::CheckCollision(const Rect& playerRect) const
{
	if (m_isActive && this->GetRect().IsCollision(playerRect))
	{
		return true; // �v���C���[�Ǝ藠�����Փ�
	}

	return false; // �Փ˂Ȃ�
}

Rect Shuriken::GetRect() const
{
	return m_colRect; // �藠���̓����蔻��p�̋�`��Ԃ�
}

void Shuriken::SetPlayer(Player* player)
{
	m_pPlayer = player; // �v���C���[�̃|�C���^���Z�b�g
}

