//-----------------------------------------------------------------
//
// 動く背景の処理 (movebg.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#ifndef _MOVEBG_H_
#define _MOVEBG_H_

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
HRESULT InitMovebg(void);
void UninitMovebg(void);
void UpdateMovebg(void);
void DrawMovebg(void);
void SetVertexTextureMovebg(int nType);
#endif
