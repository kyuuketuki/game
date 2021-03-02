#define _CRT_SECURE_NO_WARNINGS
//======================
//
//Windowsの処理[main.cpp]
//Author: shota
//======================

//------------------------
//インクルードファイル
//------------------------
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
#include "meshfield.h"
#include "bullet.h"
#include "game.h"
#include "title.h"
#include"fade.h"
#include "result.h"
#include"car.h"
#include"gamepad.h"
#include "ranking.h"
#include "tutorial.h"
#include "sound.h"
#include <stdio.h>
#include <string.h>

//================
//プロトタイプ宣言
//================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//===============
//グローバル変数
//===============
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;//フォントのポインタ
int g_nCountFPS;//FPSカウンター
int g_camera;
int g_Itemdate;
//MODE g_mode = MODE_TITLE; //モードの種類
MODE g_mode = MODE_GAME; //モードの種類

//==========
//メイン関数
//==========
int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_EXCLAMATION),
		LoadCursor(NULL,IDC_HAND),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_EXCLAMATION),
	};
	HWND hWnd;
	MSG msg;

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウの作成
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,//ウィンドウクラスの名前
		WINDOW_NAME,//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	//ウインドウ
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	////フルスクリーン
	//if (FAILED(Init(hInstance, hWnd, false)))
	//{
	//	return -1;
	//}

	//ウィンドウの表示
	UpdateWindow(hWnd);

	ShowWindow(hWnd, nCmdShow);
	DWORD dwExcelastTime;//処理実行最終時刻
	DWORD dwFPSLastTime;//FPS計測実行最終時刻
	DWORD dwCurrentTime;//現在時刻
	DWORD dwFrameCount;//フレームカウント

	dwExcelastTime = dwFPSLastTime = dwCurrentTime = timeGetTime();
	dwFrameCount = 0;//現在時刻を設定

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExcelastTime) >= 500)//0.5秒経過
			//if ((dwCurrentTime - dwExcelastTime) >= 1000)//0.5秒経過
			{
				//g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//保存
				dwFrameCount = 0;//リセット
			}

			if ((dwCurrentTime - dwExcelastTime) > (1000 / 60))
			{
				dwFPSLastTime = dwCurrentTime;//保存

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	//終了処理
	Uninit();

	//ウィンドウクラスの登録解除
	UnregisterClass(CLASS_NAME,//ウィンドウクラスの名前
		wcex.hInstance);
	return (int)msg.wParam;
}

