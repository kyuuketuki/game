//==============================================================================
//
// 入力処理（キーボード）〔input.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "input.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define KEY_STATE	(256)	// 入力情報

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECTINPUT8 g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		// 入力デバイスへのポインタ
BYTE g_aKeyState[KEY_STATE];					// キーボードの入力情報
BYTE g_aKeyStateTrigger[KEY_STATE];				// キートリガー情報
BYTE g_aKeyStateRelease[KEY_STATE];				// キーリリース情報
BYTE g_aKeyStateRepeat[KEY_STATE];				// キーリピート情報
int g_aCountRepeat[KEY_STATE];					// キーリピートカウンター
int g_aKeyInfo[] =
{
	DIK_RETURN,
	DIK_X,
	DIK_W,
	DIK_S,
	DIK_A,
	DIK_D,
	DIK_T,
	DIK_B,
	DIK_Z,
	DIK_C,
	DIK_Y,
	DIK_N,
	DIK_I,
	DIK_K,
	DIK_LSHIFT,
	DIK_RSHIFT,
	DIK_SPACE,
	DIK_U,
	DIK_M,
	DIK_Q,
	DIK_E,
	DIK_P,
	DIK_1,
	DIK_2
};

//==============================================================================
// 入力の初期化処理
//==============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// アクセス権の取得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//==============================================================================
// 入力の終了処理
//==============================================================================
void UninitKeyboard(void)
{
	if (g_pDevKeyboard != NULL)
	{
		// アクセス権の破棄
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//==============================================================================
// 入力の更新処理
//==============================================================================
void UpdateKeyboard(void)
{
	// ローカル変数宣言
	BYTE aKeyState[KEY_STATE];

	// デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (int nKey = 0; nKey < KEY_STATE; nKey++)
		{
			g_aKeyStateTrigger[nKey] = (g_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];
			g_aKeyStateRelease[nKey] = (g_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];
			g_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		// アクセス権の取得
		g_pDevKeyboard->Acquire();
	}
}

//-----------------------------------------------------------------------------
//キーボード情報の取得 プレス
//-----------------------------------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
//キーボード情報の取得 トリガー
//-----------------------------------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
//キーボード情報の取得 リリース
//-----------------------------------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

////==============================================================================
//// キーボード情報の取得
////==============================================================================
//bool GetKeyboardPress(int nKey)
//{
//	// キーボード情報の取得
//	return (g_aKeyState[g_aKeyInfo[nKey]] & 0x80) ? true : false;
//	//return (g_aKeyState[nKey] & 0x80) ? true : false;
//}
//
////==============================================================================
//// キーボード情報トリガーの取得
////==============================================================================
//bool GetKeyboardTrigger(int nKey)
//{
//	// キーボード情報の取得
//	return (g_aKeyStateTrigger[g_aKeyInfo[nKey]] & 0x80) ? true : false;
//}
//
////==============================================================================
//// キーボード情報リリースの取得
////==============================================================================
//bool GetKeyboardRelease(int nKey)
//{
//	// キーボード情報の取得
//	return (g_aKeyStateRelease[g_aKeyInfo[nKey]] & 0x80) ? true : false;
//}