
#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------
//
// game (game.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include<stdio.h>
#include "main.h"
#include "camera.h"
#include "light.h"
#include"model.h"
#include "xinput.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "stdio.h"
#include "object.h"
#include "billboard.h"
#include "effect.h"
#include "ui.h"
#include "meshfield.h"
#include "bullet.h"
#include "pause.h"
#include "bg.h"
#include "car.h"
#include "score.h"
#include "gamepad.h"
#include "timer.h"
#include "fade.h"
#include "gear.h"
#include "item.h"

//==============================================================================
// グローバル変数
//==============================================================================
bool g_bPause;						//ポーズ状態判定
int g_PlayerChange = 0;				//動かしているプレイヤー
int g_nDataTime = 0;				//初期状態
int g_nDataTime2 = 0;				//初期状態
int g_nDataItem = 0;				//初期状態
int g_nDataItem2 = 0;				//初期状態
FAILDATE FailDate[MAX_OBJECT];
int g_nObjectDate = 0;
int g_nItemDate = 0;

//--------------
//初期化処理
//--------------
HRESULT InitGame()
{
	g_bPause = false;

	InitPause();

	InitBG();

	InitScore();

	InitGear();

	InitMeshfield();

	InitWall();

	InitBillboard();

	InitBullet();

	InitShadow();

	InitPlayer();

	InitCar();

	InitObject();

	InitItem();

	InitCamera();

	Initlight();

	InitEffect();

	InitUi();

	InitTimer();

	//ギアの表示
	SetGear(D3DXVECTOR3(100, SCREEN_HEIGHT - 150.0f, 0.0f), 0);

	//スコアの表示
	for (int nCount = 0; nCount < 8; nCount++)
	{
		SetScore(D3DXVECTOR3(1600 + 70.0f * (nCount + 1), 50.0f, 0.0f), 0);
	}

	//タイムの表示
	//分
	for (int nCount = 0; nCount < 2; nCount++)
	{
		SetTimer(D3DXVECTOR3(90 + 50.0f * (nCount + 1), 50.0f, 0.0f), 0);
	}
	//秒
	for (int nCount = 0; nCount < 2; nCount++)
	{
		SetTimer(D3DXVECTOR3(250 + 50.0f * (nCount + 1), 50.0f, 0.0f), 0);
	}

	//フィールドの設定
	SetField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),  0.0f, 0.0f);	//平面

	//アイテム
	/*SetItem(D3DXVECTOR3(760.4, 0.0f, -472.4), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(2212.4, 0.0f, -292.3), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(2212.4, 0.0f, 379.4), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(4130.6, 0.0f, 906.2), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(4130.6, 0.0f, 3930.0), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-824.8, 0.0f, 3929.8), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-3093.5, 0.0f, 3341.8), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-3668.0, 0.0f, 2295.0), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-3668.0, 0.0f, 754.7), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-2382.2, 0.0f, -773.9), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-1554.5, 0.0f, -2155.2), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-2300.6, 0.0f, -2957.9), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-1580.6, 0.0f, -3627.3), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(-847.3, 0.0f, -2968.8), D3DXVECTOR3(0.0f, 0, 0.0f), 0);
	SetItem(D3DXVECTOR3(4029.8, 0.0f, -3801.2), D3DXVECTOR3(0.0f, 0, 0.0f), 0);*/

	return E_NOTIMPL;
}