//------------------------------------
//ウィンドウプロシージャ
//------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	switch (uMsg)
	{

	case WM_DESTROY://ウィンドウを破棄する

		PostQuitMessage(0);//WM_QUITメッセージを返す

		break;

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_BUTTON000)
		{
			nID = MessageBox(hWnd, "終了しますか？", "本当に？", MB_YESNOCANCEL | MB_ICONASTERISK);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}

			else
			{
				return 0;
			}
		}

		break;

	default:

		break;

	case WM_CLOSE://×ボタンで確認表示
		nID = MessageBox(NULL, "終了しますか？", "本当に？", MB_YESNOCANCEL | MB_ICONASTERISK);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);//YESだったら閉じる
		}

		else
		{
			return 0;//それ以外はウィンドウは閉じない
		}

		break;
	}

	switch (wParam)
	{

	case WM_KEYDOWN://キー押したときのメッセージ

	case VK_UP:
		//無効領域を強制的に発生させる
		InvalidateRect(hWnd, &rect, FALSE);
		break;

	case VK_RIGHT:
		//nPosy++;
		//無効領域を強制的に発生させる
		InvalidateRect(hWnd, &rect, FALSE);
		break;

	case VK_ESCAPE://エスケープキーで表示
		DestroyWindow(hWnd);
		break;

		//case VK_SPACE://スペースキーで表示
		DestroyWindow(hWnd);
		break;

	default:

		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//==========
//初期化処理
//==========
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデヴァイスの生成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{

		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{

			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小時に補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大時に補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U値の繰り返し
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V値の繰り返し

																			//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // α値の合成
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//FPS文字の大きさ(18)
	D3DXCreateFont(g_pD3DDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//キーボード初期化処理
	InitKeyboard(hInstance, hWnd);

	// パッド入力の初期化処理
	InitGamepad(hInstance, hWnd);

	//フェイド処理
	InitFade(g_mode);

	InitSound(hWnd);

	//モードの設定
	SetMode(g_mode);

	return S_OK;
}

//-------------------
//モードの設定
//-------------------
void SetMode(MODE mode)
{
	switch (g_mode)
	{
	case MODE_TITLE:

		//タイトル画面の終了処理
		UninitTitle();
		break;

	case MODE_GAME:

		//ゲーム画面の終了処理
		UninitGame();
		break;

	case MODE_RESULT:

		//ゲーム画面の終了処理
		UninitResult();
		break;

	case MODE_RANKING:
		// ランキング画面の終了処理
		UninitRanking();
		break;

	case MODE_TUTORIAL:
		// ランキング画面の終了処理
		UninitTutorial();
		break;

	default:
		break;
	}

	switch (mode)
	{
	case MODE_TITLE:

		//タイトル画面の初期化処理
		InitTitle();
		StopSound(SOUND_LABEL_BGM004
		);
		PlaySound(SOUND_LABEL_BGM003);
		break;

	case MODE_GAME:

		//ゲーム画面の初期化処理
		InitGame();
		//音楽を入れる
		StopSound(SOUND_LABEL_BGM003);
		PlaySound(SOUND_LABEL_BGM004);
		break;

	case MODE_RESULT:

		//ゲーム画面の初期化処理
		InitResult();
		break;

	case MODE_RANKING:
		// ランキング画面の初期化処理
		InitRanking();
		break;

	case MODE_TUTORIAL:
		// ランキング画面の初期化処理
		InitTutorial();
		break;

	default:
		break;
	}
	g_mode = mode;
}

//========
//終了処理
//========
void Uninit(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	UninitGamepad();

	//UninitPlayer();

	UninitMeshfield();

	UninitWall();

	UninitBillboard();

	UninitBullet();

	UninitShadow();

	UninitPlayer();

	UninitObject();

	UninitCamera();

	Uninitlight();

	UninitEffect();

	UninitSound();

	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Directオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//=========
//更新処理
//=========
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	UpdateGamepad();

	switch (g_mode)
	{
	case MODE_TITLE:

		//タイトル画面の更新処理
		UpdateTitle();
		break;

	case MODE_GAME:

		//ゲーム画面の更新処理
		UpdatetGame();
		break;

	case MODE_RESULT:

		//ゲーム画面の更新処理
		UpdateResult();
		break;

	case MODE_RANKING:
		// ランキング画面の更新処理
		UpdateRanking();
		break;

	case MODE_TUTORIAL:
		// ランキング画面の更新処理
		UpdateTutorial();
		break;

	default:
		break;
	}

	//フェイド処理
	UpdateFade();

	CAMERA  *pCamera;
	pCamera = GetCamera();

	CAR  *pCar;
	pCar = GetCar();

	////コントローラーの設定
	//XINPUT_STATE state;
	//ZeroMemory(&state, sizeof(XINPUT_STATE));

	//// Simply get the state of the controller from XInput.
	//DWORD dwResult = XInputGetState(0, &state);

	//if (dwResult == ERROR_SUCCESS)
	//{
	//	// Controller is connected
	//}
	//else
	//{
	//	// Controller is not connected
	//}

	//if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	//{
	//	// A ボタンが押された
	//	//移動
	//	//pCar->move.x += sinf(pCamera->rot.y) * 0.2;
	//	//pCar->move.x += sinf(pCamera->rot.y) * 0.2;
	//}

	//if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	//{
	//	// スティックの左が押された

	//
	//}
}

//=========
//描画処理
//=========
void Draw(void)
{
	//ローカル
	//LPDIRECT3DDEVICE9 pDevice;

	//バックバッファ&Zバッファのクリア（画面のクリア）
	//背景の色
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(137, 189, 222, 0),
		1.0f,
		0);

	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		D3DVIEWPORT9 viewportDef;

		//
		g_pD3DDevice->GetViewport(&viewportDef);

		switch (g_mode)
		{
		case MODE_TITLE:

			//タイトル画面の描画処理
			DrawTitle();
			break;

		case MODE_GAME:

			//ゲーム画面の描画処理
			DrawGame();
			break;

		case MODE_RESULT:

			//ゲーム画面の描画処理
			DrawResult();
			break;

		case MODE_RANKING:
			// ランキング画面の描画処理
			DrawRanking();
			break;

		case MODE_TUTORIAL:
			// ランキング画面の描画処理
			DrawTutorial();
			break;
		}

		//フェイド処理
		DrawFade();

		//#ifdef _DEBUG
		DrawFPS();
		//#endif // DEBUG

		//
		g_pD3DDevice->SetViewport(&viewportDef);

		//描画の終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-------------------------
//FPS表示に必要
//-------------------------
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	CAMERA  *pCamera;
	pCamera = GetCamera();

	MeshField *pMeshfild;
	pMeshfild = GetMesh();

	PLAYER *pModel;
	pModel = GetModel();

	CAR  *pCar;
	pCar = GetCar();

	char aStr[512];

	g_Itemdate = GetItem();

	int nNum = wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
	nNum += sprintf(&aStr[nNum], "カメラ pos:  X%.2f Y%.2f  Z%.2f\n\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	nNum += sprintf(&aStr[nNum], "CAR    pos:  X%.2f Y%.2f  Z%.2f\n\n", pCar->pos.x, pCar->pos.y, pCar->pos.z);
	nNum += sprintf(&aStr[nNum], "MODEL  pos:  X%.2f Y%.2f  Z%.2f\n\n\n", pModel->pos.x, pModel->pos.y, pModel->pos.z);
	
	nNum += sprintf(&aStr[nNum], "0     :  カメラの角度リセット\n\n");
	
	nNum += sprintf(&aStr[nNum], "-----キャラクターの切り替え-----\n");
	nNum += sprintf(&aStr[nNum], "1     :  CAR\n");
	nNum += sprintf(&aStr[nNum], "2     :  MODEL\n\n");

	nNum += sprintf(&aStr[nNum], "-----MODEL-----\n");
	nNum += sprintf(&aStr[nNum], "6     :  スペードUP\n");
	nNum += sprintf(&aStr[nNum], "7     :  スペードDOWN\n");
	nNum += sprintf(&aStr[nNum], "8     :  視点の高さUP\n");
	nNum += sprintf(&aStr[nNum], "9     :  視点の高さDOWN\n");
	nNum += sprintf(&aStr[nNum], "G     :  オブジェクト生成\n");
	nNum += sprintf(&aStr[nNum], "H     :  オブジェクト消去\n");
	nNum += sprintf(&aStr[nNum], "↑↓  :  オブジェクトタイプ変更\n");
	//nNum += sprintf(&aStr[nNum], "TYPE  :  %.0f\n\n" , g_Itemdate);

	//テキスト
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


