//-----------------------------------------------------------------
//
// リザルト画面 (result.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "input.h"

//-----------------------------------------------------------------------------
// リザルト画面の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	int nType;//種類
	int nData;
}TUTORIAL;

//===============
//プロトタイプ宣言
//===============
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void SetVertexTextureTutorial(int nType);
//void SetResult(int nData);
#endif

