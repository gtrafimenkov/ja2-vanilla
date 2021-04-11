#ifndef __ENDGAME_H
#define __ENDGAME_H

BOOLEAN DoesO3SectorStatueExistHere(INT16 sGridNo);
void ChangeO3SectorStatue(BOOLEAN fFromExplosion);

void HandleDeidrannaDeath(SOLDIERCLASS *pKillerSoldier, INT16 sGridNo, INT8 bLevel);
void BeginHandleDeidrannaDeath(SOLDIERCLASS *pKillerSoldier, INT16 sGridNo, INT8 bLevel);

void HandleDoneLastKilledQueenQuote();

void EndQueenDeathEndgameBeginEndCimenatic();
void EndQueenDeathEndgame();

void HandleQueenBitchDeath(SOLDIERCLASS *pKillerSoldier, INT16 sGridNo, INT8 bLevel);
void BeginHandleQueenBitchDeath(SOLDIERCLASS *pKillerSoldier, INT16 sGridNo, INT8 bLevel);

void HandleDoneLastEndGameQuote();

#endif
