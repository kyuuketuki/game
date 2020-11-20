//-----------------------------------------------------------------
//
// ゲーム内容の処理 (game.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//----------------------
//プロトタイプ宣言
//----------------------
HRESULT InitGame();
void UninitGame(void);
void UpdatetGame(void);
void DrawGame(void);
//bool GetKeyboardRepeat(int nKey);

#endif
