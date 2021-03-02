//-----------------------------------------------------------------
//
// プレイヤーの処理 (player.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"


//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_SHADOW	(128)	// 影の最大数

//-----------------------------------------------------------------------------
//構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	float fWidth;			// 横幅
	float fDepth;			// 奥行き
	D3DXMATRIX mtxWorld;					//ワールドマトリックス
	bool bUse;
}SHADOW;

//===============
//プロトタイプ宣言
//===============
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float  fWidth, float fDepth);
//int SetShadow(D3DXVECTOR3 pos);
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos);
//void SetPositionShadow(int nIdx);
#endif

