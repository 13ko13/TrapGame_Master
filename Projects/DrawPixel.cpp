#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);

	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight,0);

	SetMainWindowText("TrapGame");

	SetWindowSizeChangeEnableFlag(TRUE);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//描画先を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	SceneMain* m_pScene = new SceneMain;
	m_pScene->Init();

	while (ProcessMessage() == 0)
	{
		//このフレームの開始時間を取得
		LONGLONG start = GetNowHiPerformanceCount();

		//画面をクリア
		ClearDrawScreen();

		//ここにゲームの処理などを書く
		m_pScene->Update();
		
		//描画
		m_pScene->Draw();

		//裏画面の内容を表画面に反映
		ScreenFlip();

		//フレームレート60に固定
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}

		//ESCキーが押されたらウィンドウを閉じる
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}
	//メモリ上のグラフィックを開放
	m_pScene->End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}