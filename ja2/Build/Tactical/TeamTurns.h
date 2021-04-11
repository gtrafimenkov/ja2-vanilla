#ifndef TEAMTURNS_H
#define TEAMTURNS_H

#include "Tactical/SoldierControl.h"

extern UINT8 gubOutOfTurnPersons;
extern BOOLEAN gfHiddenInterrupt;
extern BOOLEAN gfHiddenTurnbased;

#define INTERRUPT_QUEUED (gubOutOfTurnPersons > 0)

extern BOOLEAN StandardInterruptConditionsMet(SOLDIERCLASS* pSoldier, UINT8 ubOpponentID,
                                              INT8 bOldOppList);
extern INT8 CalcInterruptDuelPts(SOLDIERCLASS* pSoldier, UINT8 ubOpponentID,
                                 BOOLEAN fUseWatchSpots);
extern void EndAITurn(void);
extern void DisplayHiddenInterrupt(SOLDIERCLASS* pSoldier);
extern BOOLEAN InterruptDuel(SOLDIERCLASS* pSoldier, SOLDIERCLASS* pOpponent);
extern void AddToIntList(UINT8 ubID, BOOLEAN fGainControl, BOOLEAN fCommunicate);
extern void DoneAddingToIntList(SOLDIERCLASS* pSoldier, BOOLEAN fChange, UINT8 ubInterruptType);

void ClearIntList(void);

BOOLEAN SaveTeamTurnsToTheSaveGameFile(HWFILE hFile);

BOOLEAN LoadTeamTurnsFromTheSavedGameFile(HWFILE hFile);

void EndAllAITurns(void);

BOOLEAN NPCFirstDraw(SOLDIERCLASS* pSoldier, SOLDIERCLASS* pTargetSoldier);

#endif
