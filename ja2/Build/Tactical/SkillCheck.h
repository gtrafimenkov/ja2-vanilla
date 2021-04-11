#ifndef __SKILL_CHECK_H
#define __SKILL_CHECK_H

#include "SGP/Types.h"
#include "Tactical/SoldierControl.h"

extern void ReducePointsForFatigue(SOLDIERCLASS *pSoldier, UINT16 *pusPoints);
extern INT32 GetSkillCheckPenaltyForFatigue(SOLDIERCLASS *pSoldier, INT32 iSkill);
extern INT32 SkillCheck(SOLDIERCLASS *pSoldier, INT8 bReason, INT8 bDifficulty);
extern INT8 CalcTrapDetectLevel(SOLDIERCLASS *pSoldier, BOOLEAN fExamining);

extern INT8 EffectiveStrength(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveWisdom(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveAgility(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveMechanical(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveExplosive(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveLeadership(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveMarksmanship(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveDexterity(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveExpLevel(SOLDIERCLASS *pSoldier);
extern INT8 EffectiveMedical(SOLDIERCLASS *pSoldier);

extern enum {
  NO_CHECK = 0,
  LOCKPICKING_CHECK,
  ELECTRONIC_LOCKPICKING_CHECK,
  ATTACHING_DETONATOR_CHECK,
  ATTACHING_REMOTE_DETONATOR_CHECK,
  PLANTING_BOMB_CHECK,
  PLANTING_REMOTE_BOMB_CHECK,
  OPEN_WITH_CROWBAR,
  SMASH_DOOR_CHECK,
  DISARM_TRAP_CHECK,
  UNJAM_GUN_CHECK,
  NOTICE_DART_CHECK,
  LIE_TO_QUEEN_CHECK,
  ATTACHING_SPECIAL_ITEM_CHECK,
  ATTACHING_SPECIAL_ELECTRONIC_ITEM_CHECK,
  DISARM_ELECTRONIC_TRAP_CHECK,
} SkillChecks;

#endif
