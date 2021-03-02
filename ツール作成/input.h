//-----------------------------------------------------------------
//
// ボタンの処理 (input.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==============================================================================
// キー入力
//==============================================================================
typedef enum
{
	KEYINFO_B = 0,
	KEYINFO_A,
	KEYINFO_UP,
	KEYINFO_DOWN,
	KEYINFO_LEFT,
	KEYINFO_RIGHT,
	KEYINFO_UP_CENTER,
	KEYINFO_DOWN_CENTER,
	KEYINFO_LEFT_TURN,
	KEYINFO_RIGHT_TURN,
	KEYINFO_UP_SIGHT,
	KEYINFO_DOWN_SIGHT,
	KEYINFO_UP_MODEL,
	KEYINFO_DOWN_MODEL,
	KEYINFO_TURN_LEFT_MODEL,
	KEYINFO_TURN_RIGHT_MODEL,
	KEYINFO_SHOT,
	KEYINFO_ZOOMIN,
	KEYINFO_ZOOMOUT,
	KEYINFO_FRONT_PLUS,
	KEYINFO_FRONT_MINUS,
	KEYINFO_PAUSE,
	KEYINFO_PRESET,
	KEYINFO_CRESET,
	KEYINFO_R1,
	KEYINFO_L1,
	KEYINFO_MAX
} KEYINFO;

//----------------------
//プロトタイプ宣言
//----------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
int GetPress(int nKey);
//bool GetKeyboardRepeat(int nKey);

#endif