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

//--------------
//初期化処理
//--------------
HRESULT InitTutorial()
{
	//背景の初期化処理
	InitBG();
	InitMovebg();

	//ポリゴンの初期化処理
	InitPlayer();

	InitBoss();

	//弾の初期化処理
	InitBullet();
	InitEffect();

	//スコア初期化処理
	InitScore();

	for (int nCount = 0; nCount < 8; nCount++)
	{
		SetScore(D3DXVECTOR3(1400 + 50.0f * (nCount + 1), 50.0f, 0.0f), 0);
	}

	return E_NOTIMPL;
}

//----------------
//終了処理
//----------------
void UninitTutorial(void)
{
	//Directデバイスの開放
	UninitBG();
	UninitMovebg();

	//ポリゴンの終了処理
	UninitPlayer();

	//弾の終了処理
	UninitBullet();
	UninitEffect();

	//スコアの終了処理
	UninitScore();
}

//--------------------
//更新処理
//--------------------
void UpdateTutorial(void)
{
	//背景の更新処理
	UpdateBG();
	UpdateMovebg();

	//ポリゴンの更新処理
	UpdatePlayer();

	//弾の更新処理
	UpdateBullet();
	UpdateEffect();

	//スコアの更新処理
	UpdateScore();

	if (GetKeyboardPress(DIK_RETURN) == true)
	{
		//画面切り替え
		SetMode(MODE_GAME);
	}
}

//--------------------
//描画処理
//--------------------
void DrawTutorial(void)
{

	//弾の描画処理
	DrawBullet();
	DrawEffect();

	//ポリゴンの描画処理
	DrawPlayer();

	//スコアの描画処理
	DrawScore();

	//背景の描画処理
	DrawMovebg();
	DrawBG();
}

