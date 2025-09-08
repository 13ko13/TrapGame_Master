#include "SceneTitle.h"
#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"

namespace
{
	constexpr int kTitleBgmVolume = 100;	//タイトルBGMの音量
	constexpr int kStartSeVolume = 140;	//ゲームスタートの音量
}

SceneTitle::SceneTitle() :
	m_titleGraphHandle(-1),
	m_bgGraphHandle(-1),
	m_startFontHandle(-1),
	m_titleBgmHandle(-1),
	m_startGameSeHandle(-1),
	m_titleBgmVolume(0),
	m_startSeVolume(0),
	m_currentSeq(Seq::SeqFadeIn),
	m_fadeAlpha(255),
	m_time(0),
	m_isFadeIn(false),
	m_isPressStart(false),
	m_pressFrame(0),
	m_pSceneMain(nullptr)
{

}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	//グラフィック読み込み
	m_titleGraphHandle = LoadGraph("data/TitleName.png");
	m_bgGraphHandle = LoadGraph("data/BackGround.png");

	//フォント読み込み
	m_startFontHandle = CreateFontToHandle(
		"x10y12pxDonguriDuel", 60, -1,
		DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//サウンド読み込み
	m_titleBgmHandle = LoadSoundMem("data/TitleBgm.mp3");
	m_startGameSeHandle = LoadSoundMem("data/StartGame.mp3");

	//初期化
	m_time = 0;
	m_isFadeIn = (false);
	m_isPressStart = (false);
	m_currentSeq = Seq::SeqFadeIn;
	m_fadeAlpha = 255; 
	m_pressFrame = 0;
}

void SceneTitle::End()
{
	//BGMの停止
	StopSoundMem(m_titleBgmHandle);

	//グラフィック削除
	DeleteGraph(m_titleGraphHandle);
	DeleteGraph(m_bgGraphHandle);

	//フォント削除
	DeleteFontToHandle(m_startFontHandle);

	//サウンド削除
	DeleteSoundMem(m_titleBgmHandle);
	DeleteSoundMem(m_startGameSeHandle);
}

void SceneTitle::Update()
{
	switch (m_currentSeq)
	{
	case Seq::SeqTitle:
		UpdateTitle();
		if (CheckSoundMem(m_titleBgmHandle) == 0)
		{
			//BGMの再生開始
			m_titleBgmVolume = kTitleBgmVolume; //音量を設定
			//BGMの再生
			PlaySoundMem(m_titleBgmHandle, DX_PLAYTYPE_LOOP);
			ChangeVolumeSoundMem(m_titleBgmVolume, m_titleBgmHandle);
		}
		break;
	case Seq::SeqFadeIn:
		UpdateFadeIn();
		break;
	default:
		break;
	}
}

void SceneTitle::Draw()
{
	//背景
	DrawRotaGraph3(
		0, 0,
		0, 0, 1.0f, 1.0f, 0,
		m_bgGraphHandle, true, false);

	DrawGraph(160, 0, m_titleGraphHandle, true);

	//文字列の幅を取得
	int strWidth = GetDrawFormatStringWidthToHandle(
		m_startFontHandle,
		"Press A to Start"
	);

	//PressStartの点滅
	if (m_currentSeq == Seq::SeqTitle)
	{
		if (m_pressFrame / 30 % 2 == 0)
		{
			if (m_startFontHandle != 1)
			{
				int x = (Game::kScreenWidth * 0.5f - strWidth * 0.5f);
				int y = (Game::kScreenHeight * 0.7f);
				DrawStringToHandle(
					x, y, "Press A to Start",
					GetColor(205, 195, 1),
					m_startFontHandle);
			}
		}
	}
	//フェードイン
	if (m_isFadeIn)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void SceneTitle::UpdateTitle()
{
	m_pressFrame++;
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	if (((pad & PAD_INPUT_A) != 0) && !m_isPressStart)//&演算:ビット単位の演算
	{
		//ゲームスタートSE
		m_startSeVolume = kStartSeVolume; //音量を設定
		PlaySoundMem(m_startGameSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_startSeVolume, m_startGameSeHandle);

		m_isPressStart = true;
	}

	if (m_isPressStart)
	{
		m_time++;
	}

	if (m_time >= 120)
	{
		m_pSceneMain->m_isStartPressed = true;
		m_fadeAlpha = 0;
	}
}

void SceneTitle::UpdateFadeIn()
{
	m_isFadeIn = true;
	m_fadeAlpha -= 5;
	if (m_fadeAlpha <= 0)
	{
		m_fadeAlpha = 0;
		m_isFadeIn = false;
		m_currentSeq = Seq::SeqTitle;
	}
}

void SceneTitle::SetSceneMain(SceneMain* pSceneMain)
{
	m_pSceneMain = pSceneMain;
}
