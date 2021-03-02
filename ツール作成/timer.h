#pragma once

//-----------------------------------------------------------------
//
// スコアの処理 (score.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "game.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define  TIMER_SIZE (25)
#define  MAX_TIMER (4)

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	bool bUse;
}TIMER;

//===============
//プロトタイプ宣言
//===============
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void AddTimer(int nValue);
void SetTimer(D3DXVECTOR3 pos, int nType);
void SetVertexTimer(int nIdx);
int GetTimer(void);

#endif
