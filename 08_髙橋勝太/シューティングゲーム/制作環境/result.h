//-----------------------------------------------------------------
//
// リザルト画面 (result.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "input.h"

//-----------------------------------------------------------------------------
// リザルト画面の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	int nType;//種類
	int nData;
}RESULT;

//===============
//プロトタイプ宣言
//===============
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetVertexTextureResult(int nType);
//void SetResult(int nData);
#endif
