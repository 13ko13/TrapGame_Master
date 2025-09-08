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
	constexpr int kShotMax = 5;	//1度に撃てる最大弾数
	constexpr int kFadeFrame = 30; //フェードにかかるフレーム数
	constexpr int kMainBgmVolume = 50; //BGMの音量
	constexpr int kClearSeVolume = 60;	//クリアの音量
	constexpr int kFireSpikeSeVolume = 100;	//トゲ発射時音量
	constexpr int kDeadSeVolume = 100;	//死亡時音量
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
	//フレーム数の初期化
	m_frame = 0;

	//初期シーケンスの決定
	m_gameSeq = SeqTitle;

	m_frameCount = 0;

	//フェード状態の初期化
	m_fadeFrame = 0;

	//グラフィックをロードする
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

	//フォントのロード
	m_clearFontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 120, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_backTitleFontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 20, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_gameoverFontHandle = CreateFontToHandle("x10y12pxDonguriDuel", 140, -1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//サウンドのロード
	m_mainBgmHandle = LoadSoundMem("data/MainBgm.mp3");
	m_clearSeHandle = LoadSoundMem("data/ClearSe.mp3");
	m_fireSpikeSeHandle = LoadSoundMem("data/SpikeFireSe.mp3");
	m_deadSeHandle = LoadSoundMem("data/DeadSe.mp3");

	//プレイヤーの初期化
	m_pPlayer->Init(
		m_playerIdleGraphHandle, m_playerIdleGraphHandle,
		m_playerWalkGraphHandle, m_playerShotGraphHnadle,
		m_playerJumpGraphHandle, m_playerDJumpGraphHandle);

	//タイトルシーンの初期化
	m_sceneTitle.Init();

	//背景の初期化
	m_pBg->Init();

	//トラップの初期化
	m_trapManager.Init(
		m_RtrapGraphHandle, m_UtrapGraphHandle,
		m_LtrapGraphHandle, m_BtrapGraphHandle);

	//消える地面の初期化
	m_platformManager.Init(m_platformGraphHandle);

	//プレイヤーにプラットフォームマネージャーをセット
	m_pPlayer->SetPlatformManager(&m_platformManager);

	//プレイヤーのポインタを取得
	m_goal.SetPlayer(m_pPlayer);
	m_trapManager.SetPlayer(m_pPlayer);

	//このシーンのポインタを渡す
	m_sceneTitle.SetSceneMain(this);

	//移動トゲの初期化
	m_moveSpikeMgr.Init(m_moveSpikeGraphHandle);

	//手裏剣の初期化
	m_pShuriken->Init(m_shurikenGraphHandle);

	//ゴールの初期化
	m_goal.Init(m_goalGraphHandle);

	//岩の初期化
	//m_pRock->Init(m_rockGraphHandle);

	//手裏剣のポインタをプレイヤーにセット
	m_pShuriken->SetPlayer(m_pPlayer);

	//岩にプレイヤーのポインタをセット
	//m_pRock->SetPlayer(m_pPlayer);

	//フラグの初期化
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

	//トゲ発射イベント(X:1000,Y:300を越えたら)
	if (m_pPlayer->GetPos().x > 1100.0f &&
		m_pPlayer->GetPos().y > 200.0f &&
		m_pPlayer->GetPos().y <= 300.0f &&
		!m_isRtrapFired)
	{
		m_trapManager.SpawnTrap(
			{ 0.0f, 300.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //右に飛ぶトゲ

		m_trapManager.SpawnTrap(
			{ 0.0f, 280.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //右に飛ぶトゲ

		m_isRtrapFired = true; // トゲを発射済みフラグを立てる
	}

	//上向きトゲを設置
	if (!m_isUtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 848.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 670.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 900.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 176.0f, 624.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_isUtrapSpawned = true; // トゲを設置済みフラグを立てる
	}

	//左向きトゲを設置
	if (!m_isLtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 368.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 400.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 432.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 464.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 492.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_isLtrapSpawned = true; // トゲを設置済みフラグを立てる
	}

	//下向きトゲを設置
	if (!m_isBtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ -15.0f,  632.0f }, { 240.0f,  528.0f },
			{ 0.0f,   9.0f }); // 下向きトゲ

		m_isBtrapSpawned = true; // トゲを設置済みフラグを立てる
	}

	//プラットフォーム生成
	if (!m_isPlatformSpawned)
	{
		//プラットフォームを生成
		m_platformManager.SpawnPlatform({ 736.0f, 168.0f }, 0.1f); // 0.1フレーム後に落下開始
		m_isPlatformSpawned = true; // プラットフォームを生成済みフラグを立てる
	}

	//移動トゲ生成
	if (!m_isMoveSpikeSpawned)
	{
		//移動トゲを生成
		m_moveSpikeMgr.SpawnSpike({ 400.0f, 363.0f }, { 0.0f, 0.0f }); // 初期位置と速度

		m_moveSpikeMgr.SpawnSpike({ 630.0f, 518.0f }, { 0.0f, 1.0f }); // 初期位置と速度

		m_moveSpikeMgr.SpawnSpike({ 700.0f, 630.0f }, { 0.0f, 1.0f }); // 初期位置と速度

		m_isMoveSpikeSpawned = true; // 移動トゲを生成済みフラグを立てる
	}

	int x = 20;
	int y = 3;
	DrawStringToHandle(
		x, y, "OPSIONでタイトルにもどる",
		GetColor(255, 255, 255),
		m_backTitleFontHandle);
}

void SceneMain::End()
{
	//BGMの停止
	StopSoundMem(m_mainBgmHandle);

	m_pPlayer->End();
	m_pBg->End();
	m_moveSpikeMgr.End();
	//m_pRock->End();

	//グラフィックを開放
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

	//フォントの削除
	DeleteFontToHandle(m_clearFontHandle);
	DeleteFontToHandle(m_backTitleFontHandle);
	DeleteFontToHandle(m_gameoverFontHandle);

	//サウンドの削除
	DeleteSoundMem(m_mainBgmHandle);
}

void SceneMain::Update()
{
	/*printfDx("現在のシーン:%d\n", m_gameSeq);*/
	//現在のシーケンスの経過フレーム数をカウント
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
		//BGMの停止
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
		// 背景の描画
		m_pBg->Draw();

		// ゴールの描画
		m_goal.Draw();

		// プレイヤーの描画
		if (!m_isDead)
		{
			m_pPlayer->Draw();
		}

		//手裏剣の描画
		m_pShuriken->Draw();

		// 弾の描画
		if (!m_pShot) return;
		for (int i = 0; i < kShotMax; i++)
		{
			if (!m_pShot[i]) continue;
			m_pShot[i]->Draw();
		}

		// トラップの描画
		m_trapManager.Draw();
		m_platformManager.Draw();
		m_moveSpikeMgr.Draw();
		//m_pRock->Draw();

		//パーティクルの描画
		m_particleMgr.Draw();


		// フェードの描画
		int fadeAlpha = 0;

		//フレームカウントをフェードの進行度（割合)に変換
		float fadeProgress = static_cast<float>(m_fadeFrame) / kFadeFrame;
		//255 -> 0 に変化させたいので割合を逆転させる
		fadeProgress = 1.0f - fadeProgress;
		//割合をかけることで現在のフェード値を決定する
		fadeAlpha = 255 * fadeProgress;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードを元に戻す

		//文字を中央に表示する
		int strWidth = GetDrawFormatStringWidthToHandle(
			m_clearFontHandle,
			"CLEAR!");

		//クリア表示
		if (m_goal.IsClear())
		{
			int clearX = (Game::kScreenWidth / 2 - strWidth / 2);
			int clearY = (Game::kScreenHeight / 2 - 60);
			DrawStringToHandle(
				clearX, clearY, "CLEAR!",
				GetColor(2, 155, 1),
				m_clearFontHandle);
		}

		//タイトルに戻る表示
		int titleX = 3;
		int titleY = 3;
		DrawStringToHandle(
			titleX, titleY, "OPSIONでタイトルにもどる",
			GetColor(255, 255, 255),
			m_backTitleFontHandle);
		break;
	}
	case SeqGameOver:
		// 背景の描画
		m_pBg->Draw();

		// ゴールの描画
		m_goal.Draw();

		// プレイヤーの描画
		if (!m_isDead)
		{
			m_pPlayer->Draw();
		}

		//手裏剣の描画
		m_pShuriken->Draw();

		// 弾の描画
		if (!m_pShot) return;
		for (int i = 0; i < kShotMax; i++)
		{
			if (!m_pShot[i]) continue;
			m_pShot[i]->Draw();
		}

		// トラップの描画
		m_trapManager.Draw();
		m_platformManager.Draw();
		m_moveSpikeMgr.Draw();
		//m_pRock->Draw();

		//パーティクルの描画
		m_particleMgr.Draw();

		//ゲームオーバー表示
		//文字を中央に表示する
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

		// 画面外に出た場合
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

	//タイトルシーケンスの更新処理
	if (m_isStartPressed)
	{
		m_sceneTitle.End();
		m_gameSeq = SeqFadeIn; //シーケンスをフェードインに変更
	}
	m_isStartPressed = false; //スタートボタンが押されたかのフラグをリセット
}

void SceneMain::UpdateFadeIn()
{
	//フェードインの進行
	m_fadeFrame++;
	if (m_fadeFrame > kFadeFrame)
	{
		//フェードイン完了
		m_fadeFrame = kFadeFrame;
		m_gameSeq = SeqGame; //シーケンスをゲームプレイに変更
		m_frameCount = 0; //フレームカウントをリセット

		if (CheckSoundMem(m_mainBgmHandle) == 0)
		{
			//BGMの再生開始
			m_mainBgmVolume = kMainBgmVolume; //BGMの音量設定

			if (m_gameSeq == SeqGame)
			{
				//BGMの再生
				PlaySoundMem(m_mainBgmHandle, DX_PLAYTYPE_LOOP);
				ChangeVolumeSoundMem(m_mainBgmVolume, m_mainBgmHandle);
			}
		}
	}
}

void SceneMain::UpdateGame()
{
	DINPUT_JOYSTATE joyState{};

	//PADのOPSIONボタンでタイトルに戻る
	if (GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &joyState) == 0)
	{
		if (((joyState.Buttons[9]) != 0) || (joyState.Buttons[7]) != 0) // OPTIONボタンが押されたら(コントローラーが2つある場合を考慮)
		{
			//BGMの停止
			StopSoundMem(m_mainBgmHandle);
			m_sceneTitle.Init();
			m_gameSeq = SeqTitle; //シーケンスをタイトルに変更
		}
	}
	
#ifdef _DEBUG
	//ボタン一発でクリアできるデバッグ機能

	//Xボタンを押したらクリアする
	int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	//padのXボタンもしくはキーボードのCを押したとき
	if ((pad & PAD_INPUT_4) != 0)
	{
		//勝利条件を満たすような処理を書く
		m_goal.m_isClear = true;
		m_gameSeq = SeqClear; //シーケンスをクリアに変更
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

	//トゲ発射イベント(X:1000,Y:300を越えたら)
	if (m_pPlayer->GetPos().x > 1100.0f &&
		m_pPlayer->GetPos().y > 200.0f &&
		m_pPlayer->GetPos().y <= 300.0f &&
		!m_isRtrapFired)
	{
		//トゲ発射音
		m_fireSpikeSeVolume = kFireSpikeSeVolume; //音量を設定
		PlaySoundMem(m_fireSpikeSeHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_fireSpikeSeVolume, m_fireSpikeSeHandle);

		m_trapManager.SpawnTrap(
			{ 0.0f, 300.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //右に飛ぶトゲ

		m_trapManager.SpawnTrap(
			{ 0.0f, 280.0f }, { -15.0f,-15.0f },
			{ -15.0f, -15.0f }, { -15.0f,-15.0f },
			{ 40.0f,   0.0f }); //右に飛ぶトゲ

		m_isRtrapFired = true; // トゲを発射済みフラグを立てる
	}

	//上向きトゲを設置
	if (!m_isUtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 848.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 670.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 900.0f, 304.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { 176.0f, 624.0f },
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 上向きトゲ

	}

	//左向きトゲを設置
	if (!m_isLtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 368.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 400.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 432.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 464.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ 1233.0f, 492.0f }, { -15.0f, -15.0f },
			{ 0.0f,   0.0f }); // 左向きトゲ

		m_isLtrapSpawned = true; // トゲを設置済みフラグを立てる
	}

	//下向きトゲを設置
	if (!m_isBtrapSpawned)
	{
		m_trapManager.SpawnTrap(
			{ -15.0f, -15.0f }, { -15.0f, -15.0f },
			{ -15.0f,  632.0f }, { 240.0f,  528.0f },
			{ 0.0f,   0.0f }); // 下向きトゲ

		m_isBtrapSpawned = true; // トゲを設置済みフラグを立てる
	}

	//プレイヤーとの当たり判定
	if (m_trapManager.CheckCollision(m_pPlayer->GetColRect()) ||
		m_moveSpikeMgr.CheckCollision(m_pPlayer->GetColRect()) ||
		m_pShuriken->CheckCollision(m_pPlayer->GetColRect()))
	{
		if (!m_isDeadActive)
		{
			//プレイヤーがトゲに当たった場合の処理
#ifdef _DEBUG

			printfDx("トゲに当たった！\n");
#endif
			//死亡時音
			m_deadSeVolume = kDeadSeVolume; //音量を設定
			PlaySoundMem(m_deadSeHandle, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(m_deadSeVolume, m_deadSeHandle);
			m_isDeadActive = true;
			m_isDead = true;
		}

		if (m_frame <= 1.0f)
		{
			//死亡演出継続
			m_particleMgr.Spawn(m_pPlayer->GetPos(), 200);
		}
	}

	//プラットフォーム生成
	if (!m_isPlatformSpawned)
	{
		//プラットフォームを生成
		m_platformManager.SpawnPlatform({ 736.0f, 168.0f }, 10.0f); // 60フレーム後に落下開始
		m_isPlatformSpawned = true; // プラットフォームを生成済みフラグを立てる
	}

	//移動トゲ生成
	if (!m_isMoveSpikeSpawned)
	{
		//移動トゲを生成
		m_moveSpikeMgr.SpawnSpike({ 400.0f, 363.0f }, { 0.0f, 0.0f }); // 初期位置と速度

		m_moveSpikeMgr.SpawnSpike({ 630.0f, 518.0f }, { 0.0f, 1.0f }); // 初期位置と速度

		m_moveSpikeMgr.SpawnSpike({ 700.0f, 630.0f }, { 0.0f, 1.0f }); // 初期位置と速度

		m_isMoveSpikeSpawned = true; // 移動トゲを生成済みフラグを立てる
	}

	//クリアしたかどうか
	if (m_goal.IsClear())
	{
		m_gameSeq = SeqClear; //シーケンスをクリアに変更
		m_frameCount = 0; //フレームカウントをリセット

		//クリア音
		m_clearSeVolume = kClearSeVolume; //音量を設定
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
	//2秒経ったらシーン変更
	if (m_frame >= 120.0f)
	{
		m_frame = 0;
		//シーンをゲームオーバーに変更
		m_gameSeq = SeqGameOver;
	}

	if (!m_pShot) return;

	UpdateShot();
}

void SceneMain::UpdateClear()
{
	m_pPlayer->Update();
	m_clearFrame++;
	//クリアしたときの処理
	//クリア表示
	if (m_clearFrame > 180)
	{
		//シーンをタイトルに変更
		m_sceneTitle.Init();
		m_gameSeq = SeqTitle;
		m_clearFrame = 0;
	}
}

void SceneMain::UpdateGameOver()
{
	//すべてをリセットする
	m_isDead = false;
	m_isDeadActive = false;
	End();
	Init();

	//最初から
	m_gameSeq = SeqFadeIn;

}
