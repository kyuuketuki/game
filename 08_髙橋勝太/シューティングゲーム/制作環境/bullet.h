//-----------------------------------------------------------------
//
// 弾の処理 (ｂullet.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _BULLET_H_
#define _BULLET_H_

#pragma once
#include "tutorial.h"
#include "game.h"
#include "enemy.h"
#include "Explosion.h"

//-----------------------------------------------------------------------------
// マクロ定義
//---------------------------------------------------------------------------
#define MOVE_BULLET (5)				//弾の速度
#define BULLET_SIZE (10)			//弾の大きさ
#define MAX_BULLET (1024)			//弾の上限

typedef enum
{
	BULLET_PLAYR = 0,
	BULLET_ENEMY,
	BULLET_BOSS,
	BULLET_MAX
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nLife;			//寿命
	bool bUse;			//弾の使用状況
	int nType;
}BULLET;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife,int nType);
void SetVertexBullet(int nIdx);
BULLET *GetBullet(void);
#endif _BULLET_H_