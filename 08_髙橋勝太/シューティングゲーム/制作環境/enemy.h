//-----------------------------------------------------------------
//
// 敵の処理 (enemy.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------


#ifndef _ENEMY_H_
#define _ENEMY_H_

#define MAX_ENEMY (256)//敵の最大数
#define MAX_TYPE_ENEMY (3)//敵の種類数
#define MAX_ENEMY_SIZE (30)

#include "main.h"
#include "input.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "Explosion.h"
#include "sound.h"
//----------------------------------
//敵の状態
//----------------------------------
typedef enum
{
	ENEMYSTATE_NOMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//
//敵の構造体
//
typedef struct
{
	D3DXVECTOR3 Pos;//位置
	int nType;//種類
	bool bUse;//使用しているかどうか
	int g_nCounterAni;
	int g_nPatternAni;
	int g_nData;
	int g_nP;
	int g_nseetAni;
	ENEMYSTATE state;//状態
	int nCounterState;//状態カウンター
	int nLife;//体力
}ENEMY;

//===============
//プロトタイプ宣言
//===============
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 Pos, int nType);
ENEMY *GetEnemy(void);
bool HitEnemy(int nIdx, int nDamage);
int GetEnemyNomber(void);
#endif
