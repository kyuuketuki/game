//-----------------------------------------------------------------
//
// 爆破の処理 (explosion.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#pragma once
#include "game.h"

//-----------------------------------------------------------------------------
// マクロ定義
//---------------------------------------------------------------------------		
#define EXPLOSION_SIZE (50)			//爆発の大きさ
#define MAX_EXPLOSION (256)			//爆発の上限

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos);
void SetVertexExplosion(int nIdx);


#endif _EXPLOSION_H_

