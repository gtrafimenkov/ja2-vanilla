#ifndef __SOLDIER_TILE_H
#define __SOLDIER_TILE_H

#include "Tactical/SoldierControl.h"

#define MOVE_TILE_CLEAR 1
#define MOVE_TILE_TEMP_BLOCKED -1
#define MOVE_TILE_STATIONARY_BLOCKED -2

INT8 TileIsClear(SOLDIERCLASS *pSoldier, INT8 bDirection, INT16 sGridNo, INT8 bLevel);

void MarkMovementReserved(SOLDIERCLASS *pSoldier, INT16 sGridNo);

void UnMarkMovementReserved(SOLDIERCLASS *pSoldier);

BOOLEAN HandleNextTile(SOLDIERCLASS *pSoldier, INT8 bDirection, INT16 sGridNo,
                       INT16 sFinalDestTile);

BOOLEAN HandleNextTileWaiting(SOLDIERCLASS *pSoldier);

BOOLEAN TeleportSoldier(SOLDIERCLASS *pSoldier, INT16 sGridNo, BOOLEAN fForce);

void SwapMercPositions(SOLDIERCLASS *pSoldier1, SOLDIERCLASS *pSoldier2);

void SetDelayedTileWaiting(SOLDIERCLASS *pSoldier, INT16 sCauseGridNo, INT8 bValue);

BOOLEAN CanExchangePlaces(SOLDIERCLASS *pSoldier1, SOLDIERCLASS *pSoldier2, BOOLEAN fShow);

#endif
