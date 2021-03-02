//==============================================================================
//
// 入力処理（ゲームパッド）〔gamepad.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
#include "main.h"

//==============================================================================
// キー入力
//==============================================================================
// デバイスの列挙
typedef enum
{
	DIDEVTYPE_MOUSE = 0,	// マウス
	DIDEVTYPE_KEYBOARD,		// キーボード
	DIDEVTYPE_JOYSTICK,		// ジョイスティック
	DIDEVTYPE_DEVICE,		// その他
	DIDEVTYPE_MAX
} DIDEVTYPE;

typedef enum
{
	ButtonStateNone = 0,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
} ButtonState;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd);	// 入力の初期化処理
void UninitGamepad(void);								// 入力の終了処理
void UpdateGamepad(void);								// 入力の更新処理
BOOL CALLBACK DeviceFindCallBack(
	LPCDIDEVICEINSTANCE ipddi,
	LPVOID pvRef);
bool IsButtonPush(KEYINFO button);
bool IsButtonUp(KEYINFO button);
bool IsButtonDown(KEYINFO button);

#endif
