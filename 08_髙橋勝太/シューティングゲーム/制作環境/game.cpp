//-----------------------------------------------------------------
//
// game (game.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "movebg.h"
#include "boss.h"
#include "effect.h"
#include "result.h"
#include "pause.h"
#include "fade.h"
#include "appeared.h"

//==============================================================================
// グローバル変数
//==============================================================================
bool g_bPause;

//初期状態
int g_nCount = 0;

//--------------
//初期化処理
//--------------
HRESULT InitGame()
{
	g_nCount = 0;
	g_bPause = false;

	//背景の初期化処理
	//InitBG();
	InitMovebg();

	//ポリゴンの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	InitBoss();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	InitAppeared();

	InitEffect();

	//敵キャラの表示
	for (int nCount = 0; nCount < 5; nCount++)
	{

		SetEnemy(D3DXVECTOR3(200.0f * (nCount + 1), 100.0f, 0.0f), 0);
		SetEnemy(D3DXVECTOR3(200.0f * (nCount + 1), 200.0f, 0.0f), 1);
		SetEnemy(D3DXVECTOR3(200.0f * (nCount + 1), 300.0f, 0.0f), 2);
	}

	//SetBoss(D3DXVECTOR3(200.0f, 300.0f, 0.0f), 0);
	//SetBoss(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0.0f), 0);

	//スコア初期化処理
	InitScore();

	for (int nCount = 0; nCount < 8; nCount++)
	{
		SetScore(D3DXVECTOR3(1400 + 50.0f * (nCount + 1), 50.0f, 0.0f), 0);
	}

	// ポーズメニューの初期化処理
	InitPause();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM002);
	//PlaySound(SOUND_LABEL_SE_EXPLOSION);

	return E_NOTIMPL;
}

//----------------
//終了処理
//----------------
void UninitGame(void)
{
	//Directデバイスの開放
	//UninitBG();
	UninitMovebg();

	//ポリゴンの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();

	//敵の終了処理
	UninitEnemy();

	UninitBoss();

	//爆発の終了処理
	UninitExplosion();

	UninitAppeared();

	UninitEffect();

	//スコアの終了処理
	UninitScore();

	//サウンド停止
	StopSound();

	// ポーズメニューの終了処理
	UninitPause();
}

//--------------------
//更新処理
//--------------------
void UpdatetGame(void)
{
	//ポーズボタンを押したとき
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;

	}

	if (g_bPause == true)
	{
		UpdatePause();
	}
	else
	{
		//背景の更新処理
		//UpdateBG();
		UpdateMovebg();

		//ポリゴンの更新処理
		UpdatePlayer();

		//弾の更新処理
		UpdateBullet();

		//敵の更新処理
		UpdateEnemy();

		//ボスの更新処理
		UpdateBoss();

		//爆発の更新処理
		UpdateExplosion();

		UpdateAppeared();

		UpdateEffect();

		//スコアの更新処理
		UpdateScore();

		int nA = rand() % 1000;
		int nB = rand() % 500;
		int nCnt = rand() % 150;

		//敵キャラの表示
		if (nCnt == 1 || nCnt == 2 || nCnt == 3)
		{
			if (g_nCount <= 10)
			{
				g_nCount++;

				if (nCnt == 1)
				{
					SetAppeared(D3DXVECTOR3(nA, nB, 0.0f),50,6);
					SetEnemy(D3DXVECTOR3(nA, nB, 0.0f), 0);
				}

				if (nCnt == 2)
				{
					SetAppeared(D3DXVECTOR3(nA, nB, 0.0f),50,6);
					SetEnemy(D3DXVECTOR3(nA, nB, 0.0f), 1);
				}

				if (nCnt == 3)
				{
					SetAppeared(D3DXVECTOR3(nA, nB, 0.0f),50,6);
					SetEnemy(D3DXVECTOR3(nA, nB, 0.0f), 2);
				}
			}
		}
	}

	if (GetKeyboardPress(DIK_RETURN) == true)
	{
		//画面切り替え
		//SetMode(MODE_RESULT);
	}
}

//--------------------
//描画処理
//--------------------
void DrawGame(void)
{

	//弾の描画処理
	DrawBullet();
	DrawEffect();
	
	DrawAppeared();

	//ポリゴンの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	DrawBoss();

	//爆破の描画処理
	DrawExplosion();

	//スコアの描画処理
	DrawScore();

	//背景の描画処理
	DrawMovebg();

	// ポーズの描画処理
	if (g_bPause == true)
	{
		DrawPause();
	}
}

