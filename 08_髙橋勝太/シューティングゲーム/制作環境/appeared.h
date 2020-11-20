//-----------------------------------------------------------------
//
// 爆破の処理 (explosion.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#pragma once
#ifndef _APPEARED_H_
#define _APPEARED_H_

#pragma once
#include "game.h"

//-----------------------------------------------------------------------------
// マクロ定義
//---------------------------------------------------------------------------		
#define APPEARED_SIZE (0)			//爆発の大きさ
#define MAX_APPEARED (256)			//爆発の上限

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitAppeared(void);
void UninitAppeared(void);
void UpdateAppeared(void);
void DrawAppeared(void);
void SetAppeared(D3DXVECTOR3 pos, int size, int nCut);
void SetVertexAppeared(int nIdx);


#endif _EXPLOSION_H_

#pragma once
