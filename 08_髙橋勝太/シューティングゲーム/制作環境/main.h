//-----------------------------------------------------------------
//
// windowsの処理 (main.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#ifndef _MAIN_H_
#define _MAIN_H_

//---------------------
//インクルードファイル
//---------------------
#include <windows.h>
#include "d3dx9.h"//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"//入力処理に必要
#include "xaudio2.h"//サウンド再生に必要

//---------------------
//ライブラリのリンク
//---------------------
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")

//---------------------
//マクロ定義
//---------------------
#define CLASS_NAME "WindowClass"//ウィンドウのクラスの名前
#define WINDOW_NAME "WARNING"//ウィンドウの名前
#define ID_BUTTON000 (101)//ボタンのID
#define ID_EDIT000 (111)//エディットのID
#define WINDOW_POS_X (40)//
#define WINDOW_POS_Y (40)//
#define ID_TIMER (121)//
#define TIMER_INTERVAL (1000/60)//
#define LOOP_PLAYER (3)//
#define SCREEN_WIDTH (1920)//画面の幅
#define SCREEN_HEIGHT (1080)//画面の高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//---------------------
//構造体
//---------------------
typedef enum
{
	MODE_TITLE = 0,//タイトル画面
	MODE_TUTORIAL,
	MODE_GAME,//ゲーム画面
	MODE_RESULT,//リザルト画面
	MODE_MAX,
}MODE;

//------------------------
//プロトタイプ宣言
//------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
//MODE GetMode(void);

//=============
//構造体の定義
//=============
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
} VERTEX_2D;
#endif