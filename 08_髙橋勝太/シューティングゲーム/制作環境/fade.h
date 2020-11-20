//-----------------------------------------------------------------
//
// フェイドの処理 (fade.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

//===============
//プロトタイプ宣言
//===============
HRESULT InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(FADE fade, MODE modeNext);
//void SetFade2(FADE fade, MODE modeNext,int nData);
FADE GetFade(void);
#endif
