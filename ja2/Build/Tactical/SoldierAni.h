#ifndef __SOLDIER_ANI_H
#define __SOLDIER_ANI_H

BOOLEAN AdjustToNextAnimationFrame(SOLDIERTYPE *pSoldier);

BOOLEAN CheckForAndHandleSoldierDeath(SOLDIERTYPE *pSoldier, BOOLEAN *pfMadeCorpse);

BOOLEAN CheckForAndHandleSoldierDyingNotFromHit(SOLDIERTYPE *pSoldier);

BOOLEAN HandleSoldierDeath(SOLDIERTYPE *pSoldier, BOOLEAN *pfMadeCorpse);

BOOLEAN OKFallDirection(SOLDIERTYPE *pSoldier, INT16 sGridNo, INT8 bLevel, INT8 bTestDirection,
                        UINT16 usAnimState);

BOOLEAN HandleCheckForDeathCommonCode(SOLDIERTYPE *pSoldier);

void KickOutWheelchair(SOLDIERTYPE *pSoldier);

#endif
