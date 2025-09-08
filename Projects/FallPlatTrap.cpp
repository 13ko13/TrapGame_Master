#include "FallPlatTrap.h"
#include "DxLib.h"
#include "PlatformManager.h"
#include "MoveSpike.h"

namespace
{
	constexpr float kPlatformWidth = 128.0f; // �v���b�g�t�H�[���̕�
	constexpr float kPlatformHeight = 16.0f; // �v���b�g�t�H�[���̍���
	//�v���b�g�t�H�[���̊g�嗦
	constexpr float kChipScale = 1.0f;
	//�`�b�v��̑傫��
	constexpr int kChipWidth = 32;
	constexpr int kChipHeight = 32;
	constexpr int kChipSize = 32;

	//�`�b�v�����u�����̏��
	constexpr int kChipNumX = 2; // ���ɒu���`�b�v�̐�
	constexpr int kChipNumY = 1; // �c�ɒu���`�b�v�̐�
}

FallPlatTrap::FallPlatTrap() :
	m_pos({ 0.0f, 0.0f }),
	m_delayFrames(0),
	m_currentFrames(0),
	m_handle(-1),
	m_isActive(false),
	m_playerOnPlatform(false)
{
}

FallPlatTrap::~FallPlatTrap()
{
	// �O���t�B�b�N�폜��DxLib�ōs�����߁A�����ł͉������Ȃ�
}

void FallPlatTrap::Init(const Vec2& pos, int delayFrames, int graphHandle)
{
	m_pos = pos;
	m_delayFrames = delayFrames;
	m_currentFrames = 0;
	m_handle = graphHandle;
	m_isActive = true;

	// �����蔻��p�̋�`��ݒ�
	m_colRect.SetCenter(m_pos.x + 32.0f , m_pos.y, kPlatformWidth, kPlatformHeight);
}

void FallPlatTrap::Update(const Rect& playerRect)
{
	if (!m_isActive) return; // �g���b�v���A�N�e�B�u�łȂ��ꍇ�͍X�V���Ȃ�

	// �v���C���[���v���b�g�t�H�[����ɂ��邩�ǂ������`�F�b�N
	m_playerOnPlatform = m_colRect.IsCollision(playerRect);

	if (m_playerOnPlatform)
	{
		// �v���C���[���v���b�g�t�H�[����ɂ���ꍇ�A�t���[�����J�E���g
		m_currentFrames++;
		if (m_currentFrames >= m_delayFrames)
		{
			// �w�肳�ꂽ�t���[�����o�ߌ�A�g���b�v���A�N�e�B�u�ɂ���
			m_isActive = false;
		}
	}
	else
	{
		m_currentFrames = 0; // �v���C���[�����Ȃ��ꍇ�̓J�E���g�����Z�b�g
	}
	// �����蔻��p�̋�`���X�V
	m_colRect.SetCenter(m_pos.x + 32.0f , m_pos.y, kPlatformWidth, kPlatformHeight);
}

void FallPlatTrap::Draw()
{
	if (!m_isActive) return; // �g���b�v���A�N�e�B�u�łȂ��ꍇ�͕`�悵�Ȃ�

	int DrawPosX = 1000; // �`��ʒuX
	int DrawPosY = 300; // �`��ʒuY

	// �v���b�g�t�H�[���̕`��
	//�}�b�v�`�b�v�̂Ƃ���\�����邩
	//�`��
	DrawFallPlat(DrawPosX, DrawPosY, m_pos.x, m_pos.y);
	DrawFallPlat(DrawPosX + 32, DrawPosY, m_pos.x + 32, m_pos.y);
	DrawFallPlat(DrawPosX + 64, DrawPosY, m_pos.x + 64, m_pos.y);
	DrawFallPlat(DrawPosX + 96, DrawPosY, m_pos.x + 96, m_pos.y);
}

bool FallPlatTrap::IsActive() const
{
	return m_isActive;
}

Rect FallPlatTrap::GetRect() const
{
	return m_colRect;
}

void FallPlatTrap::DrawFallPlat(int DrawPosX, int DrawPosY, int posX, int posY)
{
	// �v���b�g�t�H�[���̕`��
	DrawRectRotaGraph(
		posX - kChipSize * 0.5f,
		posY + kPlatformHeight * 0.5f ,
		0, 0,
		kChipSize, kChipSize,
		kChipScale, 0.0f,
		m_handle, true);

#ifdef _DEBUG
	//�����蔻��
	DrawBox(
		posX - kChipWidth ,
		posY - kPlatformHeight * 0.5f,
		posX ,
		posY + kPlatformHeight + 7,
		GetColor(255, 0, 0),
		false);
#endif // DEBUG
}