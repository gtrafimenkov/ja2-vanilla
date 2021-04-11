#ifndef __SOLDIER_ANI_H
#define __SOLDIER_ANI_H

BOOLEAN AdjustToNextAnimationFrame(SOLDIERCLASS *pSoldier);

BOOLEAN CheckForAndHandleSoldierDeath(SOLDIERCLASS *pSoldier, BOOLEAN *pfMadeCorpse);

BOOLEAN CheckForAndHandleSoldierDyingNotFromHit(SOLDIERCLASS *pSoldier);

BOOLEAN HandleSoldierDeath(SOLDIERCLASS *pSoldier, BOOLEAN *pfMadeCorpse);

BOOLEAN OKFallDirection(SOLDIERCLASS *pSoldier, INT16 sGridNo, INT8 bLevel, INT8 bTestDirection,
                        UINT16 usAnimState);

BOOLEAN HandleCheckForDeathCommonCode(SOLDIERCLASS *pSoldier);

void KickOutWheelchair(SOLDIERCLASS *pSoldier);

#endif