//----------------
//終了処理
//----------------
void UninitGame(void)
{
	UninitBG();

	UninitScore();

	UninitGear();

	UninitMeshfield();

	UninitWall();

	UninitBillboard();

	UninitBullet();

	UninitShadow();

	UninitPlayer();

	UninitCar();

	UninitObject();

	UninitItem();

	UninitCamera();

	Uninitlight();

	UninitEffect();

	UninitUi();

	UninitTimer();

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
		UpdateMeshfield();

		UpdateScore();

		UpdateGear();

		UpdateBullet();

		UpdateWall();

		UpdateBillboard();

		UpdateShadow();

		//プレイヤーの切り替え
		if (g_PlayerChange == 1)
		{
			UpdatePlayer();
		}

		//プレイヤーの切り替え
		if (g_PlayerChange == 0)
		{
			UpdateCar();
		}

		UpdateObject();

		UpdateItem();

		UpdateCamera();

		Updatelight();

		UpdateEffect();

		UpdateUi();

		UpdateBG();

		UpdateTimer();

		AddTimer(1);

		//プレイヤーを動かす
		if (GetKeyboardTrigger(DIK_2) == true)
		{
			g_PlayerChange = 1;
		}

		//車をを動かす
		if (GetKeyboardTrigger(DIK_1) == true)
		{
			g_PlayerChange = 0;
		}

		PLAYER *player;
		player = GetModel();

		//SetWater(D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z));
		
		//プレイヤーの位置にオブジェクトを生成
		if (GetKeyboardTrigger(DIK_G) == true)
		{
			SetOject(D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z), D3DXVECTOR3(0.0f, g_nDataTime2, 0.0f), g_nDataTime);

			//txtファイルに書き込む
			FailObject();

			//Gボタン押した回数をカウント
			g_nObjectDate += 1;
		}

		//プレイヤーの位置にアイテムを生成
		if (GetKeyboardTrigger(DIK_T) == true)
		{
			SetItem(D3DXVECTOR3(player->pos.x, player->pos.y, player->pos.z), D3DXVECTOR3(0.0f, g_nDataItem2, 0.0f), g_nDataItem);

			//txtファイルに書き込む
			FailItem();

			//Tボタン押した回数をカウント
			g_nItemDate += 1;
		}

		//最新のオブジェクトを消去
		if (g_nObjectDate >= 1)
		{
			if (GetKeyboardTrigger(DIK_H) == true)
			{
				//Hボタン押した回数をカウント
				//情報を受け渡す
				g_nObjectDate -= 1;

				//txtファイルに書き込む
				FailObject();
			}
		}

		//最新のオブジェクトを消去
		if (g_nItemDate >= 1)
		{
			if (GetKeyboardTrigger(DIK_Y) == true)
			{
				//Hボタン押した回数をカウント
				//情報を受け渡す
				g_nItemDate -= 1;

				//txtファイルに書き込む
				FailItem();
			}
		}

		//オブジェクトのタイプを変える
		if (GetKeyboardTrigger(DIK_UP) == true)
		{
			g_nDataTime += 1;
		}

		if (GetKeyboardTrigger(DIK_DOWN) == true)
		{
			g_nDataTime -= 1;
		}

		//オブジェクトタイプの上限時の場合0に戻す
		if (g_nDataTime > GetobjectType() - 1.0f)
		{
			g_nDataTime = 0.0f;
		}
		//オブジェクトタイプの上限時の場合MAXに戻す
		if (g_nDataTime == -1.0f)
		{
			g_nDataTime = GetobjectType() - 1.0f;
		}

		//アイテムのタイプを変える
		if (GetKeyboardTrigger(DIK_UP) == true)
		{
			g_nDataTime += 1;
		}

		if (GetKeyboardTrigger(DIK_DOWN) == true)
		{
			g_nDataTime -= 1;
		}

		//アイテムタイプの上限時の場合0に戻す
		if (g_nDataTime > GetitemType() - 1.0f)
		{
			g_nDataTime = 0.0f;
		}
		//アイテムタイプの上限時の場合MAXに戻す
		if (g_nDataTime == -1.0f)
		{
			g_nDataTime = GetitemType() - 1.0f;
		}

		//オブジェクトの向きを変える
		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_nDataTime2 += 1.57f;
		}

		if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_nDataTime2 -= 1.0f;
		}
	}

	//if (GetKeyboardPress(DIK_N) == true)
	//{
	//	//画面切り替え
	//	SetMode(MODE_RESULT);
	//}
}

//--------------------
//描画処理
//--------------------
void DrawGame(void)
{
	SetCamera();

	//DrawBG();

	DrawMeshfield();

	DrawObject();

	DrawItem();

	DrawWall();

	DrawBillboard();

	DrawShadow();

	DrawBullet();

	DrawPlayer();

	DrawCar();

	DrawEffect();

	DrawUi();

	DrawTimer();

	DrawScore();

	DrawGear();

	// ポーズの描画処理
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetGame(void)
{
	return g_PlayerChange;
}

//---------------------------------------------
//データの受け渡し
//---------------------------------------------
int GetItem(void)
{
	return g_nDataTime;
}

//---------------------------------------------
//書き出し
//---------------------------------------------
int FailObject()
{
	PLAYER *player;
	player = GetModel();

	FILE *fp;

	//「fprintfExample.txt」のファイルを書き込みモード（"w"）で開く
	fp = fopen("SetObject.txt", "w");

	//オブジェクトの位置を記憶
	FailDate[g_nObjectDate].X = player->pos.x;
	FailDate[g_nObjectDate].Z = player->pos.z;
	FailDate[g_nObjectDate].Rot = g_nDataTime2;
	FailDate[g_nObjectDate].Type = g_nDataTime;

	for (int nCnt = 0; nCnt < g_nObjectDate + 1; nCnt++)
	{
		//「txt」に書き込む
		fprintf(fp, "SetOject(D3DXVECTOR3(%.1f\, 0.0f, %.1f\), D3DXVECTOR3(0.0f, %d\, 0.0f), %d\);\n", FailDate[nCnt].X, FailDate[nCnt].Z, FailDate[nCnt].Rot, FailDate[nCnt].Type);
	}

	//「txt」のファイルを閉じる
	fclose(fp);

	return 0;
}

int FailItem()
{
	PLAYER *player;
	player = GetModel();

	FILE *fp;

	//「fprintfExample.txt」のファイルを書き込みモード（"w"）で開く
	fp = fopen("SetItem.txt", "w");

	//オブジェクトの位置を記憶
	FailDate[g_nItemDate].X = player->pos.x;
	FailDate[g_nItemDate].Z = player->pos.z;
	FailDate[g_nItemDate].Rot = g_nDataItem2;
	FailDate[g_nItemDate].Type = g_nDataItem;

	for (int nCnt = 0; nCnt < g_nItemDate + 1; nCnt++)
	{
		//「txt」に書き込む
		fprintf(fp, "SetItem(D3DXVECTOR3(%.1f\, 0.0f, %.1f\), D3DXVECTOR3(0.0f, %d\, 0.0f), %d\);\n", FailDate[nCnt].X, FailDate[nCnt].Z, FailDate[nCnt].Rot, FailDate[nCnt].Type);
	}

	//「txt」のファイルを閉じる
	fclose(fp);

	return 0;
}

