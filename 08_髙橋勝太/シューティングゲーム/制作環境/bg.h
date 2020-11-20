//-----------------------------------------------------------------
//
// 背景の処理 (ｂｇ.ｈ)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _BG_H_
#define _BG_H_
#include "game.h"

//===============
//プロトタイプ宣言
//===============
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
#endif

