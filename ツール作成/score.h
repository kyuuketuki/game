
//-----------------------------------------------------------------
//
// スコアの処理 (score.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"
#include "game.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_SCORE	(3)			// 桁の最大数
#define  SCORE_SIZE (35)

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
	bool bUse;
}SCORE;

//===============
//プロトタイプ宣言
//===============
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SetScore(D3DXVECTOR3 pos, int nType);
void SetVertexScore(int nIdx);

SCORE*GetScore(void);

#endif
