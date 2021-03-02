#pragma once

//-----------------------------------------------------------------
//
// スコアの処理 (score.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#ifndef _GEAR_H_
#define _GEAR_H_

#include "main.h"
#include "game.h"

#define  GEAR_SIZE (70)

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	bool bUse;
}GEAR;

//===============
//プロトタイプ宣言
//===============
HRESULT InitGear(void);
void UninitGear(void);
void UpdateGear(void);
void DrawGear(void);
void AddGear(int nValue);
void SetGear(D3DXVECTOR3 pos, int nType);
void SetVertexGear(int nIdx);

GEAR*GetGear(void);

#endif
