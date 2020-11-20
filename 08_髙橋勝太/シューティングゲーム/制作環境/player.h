//-----------------------------------------------------------------
//
// プレイヤーの処理 (player.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _PLYER_H_
#define _PLYER_H_

#include "game.h"
#include "input.h"
#include "enemy.h"
#include "bullet.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MOVE_NUMBER (2)

typedef struct
{
	D3DXVECTOR3 Pos;//位置
	bool bUse;//使用しているかどうか
	int nLife;//体力
}PLAYER;

//===============
//プロトタイプ宣言
//===============
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
#endif

