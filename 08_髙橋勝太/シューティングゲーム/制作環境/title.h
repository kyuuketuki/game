//-----------------------------------------------------------------
//
// タイトルの処理 (title.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "input.h"


typedef struct
{
	D3DXVECTOR3 Pos;//位置
	int nType;//種類
	bool bUse;
	int nCounterAnim;						//アニメーションカウンター
	int nPatternAnim;						//アニメーションパターンNo.
	int nseetAni;
}TITLE;

//===============
//プロトタイプ宣言
//===============
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetVertexTexture( int nType);
#endif
