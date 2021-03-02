//==============================================================================
//
// 入力処理（ゲームパッド）〔gamepad.cpp〕
// AUTHOR : TAKAHASHI
//
//==============================================================================
#include "input.h"
#include "gamepad.h"

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
typedef struct
{
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
}DeviceEnumParameter;

//==============================================================================
// グローバル変数
//==============================================================================
static LPDIRECTINPUT8 g_InputInterface;
static LPDIRECTINPUTDEVICE8 device;						// 入力デバイスへのポインタ
static ButtonState g_ButtonStates[KEYINFO::KEYINFO_MAX];

//==============================================================================
// パッド入力の初期化処理
//==============================================================================
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// IDirectInput8インターフェースの作成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_InputInterface,
		NULL)))
	{
		return E_FAIL;
	}

	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &device;

	if (FAILED(g_InputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (FAILED(g_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// 入力デバイスの生成
	if (FAILED(g_InputInterface->CreateDevice(
		GUID_Joystick,
		&device,
		NULL)))
	{
		return E_FAIL;
	}

	// 入力情報の初期化
	for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
	{
		g_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
	}

	// データフォーマットの設定
	if (FAILED(device->SetDataFormat(
		&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DPROPAXISMODE_RELにしたら相対値)

										// 軸モードを変更
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// 協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==============================================================================
// パッド入力の終了処理
//==============================================================================
void UninitGamepad(void)
{
	if (device != NULL)
	{
		// アクセス権の破棄
		device->Unacquire();

		device->Release();
		device = NULL;
	}

	if (g_InputInterface != NULL)
	{
		g_InputInterface->Release();
		g_InputInterface = NULL;
	}
}

//==============================================================================
// パッド入力の更新処理
//==============================================================================
void UpdateGamepad(void)
{
	if (device == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;

	HRESULT hr = device->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		if (FAILED(device->Acquire()))
		{
			for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
			{
				g_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
			}
			device->Poll();
		}
		return;
	}

	bool is_push[KEYINFO::KEYINFO_MAX];

	int unresponsive_range = 200;

	if (pad_data.lX < -unresponsive_range)
	{// 左に傾けた
		is_push[KEYINFO::KEYINFO_LEFT] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{// 右に傾けた
		is_push[KEYINFO::KEYINFO_RIGHT] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{// 上に傾けた
		is_push[KEYINFO::KEYINFO_UP] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{// 下に傾けた
		is_push[KEYINFO::KEYINFO_DOWN] = true;
	}

	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		// 8方向での制御
		switch (pad_data.rgdwPOV[0])
		{
			// 上
		case 0:
			is_push[KEYINFO::KEYINFO_UP] = true;
			break;
			// 右上
		case 4500:
			is_push[KEYINFO::KEYINFO_UP] = true;
			is_push[KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 右
		case 9000:
			is_push[KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 右下
		case 13500:
			is_push[KEYINFO::KEYINFO_DOWN] = true;
			is_push[KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 下
		case 18000:
			is_push[KEYINFO::KEYINFO_DOWN] = true;
			break;
			// 左下
		case 22500:
			is_push[KEYINFO::KEYINFO_DOWN] = true;
			is_push[KEYINFO::KEYINFO_LEFT] = true;
			break;
			// 左
		case 27000:
			is_push[KEYINFO::KEYINFO_LEFT] = true;
			break;
			// 左上
		case 31500:
			is_push[KEYINFO::KEYINFO_UP] = true;
			is_push[KEYINFO::KEYINFO_LEFT] = true;
			break;
		}
	}

	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		if (!(pad_data.rgbButtons[nCnt] & 0x80))
		{
			continue;
		}

		switch (nCnt)
		{
		case 2:
			is_push[KEYINFO::KEYINFO_A] = true;
			break;

		case 3:
			is_push[KEYINFO::KEYINFO_B] = true;
			break;
		
		case 4:
			is_push[KEYINFO::KEYINFO_L1] = true;
			break;

		case 5:
			is_push[KEYINFO::KEYINFO_R1] = true;
			break;

		case 11:
			is_push[KEYINFO::KEYINFO_PAUSE] = true;
			break;
		}
	}

	// 入力情報からボタンの状態を更新する
	for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
	{
		if (is_push[nCnt] == true)
		{
			if (g_ButtonStates[nCnt] == ButtonState::ButtonStateNone)
			{
				g_ButtonStates[nCnt] = ButtonState::ButtonStateDown;
			}
			else
			{
				g_ButtonStates[nCnt] = ButtonState::ButtonStatePush;
			}
		}
		else
		{
			if (g_ButtonStates[nCnt] == ButtonState::ButtonStatePush)
			{
				g_ButtonStates[nCnt] = ButtonState::ButtonStateUp;
			}
			else
			{
				g_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
			}
		}
	}
}

// デバイス列挙コールバック関数
BOOL CALLBACK DeviceFindCallBack(
	LPCDIDEVICEINSTANCE ipddi,
	LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了
	if (parameter->FindCount >= 2)
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

bool IsButtonPush(KEYINFO button)
{
	if (g_ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

bool IsButtonUp(KEYINFO button)
{
	if (g_ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

bool IsButtonDown(KEYINFO button)
{
	if (g_ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}