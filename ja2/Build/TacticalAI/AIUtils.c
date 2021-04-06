#include "TacticalAI/AIAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "TacticalAI/AI.h"
#include "Tactical/Weapons.h"
#include "Tactical/OppList.h"
#include "Tactical/Points.h"
#include "Tactical/PathAI.h"
#include "TileEngine/WorldMan.h"
#include "TacticalAI/AIInternals.h"
#include "Tactical/Items.h"
#include "SGP/WCheck.h"
#include "Utils/Message.h"
#include "Tactical/LOS.h"
#include "Strategic/Assignments.h"
#include "Tactical/SoldierFunctions.h"
#include "Tactical/Points.h"
#include "GameSettings.h"
#include "TileEngine/Buildings.h"
#endif

//
// CJC's DG->JA2 conversion notes
//
// Commented out:
//
// InWaterOrGas - gas stuff
// RoamingRange - point patrol stuff

UINT8 Urgency[NUM_STATUS_STATES][NUM_MORALE_STATES] = {
    {URGENCY_LOW, URGENCY_LOW, URGENCY_LOW, URGENCY_LOW, URGENCY_LOW},    // green
    {URGENCY_HIGH, URGENCY_MED, URGENCY_MED, URGENCY_LOW, URGENCY_LOW},   // yellow
    {URGENCY_HIGH, URGENCY_MED, URGENCY_MED, URGENCY_MED, URGENCY_MED},   // red
    {URGENCY_HIGH, URGENCY_HIGH, URGENCY_HIGH, URGENCY_MED, URGENCY_MED}  // black
};

UINT16 MovementMode[LAST_MOVEMENT_ACTION + 1][NUM_URGENCY_STATES] = {
    {WALKING, WALKING, WALKING},  // AI_ACTION_NONE

    {WALKING, WALKING, WALKING},  // AI_ACTION_RANDOM_PATROL
    {WALKING, RUNNING, RUNNING},  // AI_ACTION_SEEK_FRIEND
    {WALKING, RUNNING, RUNNING},  // AI_ACTION_SEEK_OPPONENT
    {RUNNING, RUNNING, RUNNING},  // AI_ACTION_TAKE_COVER
    {WALKING, RUNNING, RUNNING},  // AI_ACTION_GET_CLOSER

    {WALKING, WALKING, WALKING},   // AI_ACTION_POINT_PATROL,
    {WALKING, RUNNING, RUNNING},   // AI_ACTION_LEAVE_WATER_GAS,
    {WALKING, SWATTING, RUNNING},  // AI_ACTION_SEEK_NOISE,
    {RUNNING, RUNNING, RUNNING},   // AI_ACTION_ESCORTED_MOVE,
    {WALKING, RUNNING, RUNNING},   // AI_ACTION_RUN_AWAY,

    {RUNNING, RUNNING, RUNNING},   // AI_ACTION_KNIFE_MOVE
    {WALKING, WALKING, WALKING},   // AI_ACTION_APPROACH_MERC
    {RUNNING, RUNNING, RUNNING},   // AI_ACTION_TRACK
    {RUNNING, RUNNING, RUNNING},   // AI_ACTION_EAT
    {WALKING, RUNNING, SWATTING},  // AI_ACTION_PICKUP_ITEM

    {WALKING, WALKING, WALKING},  // AI_ACTION_SCHEDULE_MOVE
    {WALKING, WALKING, WALKING},  // AI_ACTION_WALK
    {RUNNING, RUNNING, RUNNING},  // AI_ACTION_MOVE_TO_CLIMB
};

//***06.07.2016*** вычисление координаты точки для флангового обхода
INT16 GetFlankPos(SOLDIERCLASS *pSoldier, INT16 sGridNo, BOOLEAN fClimb) {
  UINT8 ubTargetID;
  SOLDIERCLASS *pOpponent;
  UINT8 ubOppositeDirection, ubAttackDirection;
  POINT A, P, D;
  INT16 sTargetGridNo;

  if (pSoldier->bAttitude != BRAVESOLO && pSoldier->bAttitude != CUNNINGAID) return (NOWHERE);

  if (pSoldier->bOrders != SEEKENEMY) return (NOWHERE);

  if (InARoom(sGridNo, NULL)) return (NOWHERE);

  ubTargetID = WhoIsThere2(sGridNo, fClimb);

  if (ubTargetID == NOBODY) return (NOWHERE);

  pOpponent = MercPtrs[ubTargetID];

  if (!pOpponent || OPPONENT_UNCONSCIOUS(pOpponent) ||
      pOpponent->bTeam == PLAYER_TEAM && pOpponent->bCollapsed)
    return (NOWHERE);

  if (PythSpacesAway(pSoldier->sGridNo, sGridNo) > MaxDistanceVisible() + 10) return (NOWHERE);

  ubAttackDirection = (UINT8)GetDirectionToGridNoFromGridNo(pSoldier->sGridNo, pOpponent->sGridNo);
  ubOppositeDirection = gOppositeDirection[ubAttackDirection];

  if (!(ubOppositeDirection == pOpponent->bDirection ||
        ubOppositeDirection == gOneCCDirection[pOpponent->bDirection] ||
        ubOppositeDirection == gOneCDirection[pOpponent->bDirection]))
    return (NOWHERE);

  ConvertGridNoToXY(pOpponent->sGridNo, &A.x, &A.y);
  ConvertGridNoToXY(pSoldier->sGridNo, &P.x, &P.y);

  if (ubOppositeDirection == gOneCCDirection[pOpponent->bDirection] ||
      ubOppositeDirection == pOpponent->bDirection && Chance(50)) {
    //вращение влево на 90 градусов точки P (бот) относительно оси A (игрок)
    D.x = A.x + (P.y - A.y);
    D.y = A.y - (P.x - A.x);
  } else if (ubOppositeDirection == gOneCDirection[pOpponent->bDirection] ||
             ubOppositeDirection == pOpponent->bDirection) {
    //вращение вправо на 90 градусов
    D.x = A.x - (P.y - A.y);
    D.y = A.y + (P.x - A.x);
  }

  if (D.y > WORLD_COLS || D.x > WORLD_ROWS || D.y < 0 || D.x < 0) return (NOWHERE);

  sTargetGridNo = (INT16)(D.y * WORLD_COLS + D.x);

  if (InARoom(sTargetGridNo, NULL)) return (NOWHERE);

  if (!LegalNPCDestination(pSoldier, sTargetGridNo, ENSURE_PATH, NOWATER, 0)) return (NOWHERE);

  return (sTargetGridNo);
}

//***22.05.2016*** для триггеров на картах
void SetTeamActiveOrders(INT8 bTeam, INT16 sGridNo, BOOLEAN fLocal) {
  INT32 iLoop;
  SOLDIERCLASS *pSoldier;

  if (gTacticalStatus.Team[bTeam].bTeamActive) {
    iLoop = gTacticalStatus.Team[bTeam].bFirstID;
    for (pSoldier = MercPtrs[iLoop]; iLoop <= gTacticalStatus.Team[bTeam].bLastID;
         iLoop++, pSoldier++) {
      if (pSoldier->bActive && pSoldier->bInSector && pSoldier->bLife >= OKLIFE) {
        if (fLocal && PythSpacesAway(pSoldier->sGridNo, sGridNo) > 30) continue;

        pSoldier->bOrders = SEEKENEMY;

        switch (Random(4)) {
          case 0:
            pSoldier->bAttitude = BRAVESOLO;
            break;
          case 1:
            pSoldier->bAttitude = BRAVEAID;
            break;
          case 2:
            pSoldier->bAttitude = CUNNINGAID;
            break;
          case 3:
            pSoldier->bAttitude = AGGRESSIVE;
            break;
        }
      }
    }
  }
}

void SetTeamPassiveOrders(INT8 bTeam, INT16 sGridNo, BOOLEAN fLocal) {
  INT32 iLoop;
  SOLDIERCLASS *pSoldier;

  if (gTacticalStatus.Team[bTeam].bTeamActive) {
    iLoop = gTacticalStatus.Team[bTeam].bFirstID;
    for (pSoldier = MercPtrs[iLoop]; iLoop <= gTacticalStatus.Team[bTeam].bLastID;
         iLoop++, pSoldier++) {
      if (pSoldier->bActive && pSoldier->bInSector && pSoldier->bLife >= OKLIFE) {
        if (fLocal && PythSpacesAway(pSoldier->sGridNo, sGridNo) > 30) continue;

        pSoldier->bOrders = ONGUARD;
      }
    }
  }
}

//***28.09.2014***
void ChoiseAIBestGun(SOLDIERCLASS *pSoldier) {
  INT8 bLoop, bBestWeaponPos = NO_SLOT;
  UINT8 ubBestDeadliness = 0;

  for (bLoop = 0; bLoop < NUM_INV_SLOTS; bLoop++) {
    if ((Item[pSoldier->inv[bLoop].usItem].usItemClass & IC_GUN) &&
        !(pSoldier->inv[bLoop].fFlags & OBJECT_AI_UNUSABLE) &&
        (pSoldier->inv[bLoop].bStatus[0] >= USABLE) && pSoldier->inv[bLoop].ubGunShotsLeft > 0) {
      if (EXPLOSIVE_GUN(pSoldier->inv[bLoop].usItem)) {
        continue;
      }

      if (ubBestDeadliness < Weapon[pSoldier->inv[bLoop].usItem].ubDeadliness) {
        ubBestDeadliness = Weapon[pSoldier->inv[bLoop].usItem].ubDeadliness;
        bBestWeaponPos = bLoop;
      }
    }
  }

  if (bBestWeaponPos != NO_SLOT && bBestWeaponPos != HANDPOS) {
    RearrangePocket(pSoldier, HANDPOS, bBestWeaponPos, FOREVER);
  }
}

//***24.01.2013***
BOOLEAN CheckAINearPlayerSeeGridNo(SOLDIERCLASS *pSoldier) {
  INT16 sGridNo, cnt;

  for (cnt = NORTH; cnt < NUM_WORLD_DIRECTIONS; cnt++) {
    sGridNo = NewGridNo(pSoldier->sGridNo, DirectionInc(cnt));
    if (sGridNo != pSoldier->sGridNo) {
      if (gbPlayerSeeGridNo[sGridNo] > 0) return (TRUE);
    }
  }
  return (FALSE);
}

//***24.01.2013*** смена модели поведения осторожных на активную при кучковании у поля зрения игрока
/*void ChangeEnemyTeamAttitudeForAttack( INT8 bTeam )
{
        INT32	iLoop, iLoop2;
        SOLDIERCLASS *pSoldier, *pSoldier2;

        if( gTacticalStatus.Team[bTeam].bTeamActive )
        {
                iLoop = gTacticalStatus.Team[ bTeam ].bFirstID;
                for( pSoldier = MercPtrs[iLoop]; iLoop <= gTacticalStatus.Team[ bTeam ].bLastID;
iLoop++, pSoldier++ )
                {
                        if( pSoldier->bActive && pSoldier->bInSector && pSoldier->bLife >= OKLIFE
                                && (pSoldier->bAttitude == CUNNINGSOLO || pSoldier->bAttitude ==
CUNNINGAID )
                                && (gbPlayerSeeGridNo[ pSoldier->sGridNo ] > 0 ||
CheckAINearPlayerSeeGridNo(pSoldier)) )
                        {
                                iLoop2 = gTacticalStatus.Team[ bTeam ].bFirstID;
                                for( pSoldier2 = MercPtrs[iLoop2]; iLoop2 <= gTacticalStatus.Team[
bTeam ].bLastID; iLoop2++, pSoldier2++ )
                                {
                                        if( iLoop != iLoop2 && pSoldier2->bActive &&
pSoldier2->bInSector && pSoldier2->bLife >= OKLIFE
                                                && (gbPlayerSeeGridNo[ pSoldier2->sGridNo ] > 0 ||
CheckAINearPlayerSeeGridNo(pSoldier2))
                                                && PythSpacesAway( pSoldier->sGridNo,
pSoldier2->sGridNo ) <= 5 )
                                        {
                                                switch( pSoldier->bAttitude )
                                                {
                                                        case CUNNINGSOLO:
                                                                pSoldier->bAttitude = BRAVESOLO;
                                                                break;
                                                        case CUNNINGAID:
                                                                pSoldier->bAttitude = BRAVEAID;
                                                                break;

                                                }

                                        }
                                }

                        }
                }

        }

}*/

//***12.08.2013*** смена модели поведения (осторожных) на Агрессивную при кучковании у поля зрения
//игрока
void ChangeEnemyTeamAttitudeForAttack(INT8 bTeam) {
  INT32 iLoop, iLoop2;
  SOLDIERCLASS *pSoldier, *pSoldier2;
  UINT8 ubFriends;

  if (gTacticalStatus.Team[bTeam].bTeamActive) {
    iLoop = gTacticalStatus.Team[bTeam].bFirstID;
    for (pSoldier = MercPtrs[iLoop]; iLoop <= gTacticalStatus.Team[bTeam].bLastID;
         iLoop++, pSoldier++) {
      if (pSoldier->bActive && pSoldier->bInSector && pSoldier->bLife >= OKLIFE &&
          (pSoldier->bAttitude == BRAVESOLO || pSoldier->bAttitude == CUNNINGAID) &&
          (gbPlayerSeeGridNo[pSoldier->sGridNo] > 0 || CheckAINearPlayerSeeGridNo(pSoldier))) {
        ubFriends = 0;

        iLoop2 = gTacticalStatus.Team[bTeam].bFirstID;
        for (pSoldier2 = MercPtrs[iLoop2]; iLoop2 <= gTacticalStatus.Team[bTeam].bLastID;
             iLoop2++, pSoldier2++) {
          if (iLoop != iLoop2 && pSoldier2->bActive && pSoldier2->bInSector &&
              pSoldier2->bLife >= OKLIFE && pSoldier2->bAlertStatus != STATUS_BLACK &&
              PythSpacesAway(pSoldier->sGridNo, pSoldier2->sGridNo) <= 7 &&
              (pSoldier2->bAttitude == BRAVESOLO || pSoldier2->bAttitude == CUNNINGAID ||
               (pSoldier2->bOrders == SEEKENEMY || pSoldier2->bOrders == ONCALL) &&
                   (pSoldier2->bAttitude == BRAVEAID ||
                    pSoldier2->bAttitude ==
                        AGGRESSIVE)))  //***09.02.2016*** присоединяемся ещё к такой команде
          {
            ubFriends++;
          }
        }

        if (ubFriends > 1) {
          pSoldier->bAttitude = AGGRESSIVE;

          iLoop2 = gTacticalStatus.Team[bTeam].bFirstID;
          for (pSoldier2 = MercPtrs[iLoop2]; iLoop2 <= gTacticalStatus.Team[bTeam].bLastID;
               iLoop2++, pSoldier2++) {
            if (iLoop != iLoop2 && pSoldier2->bActive && pSoldier2->bInSector &&
                pSoldier2->bLife >= OKLIFE && pSoldier2->bAlertStatus != STATUS_BLACK &&
                PythSpacesAway(pSoldier->sGridNo, pSoldier2->sGridNo) <= 7 &&
                (pSoldier2->bAttitude == BRAVESOLO || pSoldier2->bAttitude == CUNNINGAID)) {
              pSoldier2->bAttitude = AGGRESSIVE;
            }
          }
        }
      }
    }
  }
}

INT8 OKToAttack(SOLDIERCLASS *pSoldier, int target) {
  // can't shoot yourself
  if (target == pSoldier->sGridNo) return (NOSHOOT_MYSELF);

  if (WaterTooDeepForAttacks(pSoldier->sGridNo)) return (NOSHOOT_WATER);

  // make sure a weapon is in hand (FEB.8 ADDITION: tossable items are also OK)
  if (!WeaponInHand(pSoldier)) {
    return (NOSHOOT_NOWEAPON);
  }

  // JUST PUT THIS IN ON JULY 13 TO TRY AND FIX OUT-OF-AMMO SITUATIONS

  if (Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN) {
    if (pSoldier->inv[HANDPOS].usItem == TANK_CANNON) {
      // look for another tank shell ELSEWHERE IN INVENTORY
      if (FindLaunchable(pSoldier, TANK_CANNON) == NO_SLOT)
      // if ( !ItemHasAttachments( &(pSoldier->inv[HANDPOS]) ) )
      {
        return (NOSHOOT_NOLOAD);
      }
    } else if (pSoldier->inv[HANDPOS].ubGunShotsLeft == 0) {
      return (NOSHOOT_NOAMMO);
    }
  } else if (Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_LAUNCHER) {
    if (FindLaunchable(pSoldier, pSoldier->inv[HANDPOS].usItem) == NO_SLOT)
    // if ( !ItemHasAttachments( &(pSoldier->inv[HANDPOS]) ) )
    {
      return (NOSHOOT_NOLOAD);
    }
  }

  return (TRUE);
}

BOOLEAN ConsiderProne(SOLDIERCLASS *pSoldier) {
  INT16 sOpponentGridNo;
  INT8 bOpponentLevel;
  INT32 iRange;

  if (pSoldier->bAIMorale >= MORALE_NORMAL) {
    return (FALSE);
  }
  // We don't want to go prone if there is a nearby enemy
  ClosestKnownOpponent(pSoldier, &sOpponentGridNo, &bOpponentLevel);
  iRange = GetRangeFromGridNoDiff(pSoldier->sGridNo, sOpponentGridNo);
  if (iRange > 10) {
    return (TRUE);
  } else {
    return (FALSE);
  }
}

UINT8 StanceChange(SOLDIERCLASS *pSoldier, UINT8 ubAttackAPCost) {
  // consider crouching or going prone

  if (PTR_STANDING) {
    if (pSoldier->bActionPoints - ubAttackAPCost >= AP_CROUCH) {
      if ((pSoldier->bActionPoints - ubAttackAPCost >= AP_CROUCH + AP_PRONE) &&
          IsValidStance(pSoldier, ANIM_PRONE) && ConsiderProne(pSoldier)) {
        return (ANIM_PRONE);
      } else if (IsValidStance(pSoldier, ANIM_CROUCH)) {
        return (ANIM_CROUCH);
      }
    }
  } else if (PTR_CROUCHED) {
    if ((pSoldier->bActionPoints - ubAttackAPCost >= AP_PRONE) &&
        IsValidStance(pSoldier, ANIM_PRONE) && ConsiderProne(pSoldier)) {
      return (ANIM_PRONE);
    }
  }
  return (0);
}

UINT8 ShootingStanceChange(SOLDIERCLASS *pSoldier, ATTACKCLASS *pAttack, INT8 bDesiredDirection) {
  // Figure out the best stance for this attack

  // We don't want to go through a lot of complex calculations here,
  // just compare the chance of the bullet hitting if we are
  // standing, crouched, or prone

  UINT16 usRealAnimState, usBestAnimState;
  INT8 bBestStanceDiff = -1;
  INT8 bLoop, bStanceNum, bStanceDiff, bAPsAfterAttack;
  UINT32 uiChanceOfDamage, uiBestChanceOfDamage, uiCurrChanceOfDamage;
  UINT32 uiStanceBonus, uiMinimumStanceBonusPerChange = 20 - 3 * pAttack->ubAimTime;
  INT32 iRange;

  bStanceNum = 0;
  uiCurrChanceOfDamage = 0;

  bAPsAfterAttack = pSoldier->bActionPoints - pAttack->ubAPCost -
                    GetAPsToReadyWeapon(pSoldier, pSoldier->usAnimState);
  if (bAPsAfterAttack < AP_CROUCH) {
    return (0);
  }
  // Unfortunately, to get this to work, we have to fake the AI guy's
  // animation state so we get the right height values
  usRealAnimState = pSoldier->usAnimState;
  usBestAnimState = pSoldier->usAnimState;
  uiBestChanceOfDamage = 0;
  iRange = GetRangeInCellCoordsFromGridNoDiff(pSoldier->sGridNo, pAttack->sTarget);

  switch (gAnimControl[usRealAnimState].ubEndHeight) {
    // set a stance number comparable with our loop variable so we can easily compute
    // stance differences and thus AP cost
    case ANIM_STAND:
      bStanceNum = 0;
      break;
    case ANIM_CROUCH:
      bStanceNum = 1;
      break;
    case ANIM_PRONE:
      bStanceNum = 2;
      break;
  }
  for (bLoop = 0; bLoop < 3; bLoop++) {
    bStanceDiff = abs(bLoop - bStanceNum);
    if (bStanceDiff == 2 && bAPsAfterAttack < AP_CROUCH + AP_PRONE) {
      // can't consider this!
      continue;
    }

    switch (bLoop) {
      case 0:
        if (!InternalIsValidStance(pSoldier, bDesiredDirection, ANIM_STAND)) {
          continue;
        }
        pSoldier->usAnimState = STANDING;
        break;
      case 1:
        if (!InternalIsValidStance(pSoldier, bDesiredDirection, ANIM_CROUCH)) {
          continue;
        }
        pSoldier->usAnimState = CROUCHING;
        break;
      default:
        if (!InternalIsValidStance(pSoldier, bDesiredDirection, ANIM_PRONE)) {
          continue;
        }
        pSoldier->usAnimState = PRONE;
        break;
    }

    uiChanceOfDamage =
        SoldierToLocationChanceToGetThrough(pSoldier, pAttack->sTarget, pSoldier->bTargetLevel,
                                            pSoldier->bTargetCubeLevel, pAttack->ubOpponent) *
        CalcChanceToHitGun(pSoldier, pAttack->sTarget, pAttack->ubAimTime, AIM_SHOT_TORSO) / 100;
    if (uiChanceOfDamage > 0) {
      uiStanceBonus = 0;
      // artificially augment "chance of damage" to reflect penalty to be shot at various stances
      switch (pSoldier->usAnimState) {
        case CROUCHING:
          if (iRange > POINT_BLANK_RANGE + 10 * (AIM_PENALTY_TARGET_CROUCHED / 3)) {
            uiStanceBonus = AIM_BONUS_CROUCHING;
          } else if (iRange > POINT_BLANK_RANGE) {
            // reduce chance to hit with distance to the prone/immersed target
            uiStanceBonus = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE);  // penalty -3%/tile
          }
          break;
        case PRONE:
          if (iRange <= MIN_PRONE_RANGE) {
            // HATE being prone this close!
            uiChanceOfDamage = 0;
          } else  // if (iRange > POINT_BLANK_RANGE)
          {
            // reduce chance to hit with distance to the prone/immersed target
            uiStanceBonus = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE);  // penalty -3%/tile

            //***12.12.2012*** для пулемёта предпочтительное положение лёжа
            if (Weapon[pSoldier->inv[pAttack->bWeaponIn].usItem].ubWeaponType == GUN_LMG)
              uiStanceBonus *= 2;  ///
          }
          break;
        default:
          break;
      }
      // reduce stance bonus according to how much we have to change stance to get there
      // uiStanceBonus = uiStanceBonus * (4 - bStanceDiff) / 4;
      uiChanceOfDamage += uiStanceBonus;
    }

    if (bStanceDiff == 0) {
      uiCurrChanceOfDamage = uiChanceOfDamage;
    }
    if (uiChanceOfDamage > uiBestChanceOfDamage) {
      uiBestChanceOfDamage = uiChanceOfDamage;
      usBestAnimState = pSoldier->usAnimState;
      bBestStanceDiff = bStanceDiff;
    }
  }

  pSoldier->usAnimState = usRealAnimState;

  // return 0 or the best height value to be at
  if (bBestStanceDiff == 0 || ((uiBestChanceOfDamage - uiCurrChanceOfDamage) / bBestStanceDiff) <
                                  uiMinimumStanceBonusPerChange) {
    // better off not changing our stance!
    return (0);
  } else {
    return (gAnimControl[usBestAnimState].ubEndHeight);
  }
}

// DIGGLER ON
// Функция DetermineMovementMode - часть ИИ, возвращает режим перемещения для солдата при выполнении
// действия bAction
// Возвращаемые значения: из диапазона перечисления AnimationStates, как то:
/*	WALKING = 0,
        ..
        CROUCHING,
        ..
        RUNNING,
*/
// и т.д.

UINT16 DetermineMovementMode(SOLDIERCLASS *pSoldier, INT8 bAction) {
  if (pSoldier->fUIMovementFast) {
    return (RUNNING);
  } else if (CREATURE_OR_BLOODCAT(pSoldier)) {
    if (pSoldier->bAlertStatus == STATUS_GREEN) {
      return (WALKING);
    } else {
      return (RUNNING);
    }
  } else if (pSoldier->ubBodyType == COW || pSoldier->ubBodyType == CROW) {
    return (WALKING);
  } else {
    if ((pSoldier->fAIFlags & AI_CAUTIOUS) &&
        (MovementMode[bAction][Urgency[pSoldier->bAlertStatus][pSoldier->bAIMorale]] == RUNNING)) {
      //***09.02.2016***
      if (IS_MERC_BODY_TYPE(pSoldier))
        return (SWATTING);
      else  ///
        return (WALKING);
    } else if (bAction == AI_ACTION_SEEK_NOISE && pSoldier->bTeam == CIV_TEAM &&
               !IS_MERC_BODY_TYPE(pSoldier)) {
      return (WALKING);
    } else if ((pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV) &&
               (pSoldier->bAlertStatus == STATUS_GREEN) && Random(10) == 0) {
      return (KID_SKIPPING);
    } else {
      return (MovementMode[bAction][Urgency[pSoldier->bAlertStatus][pSoldier->bAIMorale]]);
    }
  }
}

void NewDest(SOLDIERCLASS *pSoldier, UINT16 usGridNo) {
  // ATE: Setting sDestination? Tis does not make sense...
  // pSoldier->sDestination = usGridNo;
  BOOLEAN fSet = FALSE;

  //***14.05.2016*** были перепутаны Orders и Attitude
  // if ( IS_MERC_BODY_TYPE( pSoldier ) && pSoldier->bAction == AI_ACTION_TAKE_COVER &&
  // (pSoldier->bOrders == DEFENSIVE || pSoldier->bOrders == CUNNINGSOLO || pSoldier->bOrders ==
  // CUNNINGAID ) && (SoldierDifficultyLevel( pSoldier ) >= 2) )
  if (IS_MERC_BODY_TYPE(pSoldier) && pSoldier->bAction == AI_ACTION_TAKE_COVER &&
      (pSoldier->bAttitude == DEFENSIVE || pSoldier->bAttitude == CUNNINGSOLO ||
       pSoldier->bAttitude == CUNNINGAID) &&
      (SoldierDifficultyLevel(pSoldier) >= 2)) {
    UINT16 usMovementMode;

    // getting real movement anim for someone who is going to take cover, not just considering
    usMovementMode =
        MovementMode[AI_ACTION_TAKE_COVER][Urgency[pSoldier->bAlertStatus][pSoldier->bAIMorale]];
    if (usMovementMode != SWATTING) {
      // really want to look at path, see how far we could get on path while swatting
      if (EnoughPoints(pSoldier, RecalculatePathCost(pSoldier, SWATTING), 0, FALSE) ||
          (pSoldier->bLastAction == AI_ACTION_TAKE_COVER &&
           pSoldier->usUIMovementMode == SWATTING)) {
        pSoldier->usUIMovementMode = SWATTING;
      } else {
        pSoldier->usUIMovementMode = usMovementMode;
      }
    } else {
      pSoldier->usUIMovementMode = usMovementMode;
    }
    fSet = TRUE;
  } else {
    if (pSoldier->bTeam == ENEMY_TEAM && pSoldier->bAlertStatus == STATUS_RED) {
      ///			switch( pSoldier->bAction )
      ///			{
      /*
      case AI_ACTION_MOVE_TO_CLIMB:
      case AI_ACTION_RUN_AWAY:
              pSoldier->usUIMovementMode = DetermineMovementMode( pSoldier, pSoldier->bAction );
              fSet = TRUE;
              break;*/
      ///				default:
      if (PreRandom(5 - SoldierDifficultyLevel(pSoldier)) == 0) {
        INT16 sClosestNoise = (INT16)MostImportantNoiseHeard(pSoldier, NULL, NULL, NULL);
        if (sClosestNoise != NOWHERE &&
            PythSpacesAway(pSoldier->sGridNo, sClosestNoise) < MaxDistanceVisible() + 10) {
          pSoldier->usUIMovementMode = SWATTING;
          fSet = TRUE;
        }
      }
      if (!fSet) {
        pSoldier->usUIMovementMode = DetermineMovementMode(pSoldier, pSoldier->bAction);
        fSet = TRUE;
      }
      ///					break;
      ///			}

    } else {
      pSoldier->usUIMovementMode = DetermineMovementMode(pSoldier, pSoldier->bAction);
      fSet = TRUE;
    }

    if (pSoldier->usUIMovementMode == SWATTING && !IS_MERC_BODY_TYPE(pSoldier)) {
      pSoldier->usUIMovementMode = WALKING;
    }
  }

  // EVENT_GetNewSoldierPath( pSoldier, pSoldier->sDestination, pSoldier->usUIMovementMode );
  // ATE: Using this more versitile version
  // Last paramater says whether to re-start the soldier's animation
  // This should be done if buddy was paused for fNoApstofinishMove...
  EVENT_InternalGetNewSoldierPath(pSoldier, usGridNo, pSoldier->usUIMovementMode, FALSE,
                                  pSoldier->fNoAPToFinishMove);
}

BOOLEAN IsActionAffordable(SOLDIERCLASS *pSoldier) {
  INT8 bMinPointsNeeded = 0;

  // NumMessage("AffordableAction - Guy#",pSoldier->ubID);

  switch (pSoldier->bAction) {
    case AI_ACTION_NONE:  // maintain current position & facing
      // no cost for doing nothing!
      break;

    case AI_ACTION_CHANGE_FACING:  // turn to face another direction
      bMinPointsNeeded = (INT8)GetAPsToLook(pSoldier);
      break;

    case AI_ACTION_RANDOM_PATROL:    // move towards a particular location
    case AI_ACTION_SEEK_FRIEND:      // move towards friend in trouble
    case AI_ACTION_SEEK_OPPONENT:    // move towards a reported opponent
    case AI_ACTION_TAKE_COVER:       // run for nearest cover from threat
    case AI_ACTION_GET_CLOSER:       // move closer to a strategic location
    case AI_ACTION_POINT_PATROL:     // move towards next patrol point
    case AI_ACTION_LEAVE_WATER_GAS:  // seek nearest spot of ungassed land
    case AI_ACTION_SEEK_NOISE:       // seek most important noise heard
    case AI_ACTION_ESCORTED_MOVE:    // go where told to by escortPlayer
    case AI_ACTION_RUN_AWAY:         // run away from nearby opponent(s)
    case AI_ACTION_APPROACH_MERC:
    case AI_ACTION_TRACK:
    case AI_ACTION_EAT:
    case AI_ACTION_SCHEDULE_MOVE:
    case AI_ACTION_WALK:
    case AI_ACTION_MOVE_TO_CLIMB:
      // for movement, must have enough APs to move at least 1 tile's worth
      bMinPointsNeeded = MinPtsToMove(pSoldier);
      break;

    case AI_ACTION_PICKUP_ITEM:  // grab things lying on the ground
      bMinPointsNeeded = __max(MinPtsToMove(pSoldier), AP_PICKUP_ITEM);
      break;

    case AI_ACTION_OPEN_OR_CLOSE_DOOR:
    case AI_ACTION_UNLOCK_DOOR:
    case AI_ACTION_LOCK_DOOR:
      bMinPointsNeeded = MinPtsToMove(pSoldier);
      break;

    case AI_ACTION_DROP_ITEM:
      bMinPointsNeeded = AP_PICKUP_ITEM;
      break;

    case AI_ACTION_FIRE_GUN:         // shoot at nearby opponent
    case AI_ACTION_TOSS_PROJECTILE:  // throw grenade at/near opponent(s)
    case AI_ACTION_KNIFE_MOVE:       // preparing to stab adjacent opponent
    case AI_ACTION_THROW_KNIFE:
      // only FIRE_GUN currently actually pays extra turning costs!
      bMinPointsNeeded = MinAPsToAttack(pSoldier, pSoldier->usActionData, ADDTURNCOST);

#ifdef BETAVERSION
      if (ptsNeeded > pSoldier->bActionPoints) {
        /*
                sprintf(tempstr,"AI ERROR: %s has insufficient points for attack action %d at grid
           %d", pSoldier->name,pSoldier->bAction,pSoldier->usActionData); PopMessage(tempstr);
                */
      }
#endif
      break;

    case AI_ACTION_PULL_TRIGGER:  // activate an adjacent panic trigger
      bMinPointsNeeded = AP_PULL_TRIGGER;
      break;

    case AI_ACTION_USE_DETONATOR:  // grab detonator and set off bomb(s)
      bMinPointsNeeded = AP_USE_REMOTE;
      break;

    case AI_ACTION_YELLOW_ALERT:   // tell friends opponent(s) heard
    case AI_ACTION_RED_ALERT:      // tell friends opponent(s) seen
    case AI_ACTION_CREATURE_CALL:  // for now
      bMinPointsNeeded = AP_RADIO;
      break;

    case AI_ACTION_CHANGE_STANCE:  // crouch
      bMinPointsNeeded = AP_CROUCH;
      break;

    case AI_ACTION_GIVE_AID:  // help injured/dying friend
      bMinPointsNeeded = 0;
      break;

    case AI_ACTION_CLIMB_ROOF:
      if (pSoldier->bLevel == 0) {
        bMinPointsNeeded = AP_CLIMBROOF;
      } else {
        bMinPointsNeeded = AP_CLIMBOFFROOF;
      }
      break;

    case AI_ACTION_COWER:
    case AI_ACTION_STOP_COWERING:
    case AI_ACTION_LOWER_GUN:
    case AI_ACTION_END_COWER_AND_MOVE:
    case AI_ACTION_TRAVERSE_DOWN:
    case AI_ACTION_OFFER_SURRENDER:
      bMinPointsNeeded = 0;
      break;

    default:
#ifdef BETAVERSION
      // NumMessage("AffordableAction - Illegal action type = ",pSoldier->bAction);
#endif
      break;
  }

  // check whether or not we can afford to do this action
  if (bMinPointsNeeded > pSoldier->bActionPoints) {
    return (FALSE);
  } else {
    return (TRUE);
  }
}

INT16 RandomFriendWithin(SOLDIERCLASS *pSoldier) {
  UINT32 uiLoop;
  UINT16 usMaxDist;
  UINT8 ubFriendCount, ubFriendIDs[MAXMERCS], ubFriendID;
  UINT16 usDirection;
  UINT8 ubDirsLeft;
  BOOLEAN fDirChecked[8];
  BOOLEAN fRangeRestricted = FALSE, fFound = FALSE;
  UINT16 usDest, usOrigin;
  SOLDIERCLASS *pFriend;

  // obtain maximum roaming distance from soldier's origin
  usMaxDist = RoamingRange(pSoldier, &usOrigin);

  // if our movement range is restricted

  // CJC: since RandomFriendWithin is only used in non-combat, ALWAYS restrict range.
  fRangeRestricted = TRUE;
  /*
  if (usMaxDist < MAX_ROAMING_RANGE)
  {
          fRangeRestricted = TRUE;
  }
  */

  // if range is restricted, make sure origin is a legal gridno!
  if (fRangeRestricted && ((usOrigin < 0) || (usOrigin >= GRIDSIZE))) {
#ifdef BETAVERSION
    NameMessage(pSoldier, "has illegal origin, but his roaming range is restricted!", 1000);
#endif
    return (FALSE);
  }

  ubFriendCount = 0;

  // build a list of the guynums of all active, eligible friendly mercs

  // go through each soldier, looking for "friends" (soldiers on same side)
  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pFriend = MercSlots[uiLoop];

    // if this merc is inactive, not in sector, or dead
    if (!pFriend) {
      continue;
    }

    // skip ourselves
    if (pFriend->ubID == pSoldier->ubID) {
      continue;
    }

    // if this man not neutral, but is on my side, OR if he is neutral, but
    // so am I, then he's a "friend" for the purposes of random visitations
    if ((!pFriend->bNeutral && (pSoldier->bSide == pFriend->bSide)) ||
        (pFriend->bNeutral && pSoldier->bNeutral)) {
      // if we're not already neighbors
      if (SpacesAway(pSoldier->sGridNo, pFriend->sGridNo) > 1) {
        // remember his guynum, increment friend counter
        ubFriendIDs[ubFriendCount++] = pFriend->ubID;
      }
    }
  }

  while (ubFriendCount && !fFound) {
    // randomly select one of the remaining friends in the list
    ubFriendID = ubFriendIDs[PreRandom(ubFriendCount)];

    // if our movement range is NOT restricted, or this friend's within range
    // use distance - 1, because there must be at least 1 tile 1 space closer
    if (!fRangeRestricted || (SpacesAway(usOrigin, Menptr[ubFriendID].sGridNo) - 1) <= usMaxDist) {
      // should be close enough, try to find a legal ->sDestination within 1 tile

      // clear dirChecked flag for all 8 directions
      for (usDirection = 0; usDirection < 8; usDirection++) {
        fDirChecked[usDirection] = FALSE;
      }

      ubDirsLeft = 8;

      // examine all 8 spots around 'ubFriendID'
      // keep looking while directions remain and a satisfactory one not found
      while ((ubDirsLeft--) && !fFound) {
        // randomly select a direction which hasn't been 'checked' yet
        do {
          usDirection = (UINT16)Random(8);
        } while (fDirChecked[usDirection]);

        fDirChecked[usDirection] = TRUE;

        // determine the gridno 1 tile away from current friend in this direction
        usDest = NewGridNo(Menptr[ubFriendID].sGridNo, DirectionInc((INT16)(usDirection + 1)));

        // if that's out of bounds, ignore it & check next direction
        if (usDest == Menptr[ubFriendID].sGridNo) {
          continue;
        }

        // if our movement range is NOT restricted
        if (!fRangeRestricted || (SpacesAway(usOrigin, usDest) <= usMaxDist)) {
          if (LegalNPCDestination(pSoldier, usDest, ENSURE_PATH, NOWATER, 0)) {
            fFound = TRUE;                    // found a spot
            pSoldier->usActionData = usDest;  // store this ->sDestination
            pSoldier->bPathStored = TRUE;     // optimization - Ian
            break;                            // stop checking in other directions
          }
        }
      }
    }

    if (!fFound) {
      ubFriendCount--;

      // if we hadn't already picked the last friend currently in the list
      if (ubFriendCount != ubFriendID) {
        ubFriendIDs[ubFriendID] = ubFriendIDs[ubFriendCount];
      }
    }
  }

  return (fFound);
}

INT16 RandDestWithinRange(SOLDIERCLASS *pSoldier) {
  INT16 sRandDest = NOWHERE;
  UINT16 usOrigin, usMaxDist;
  UINT8 ubTriesLeft;
  BOOLEAN fLimited = FALSE, fFound = FALSE;
  INT16 sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXRange, sYRange, sXOffset, sYOffset;
  INT16 sOrigX, sOrigY;
  INT16 sX, sY;
  UINT8 ubRoom = 0, ubTempRoom;

  sOrigX = sOrigY = -1;
  sMaxLeft = sMaxRight = sMaxUp = sMaxDown = sXRange = sYRange = -1;

  // Try to find a random ->sDestination that's no more than maxDist away from
  // the given gridno of origin

  if (gfTurnBasedAI) {
    ubTriesLeft = 10;
  } else {
    ubTriesLeft = 1;
  }

  usMaxDist = RoamingRange(pSoldier, &usOrigin);

  if (pSoldier->bOrders <= CLOSEPATROL &&
      (pSoldier->bTeam == CIV_TEAM || pSoldier->ubProfile != NO_PROFILE)) {
    // any other combo uses the default of ubRoom == 0, set above
    if (!InARoom(pSoldier->usPatrolGrid[0], &ubRoom)) {
      ubRoom = 0;
    }
  }

  // if the maxDist is truly a restriction
  if (usMaxDist < (MAXCOL - 1)) {
    fLimited = TRUE;

    // determine maximum horizontal limits
    sOrigX = usOrigin % MAXCOL;
    sOrigY = usOrigin / MAXCOL;

    sMaxLeft = min(usMaxDist, sOrigX);
    sMaxRight = min(usMaxDist, MAXCOL - (sOrigX + 1));

    // determine maximum vertical limits
    sMaxUp = min(usMaxDist, sOrigY);
    sMaxDown = min(usMaxDist, MAXROW - (sOrigY + 1));

    sXRange = sMaxLeft + sMaxRight + 1;
    sYRange = sMaxUp + sMaxDown + 1;
  }

  if (pSoldier->ubBodyType == LARVAE_MONSTER) {
    // only crawl 1 tile, within our roaming range
    while ((ubTriesLeft--) && !fFound) {
      sXOffset = (INT16)Random(3) - 1;  // generates -1 to +1
      sYOffset = (INT16)Random(3) - 1;

      if (fLimited) {
        sX = pSoldier->sGridNo % MAXCOL + sXOffset;
        sY = pSoldier->sGridNo / MAXCOL + sYOffset;
        if (sX < sOrigX - sMaxLeft || sX > sOrigX + sMaxRight) {
          continue;
        }
        if (sY < sOrigY - sMaxUp || sY > sOrigY + sMaxDown) {
          continue;
        }
        sRandDest = usOrigin + sXOffset + (MAXCOL * sYOffset);
      } else {
        sRandDest = usOrigin + sXOffset + (MAXCOL * sYOffset);
      }

      if (!LegalNPCDestination(pSoldier, sRandDest, ENSURE_PATH, NOWATER, 0)) {
        sRandDest = NOWHERE;
        continue;  // try again!
      }

      // passed all the tests, ->sDestination is acceptable
      fFound = TRUE;
      pSoldier->bPathStored = TRUE;  // optimization - Ian
    }
  } else {
    // keep rolling random ->sDestinations until one's satisfactory or retries used
    while ((ubTriesLeft--) && !fFound) {
      if (fLimited) {
        sXOffset = ((INT16)Random(sXRange)) - sMaxLeft;
        sYOffset = ((INT16)Random(sYRange)) - sMaxUp;

        sRandDest = usOrigin + sXOffset + (MAXCOL * sYOffset);

#ifdef BETAVERSION
        if ((sRandDest < 0) || (sRandDest >= GRIDSIZE)) {
          NumMessage("RandDestWithinRange: ERROR - Gridno out of range! = ", sRandDest);
          sRandDest = random(GRIDSIZE);
        }
#endif
      } else {
        sRandDest = (INT16)PreRandom(GRIDSIZE);
      }

      if (ubRoom && InARoom(sRandDest, &ubTempRoom) && ubTempRoom != ubRoom) {
        // outside of room available for patrol!
        sRandDest = NOWHERE;
        continue;
      }

      if (!LegalNPCDestination(pSoldier, sRandDest, ENSURE_PATH, NOWATER, 0)) {
        sRandDest = NOWHERE;
        continue;  // try again!
      }

      // passed all the tests, ->sDestination is acceptable
      fFound = TRUE;
      pSoldier->bPathStored = TRUE;  // optimization - Ian
    }
  }

  return (sRandDest);  // defaults to NOWHERE
}

INT16 ClosestReachableDisturbance(SOLDIERCLASS *pSoldier, UINT8 ubUnconsciousOK,
                                  BOOLEAN *pfChangeLevel) {
  INT16 *psLastLoc, *pusNoiseGridNo;
  INT8 *pbLastLevel;
  INT16 sGridNo = -1;
  INT8 bLevel, bClosestLevel;
  BOOLEAN fClimbingNecessary, fClosestClimbingNecessary = FALSE;
  INT32 iPathCost;
  INT16 sClosestDisturbance = NOWHERE;
  UINT32 uiLoop;
  UINT16 closestConscious = NOWHERE, closestUnconscious = NOWHERE;
  INT32 iShortestPath = 1000;
  INT32 iShortestPathConscious = 1000, iShortestPathUnconscious = 1000;
  UINT8 *pubNoiseVolume;
  INT8 *pbNoiseLevel;
  INT8 *pbPersOL, *pbPublOL;
  INT16 sClimbGridNo;
  SOLDIERCLASS *pOpp;

  // CJC: can't trace a path to every known disturbance!
  // for starters, try the closest one as the crow flies
  INT16 sClosestEnemy = NOWHERE, sDistToClosestEnemy = 1000, sDistToEnemy;

  // DIGGLER ON 22.11.2010
  // Во многих случаях можно вызывать эту функцию без указания pfChangeLevel за ненадобностью;
  // ПОэтому надо проверить, есть ли у нас этот указатель.
  if (pfChangeLevel)
    // DIGGLER OFF
    *pfChangeLevel = FALSE;

  pubNoiseVolume = &gubPublicNoiseVolume[pSoldier->bTeam];
  pusNoiseGridNo = &gsPublicNoiseGridno[pSoldier->bTeam];
  pbNoiseLevel = &gbPublicNoiseLevel[pSoldier->bTeam];

  // hang pointers at start of this guy's personal and public opponent opplists
  //	pbPersOL = &pSoldier->bOppList[0];
  //	pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);
  //	psLastLoc = &(gsLastKnownOppLoc[pSoldier->ubID][0]);

  // look through this man's personal & public opplists for opponents known
  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pOpp = MercSlots[uiLoop];

    // if this merc is inactive, at base, on assignment, or dead
    if (!pOpp) {
      continue;  // next merc
    }

    // if this merc is neutral/on same side, he's not an opponent
    if (CONSIDERED_NEUTRAL(pSoldier, pOpp) || (pSoldier->bSide == pOpp->bSide)) {
      continue;  // next merc
    }

    pbPersOL = pSoldier->bOppList + pOpp->ubID;
    pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpp->ubID;
    psLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pOpp->ubID;
    pbLastLevel = gbLastKnownOppLevel[pSoldier->ubID] + pOpp->ubID;

    // if this opponent is unknown personally and publicly
    if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN)) {
      continue;  // next merc
    }

    // this is possible if get here from BLACK AI in one of those rare
    // instances when we couldn't get a meaningful shot off at a guy in sight
    if ((*pbPersOL == SEEN_CURRENTLY) && (pOpp->bLife >= OKLIFE)) {
      // don't allow this to return any valid values, this guy remains a
      // serious threat and the last thing we want to do is approach him!
      return (NOWHERE);
    }

    // if personal knowledge is more up to date or at least equal
    if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
        (*pbPersOL == *pbPublOL)) {
      // using personal knowledge, obtain opponent's "best guess" gridno
      sGridNo = *psLastLoc;
      bLevel = *pbLastLevel;
    } else {
      // using public knowledge, obtain opponent's "best guess" gridno
      sGridNo = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpp->ubID];
      bLevel = gbPublicLastKnownOppLevel[pSoldier->bTeam][pOpp->ubID];
    }

    // if we are standing at that gridno (!, obviously our info is old...)
    if (sGridNo == pSoldier->sGridNo) {
      continue;  // next merc
    }

    if (sGridNo == NOWHERE) {
      // huh?
      continue;
    }

    sDistToEnemy = PythSpacesAway(pSoldier->sGridNo, sGridNo);
    if (sDistToEnemy < sDistToClosestEnemy) {
      sClosestEnemy = sGridNo;
      bClosestLevel = bLevel;
      sDistToClosestEnemy = sDistToEnemy;
    }
  }

  if (sClosestEnemy != NOWHERE) {
    iPathCost = EstimatePathCostToLocation(pSoldier, sClosestEnemy, bClosestLevel, FALSE,
                                           &fClimbingNecessary, &sClimbGridNo);
    // if we can get there
    if (iPathCost != 0) {
      if (fClimbingNecessary) {
        sClosestDisturbance = sClimbGridNo;
      } else {
        sClosestDisturbance = sClosestEnemy;
      }
      iShortestPath = iPathCost;
      fClosestClimbingNecessary = fClimbingNecessary;
    }
  }

  // if any "misc. noise" was also heard recently
  if (pSoldier->sNoiseGridno != NOWHERE && pSoldier->sNoiseGridno != sClosestDisturbance) {
    // test this gridno, too
    sGridNo = pSoldier->sNoiseGridno;
    bLevel = pSoldier->bNoiseLevel;

    // if we are there (at the noise gridno)
    if (sGridNo == pSoldier->sGridNo) {
      pSoldier->sNoiseGridno = NOWHERE;  // wipe it out, not useful anymore
      pSoldier->ubNoiseVolume = 0;
    } else {
      // get the AP cost to get to the location of the noise
      iPathCost = EstimatePathCostToLocation(pSoldier, sGridNo, bLevel, FALSE, &fClimbingNecessary,
                                             &sClimbGridNo);
      // if we can get there
      if (iPathCost != 0) {
        if (fClimbingNecessary) {
          sClosestDisturbance = sClimbGridNo;
        } else {
          sClosestDisturbance = sGridNo;
        }
        iShortestPath = iPathCost;
        fClosestClimbingNecessary = fClimbingNecessary;
      }
    }
  }

  // if any PUBLIC "misc. noise" was also heard recently
  if (*pusNoiseGridNo != NOWHERE && *pusNoiseGridNo != sClosestDisturbance) {
    // test this gridno, too
    sGridNo = *pusNoiseGridNo;
    bLevel = *pbNoiseLevel;

    // if we are not NEAR the noise gridno...
    if (pSoldier->bLevel != bLevel || PythSpacesAway(pSoldier->sGridNo, sGridNo) >= 6 ||
        SoldierTo3DLocationLineOfSightTest(pSoldier, sGridNo, bLevel, 0,
                                           (UINT8)MaxDistanceVisible(), FALSE) == 0)
    // if we are NOT there (at the noise gridno)
    //	if (sGridNo != pSoldier->sGridNo)
    {
      // get the AP cost to get to the location of the noise
      iPathCost = EstimatePathCostToLocation(pSoldier, sGridNo, bLevel, FALSE, &fClimbingNecessary,
                                             &sClimbGridNo);
      // if we can get there
      if (iPathCost != 0) {
        if (fClimbingNecessary) {
          sClosestDisturbance = sClimbGridNo;
        } else {
          sClosestDisturbance = sGridNo;
        }
        iShortestPath = iPathCost;
        fClosestClimbingNecessary = fClimbingNecessary;
      }
    } else {
      // degrade our public noise a bit
      *pusNoiseGridNo -= 2;
    }
  }

#ifdef DEBUGDECISIONS
  if (sClosestDisturbance != NOWHERE) {
    AINumMessage("CLOSEST DISTURBANCE is at gridno ", sClosestDisturbance);
  }
#endif

  // DIGGLER ON 22.11.2010
  // Проверяем, т.к. во многих случаях можно вызывать эту функцию с нулевым pfChangeLevel - за
  // ненадобностью;

  if (pfChangeLevel)
    // DIGGLER OFF
    *pfChangeLevel = fClosestClimbingNecessary;
  return (sClosestDisturbance);
}

INT16 ClosestKnownOpponent(SOLDIERCLASS *pSoldier, INT16 *psGridNo, INT8 *pbLevel) {
  INT16 *psLastLoc, sGridNo, sClosestOpponent = NOWHERE;
  UINT32 uiLoop;
  INT32 iRange, iClosestRange = 1500;
  INT8 *pbPersOL, *pbPublOL;
  INT8 bLevel, bClosestLevel;
  SOLDIERCLASS *pOpp;

  bClosestLevel = -1;

  // NOTE: THIS FUNCTION ALLOWS RETURN OF UNCONSCIOUS AND UNREACHABLE OPPONENTS
  psLastLoc = &(gsLastKnownOppLoc[pSoldier->ubID][0]);

  // hang pointers at start of this guy's personal and public opponent opplists
  pbPersOL = &pSoldier->bOppList[0];
  pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);

  // look through this man's personal & public opplists for opponents known
  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pOpp = MercSlots[uiLoop];

    // if this merc is inactive, at base, on assignment, or dead
    if (!pOpp) {
      continue;  // next merc
    }

    // if this merc is neutral/on same side, he's not an opponent
    if (CONSIDERED_NEUTRAL(pSoldier, pOpp) || (pSoldier->bSide == pOpp->bSide)) {
      continue;  // next merc
    }

    // Special stuff for Carmen the bounty hunter
    if (pSoldier->bAttitude == ATTACKSLAYONLY && pOpp->ubProfile != 64) {
      continue;  // next opponent
    }

    pbPersOL = pSoldier->bOppList + pOpp->ubID;
    pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpp->ubID;
    psLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pOpp->ubID;

    // if this opponent is unknown personally and publicly
    if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN)) {
      continue;  // next merc
    }

    // if personal knowledge is more up to date or at least equal
    if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
        (*pbPersOL == *pbPublOL)) {
      // using personal knowledge, obtain opponent's "best guess" gridno
      sGridNo = gsLastKnownOppLoc[pSoldier->ubID][pOpp->ubID];
      bLevel = gbLastKnownOppLevel[pSoldier->ubID][pOpp->ubID];
    } else {
      // using public knowledge, obtain opponent's "best guess" gridno
      sGridNo = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpp->ubID];
      bLevel = gbPublicLastKnownOppLevel[pSoldier->bTeam][pOpp->ubID];
    }

    // if we are standing at that gridno(!, obviously our info is old...)
    if (sGridNo == pSoldier->sGridNo) {
      continue;  // next merc
    }

    // this function is used only for turning towards closest opponent or changing stance
    // as such, if they AI is in a building,
    // we should ignore people who are on the roof of the same building as the AI
    if ((bLevel != pSoldier->bLevel) && SameBuilding(pSoldier->sGridNo, sGridNo)) {
      continue;
    }

    // I hope this will be good enough; otherwise we need a fractional/world-units-based 2D distance
    // function
    // sRange = PythSpacesAway( pSoldier->sGridNo, sGridNo);
    iRange = GetRangeInCellCoordsFromGridNoDiff(pSoldier->sGridNo, sGridNo);

    if (iRange < iClosestRange) {
      iClosestRange = iRange;
      sClosestOpponent = sGridNo;
      bClosestLevel = bLevel;
    }
  }

#ifdef DEBUGDECISIONS
  if (sClosestOpponent != NOWHERE) {
    AINumMessage("CLOSEST OPPONENT is at gridno ", sClosestOpponent);
  }
#endif

  if (psGridNo) {
    *psGridNo = sClosestOpponent;
  }
  if (pbLevel) {
    *pbLevel = bClosestLevel;
  }
  return (sClosestOpponent);
}

INT16 ClosestSeenOpponent(SOLDIERCLASS *pSoldier, INT16 *psGridNo, INT8 *pbLevel) {
  INT16 sGridNo, sClosestOpponent = NOWHERE;
  UINT32 uiLoop;
  INT32 iRange, iClosestRange = 1500;
  INT8 *pbPersOL;
  INT8 bLevel, bClosestLevel;
  SOLDIERCLASS *pOpp;

  bClosestLevel = -1;

  // look through this man's personal & public opplists for opponents known
  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pOpp = MercSlots[uiLoop];

    // if this merc is inactive, at base, on assignment, or dead
    if (!pOpp) {
      continue;  // next merc
    }

    // if this merc is neutral/on same side, he's not an opponent
    if (CONSIDERED_NEUTRAL(pSoldier, pOpp) || (pSoldier->bSide == pOpp->bSide)) {
      continue;  // next merc
    }

    // Special stuff for Carmen the bounty hunter
    if (pSoldier->bAttitude == ATTACKSLAYONLY && pOpp->ubProfile != 64) {
      continue;  // next opponent
    }

    pbPersOL = pSoldier->bOppList + pOpp->ubID;

    // if this opponent is not seen personally
    if (*pbPersOL != SEEN_CURRENTLY) {
      continue;  // next merc
    }

    // since we're dealing with seen people, use exact gridnos
    sGridNo = pOpp->sGridNo;
    bLevel = pOpp->bLevel;

    // if we are standing at that gridno(!, obviously our info is old...)
    if (sGridNo == pSoldier->sGridNo) {
      continue;  // next merc
    }

    // this function is used only for turning towards closest opponent or changing stance
    // as such, if they AI is in a building,
    // we should ignore people who are on the roof of the same building as the AI
    if ((bLevel != pSoldier->bLevel) && SameBuilding(pSoldier->sGridNo, sGridNo)) {
      continue;
    }

    // I hope this will be good enough; otherwise we need a fractional/world-units-based 2D distance
    // function
    // sRange = PythSpacesAway( pSoldier->sGridNo, sGridNo);
    iRange = GetRangeInCellCoordsFromGridNoDiff(pSoldier->sGridNo, sGridNo);

    if (iRange < iClosestRange) {
      iClosestRange = iRange;
      sClosestOpponent = sGridNo;
      bClosestLevel = bLevel;
    }
  }

#ifdef DEBUGDECISIONS
  if (sClosestOpponent != NOWHERE) {
    AINumMessage("CLOSEST OPPONENT is at gridno ", sClosestOpponent);
  }
#endif

  if (psGridNo) {
    *psGridNo = sClosestOpponent;
  }
  if (pbLevel) {
    *pbLevel = bClosestLevel;
  }
  return (sClosestOpponent);
}

INT16 ClosestPC(SOLDIERCLASS *pSoldier, INT16 *psDistance) {
  // used by NPCs... find the closest PC

  // NOTE: skips EPCs!

  UINT8 ubLoop;
  SOLDIERCLASS *pTargetSoldier;
  INT16 sMinDist = (INT16)WORLD_MAX;
  INT16 sDist;
  INT16 sGridNo = NOWHERE;

  // Loop through all mercs on player team
  ubLoop = gTacticalStatus.Team[PLAYER_TEAM].bFirstID;

  for (; ubLoop <= gTacticalStatus.Team[PLAYER_TEAM].bLastID; ubLoop++) {
    pTargetSoldier = Menptr + ubLoop;

    if (!pTargetSoldier->bActive || !pTargetSoldier->bInSector) {
      continue;
    }

    // if not conscious, skip him
    if (pTargetSoldier->bLife < OKLIFE) {
      continue;
    }

    if (AM_AN_EPC(pTargetSoldier)) {
      continue;
    }

    sDist = PythSpacesAway(pSoldier->sGridNo, pTargetSoldier->sGridNo);

    // if this PC is not visible to the soldier, then add a penalty to the distance
    // so that we weight in favour of visible mercs
    if (pTargetSoldier->bTeam != pSoldier->bTeam && pSoldier->bOppList[ubLoop] != SEEN_CURRENTLY) {
      sDist += 10;
    }

    if (sDist < sMinDist) {
      sMinDist = sDist;
      sGridNo = pTargetSoldier->sGridNo;
    }
  }

  if (psDistance) {
    *psDistance = sMinDist;
  }

  return (sGridNo);
}

INT16 FindClosestClimbPointAvailableToAI(SOLDIERCLASS *pSoldier, INT16 sStartGridNo,
                                         INT16 sDesiredGridNo, BOOLEAN fClimbUp) {
  INT16 sGridNo;
  UINT16 usRoamingOrigin;
  INT16 sRoamingRange;

  if (pSoldier->uiStatusFlags & SOLDIER_PC) {
    usRoamingOrigin = pSoldier->sGridNo;
    sRoamingRange = 99;
  } else {
    sRoamingRange = RoamingRange(pSoldier, &usRoamingOrigin);
  }

  // since climbing necessary involves going an extra tile, we compare against 1 less than the roam
  // range... or add 1 to the distance to the climb point

  sGridNo = FindClosestClimbPoint(sStartGridNo, sDesiredGridNo, fClimbUp);

  if (PythSpacesAway(usRoamingOrigin, sGridNo) + 1 > sRoamingRange) {
    return (NOWHERE);
  } else {
    return (sGridNo);
  }
}

BOOLEAN ClimbingNecessary(SOLDIERCLASS *pSoldier, INT16 sDestGridNo, INT8 bDestLevel) {
  if (pSoldier->bLevel == bDestLevel) {
    if ((pSoldier->bLevel == 0) ||
        (gubBuildingInfo[pSoldier->sGridNo] == gubBuildingInfo[sDestGridNo])) {
      return (FALSE);
    } else  // different buildings!
    {
      return (TRUE);
    }
  } else {
    return (TRUE);
  }
}

INT16 GetInterveningClimbingLocation(SOLDIERCLASS *pSoldier, INT16 sDestGridNo, INT8 bDestLevel,
                                     BOOLEAN *pfClimbingNecessary) {
  if (pSoldier->bLevel == bDestLevel) {
    if ((pSoldier->bLevel == 0) ||
        (gubBuildingInfo[pSoldier->sGridNo] == gubBuildingInfo[sDestGridNo])) {
      // on ground or same building... normal!
      *pfClimbingNecessary = FALSE;
      return (NOWHERE);
    } else {
      // different buildings!
      // yes, pass in same gridno twice... want closest climb-down spot for building we are on!
      *pfClimbingNecessary = TRUE;
      return (FindClosestClimbPointAvailableToAI(pSoldier, pSoldier->sGridNo, pSoldier->sGridNo,
                                                 FALSE));
    }
  } else {
    *pfClimbingNecessary = TRUE;
    // different levels
    if (pSoldier->bLevel == 0) {
      // got to go UP onto building
      return (FindClosestClimbPointAvailableToAI(pSoldier, pSoldier->sGridNo, sDestGridNo, TRUE));
    } else {
      // got to go DOWN off building
      return (FindClosestClimbPointAvailableToAI(pSoldier, pSoldier->sGridNo, pSoldier->sGridNo,
                                                 FALSE));
    }
  }
}

INT16 EstimatePathCostToLocation(SOLDIERCLASS *pSoldier, INT16 sDestGridNo, INT8 bDestLevel,
                                 BOOLEAN fAddCostAfterClimbingUp, BOOLEAN *pfClimbingNecessary,
                                 INT16 *psClimbGridNo) {
  INT16 sPathCost;
  INT16 sClimbGridNo;

  if (pSoldier->bLevel == bDestLevel) {
    if ((pSoldier->bLevel == 0) ||
        (gubBuildingInfo[pSoldier->sGridNo] == gubBuildingInfo[sDestGridNo])) {
      // on ground or same building... normal!
      sPathCost =
          EstimatePlotPath(pSoldier, sDestGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0);
      *pfClimbingNecessary = FALSE;
      *psClimbGridNo = NOWHERE;
    } else {
      // different buildings!
      // yes, pass in same gridno twice... want closest climb-down spot for building we are on!
      sClimbGridNo =
          FindClosestClimbPointAvailableToAI(pSoldier, pSoldier->sGridNo, pSoldier->sGridNo, FALSE);
      if (sClimbGridNo == NOWHERE) {
        sPathCost = 0;
      } else {
        sPathCost = PlotPath(pSoldier, sClimbGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0);
        if (sPathCost != 0) {
          // add in cost of climbing down
          if (fAddCostAfterClimbingUp) {
            // add in cost of later climbing up, too
            sPathCost += AP_CLIMBOFFROOF + AP_CLIMBROOF;
            // add in an estimate of getting there after climbing down
            sPathCost += (AP_MOVEMENT_FLAT + WALKCOST) * PythSpacesAway(sClimbGridNo, sDestGridNo);
          } else {
            sPathCost += AP_CLIMBOFFROOF;
            // add in an estimate of getting there after climbing down, *but not on top of roof*
            sPathCost +=
                (AP_MOVEMENT_FLAT + WALKCOST) * PythSpacesAway(sClimbGridNo, sDestGridNo) / 2;
          }
          *pfClimbingNecessary = TRUE;
          *psClimbGridNo = sClimbGridNo;
        }
      }
    }
  } else {
    // different levels
    if (pSoldier->bLevel == 0) {
      // got to go UP onto building
      sClimbGridNo =
          FindClosestClimbPointAvailableToAI(pSoldier, pSoldier->sGridNo, sDestGridNo, TRUE);
    } else {
      // got to go DOWN off building
      sClimbGridNo =
          FindClosestClimbPointAvailableToAI(pSoldier, pSoldier->sGridNo, pSoldier->sGridNo, FALSE);
    }

    if (sClimbGridNo == NOWHERE) {
      sPathCost = 0;
    } else {
      sPathCost = PlotPath(pSoldier, sClimbGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0);
      if (sPathCost != 0) {
        // add in the cost of climbing up or down
        if (pSoldier->bLevel == 0) {
          // must climb up
          sPathCost += AP_CLIMBROOF;
          if (fAddCostAfterClimbingUp) {
            // add to path a rough estimate of how far to go from the climb gridno to the friend
            // estimate walk cost
            sPathCost += (AP_MOVEMENT_FLAT + WALKCOST) * PythSpacesAway(sClimbGridNo, sDestGridNo);
          }
        } else {
          // must climb down
          sPathCost += AP_CLIMBOFFROOF;
          // add to path a rough estimate of how far to go from the climb gridno to the friend
          // estimate walk cost
          sPathCost += (AP_MOVEMENT_FLAT + WALKCOST) * PythSpacesAway(sClimbGridNo, sDestGridNo);
        }
        *pfClimbingNecessary = TRUE;
        *psClimbGridNo = sClimbGridNo;
      }
    }
  }

  return (sPathCost);
}

BOOLEAN GuySawEnemyThisTurnOrBefore(SOLDIERCLASS *pSoldier) {
  UINT8 ubTeamLoop;
  UINT8 ubIDLoop;

  //***10.12.2009*** изменена верхняя граница цикла
  for (ubTeamLoop = 0; ubTeamLoop < LAST_TEAM + 1 /*MAXTEAMS*/; ubTeamLoop++) {
    if (gTacticalStatus.Team[ubTeamLoop].bSide != pSoldier->bSide) {
      // consider guys in this team, which isn't on our side
      for (ubIDLoop = gTacticalStatus.Team[ubTeamLoop].bFirstID;
           ubIDLoop <= gTacticalStatus.Team[ubTeamLoop].bLastID; ubIDLoop++) {
        // if this guy SAW an enemy recently...
        if (pSoldier->bOppList[ubIDLoop] >= SEEN_CURRENTLY) {
          return (TRUE);
        }
      }
    }
  }

  return (FALSE);
}

INT16 ClosestReachableFriendInTrouble(SOLDIERCLASS *pSoldier, BOOLEAN *pfClimbingNecessary) {
  UINT32 uiLoop;
  INT16 sPathCost, sClosestFriend = NOWHERE, sShortestPath = 1000, sClimbGridNo;
  BOOLEAN fClimbingNecessary, fClosestClimbingNecessary = FALSE;
  SOLDIERCLASS *pFriend;

  // civilians don't really have any "friends", so they don't bother with this
  if (PTR_CIVILIAN) {
    return (sClosestFriend);
  }

  // consider every friend of this soldier (locations assumed to be known)
  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pFriend = MercSlots[uiLoop];

    // if this merc is inactive, at base, on assignment, or dead
    if (!pFriend) {
      continue;  // next merc
    }

    // if this merc is neutral or NOT on the same side, he's not a friend
    if (pFriend->bNeutral || (pSoldier->bSide != pFriend->bSide)) {
      continue;  // next merc
    }

    // if this "friend" is actually US
    if (pFriend->ubID == pSoldier->ubID) {
      continue;  // next merc
    }

    // CJC: restrict "last one to radio" to only if that guy saw us this turn or last turn

    // if this friend is not under fire, and isn't the last one to radio
    if (!(pFriend->bUnderFire ||
          (pFriend->ubID == gTacticalStatus.Team[pFriend->bTeam].ubLastMercToRadio &&
           GuySawEnemyThisTurnOrBefore(pFriend)))) {
      continue;  // next merc
    }

    //***01.09.2013*** помощник побежит сразу по координатам угрозы
    if (pFriend->ubPreviousAttackerID != NOBODY) {
      pFriend = MercPtrs[pFriend->ubPreviousAttackerID];
    } else                                                        ///
                                                                  // if we're already neighbors
        if (SpacesAway(pSoldier->sGridNo, pFriend->sGridNo) < 4)  ///== 1)
    {
      continue;  // next merc
    }

    // get the AP cost to go to this friend's gridno
    sPathCost = EstimatePathCostToLocation(pSoldier, pFriend->sGridNo, pFriend->bLevel, TRUE,
                                           &fClimbingNecessary, &sClimbGridNo);

    // if we can get there
    if (sPathCost != 0) {
      // sprintf(tempstr,"Path cost to friend %s's location is %d",pFriend->name,pathCost);
      // PopMessage(tempstr);

      if (sPathCost < sShortestPath) {
        if (fClimbingNecessary) {
          sClosestFriend = sClimbGridNo;
        } else {
          sClosestFriend = pFriend->sGridNo;
        }

        sShortestPath = sPathCost;
        fClosestClimbingNecessary = fClimbingNecessary;
      }
    }
  }

#ifdef DEBUGDECISIONS
  if (sClosestFriend != NOWHERE) {
    AINumMessage("CLOSEST FRIEND is at gridno ", sClosestFriend);
  }
#endif

  *pfClimbingNecessary = fClosestClimbingNecessary;
  return (sClosestFriend);
}

INT16 DistanceToClosestFriend(SOLDIERCLASS *pSoldier) {
  // find the distance to the closest person on the same team
  UINT8 ubLoop;
  SOLDIERCLASS *pTargetSoldier;
  INT16 sMinDist = 1000;
  INT16 sDist;

  // Loop through all mercs on player team
  ubLoop = gTacticalStatus.Team[pSoldier->bTeam].bFirstID;

  for (; ubLoop <= gTacticalStatus.Team[pSoldier->bTeam].bLastID; ubLoop++) {
    if (ubLoop == pSoldier->ubID) {
      // same guy - continue!
      continue;
    }

    pTargetSoldier = Menptr + ubLoop;

    if (pSoldier->bActive && pSoldier->bInSector) {
      if (!pTargetSoldier->bActive || !pTargetSoldier->bInSector) {
        continue;
      }
      // if not conscious, skip him
      else if (pTargetSoldier->bLife < OKLIFE) {
        continue;
      }
    } else {
      // compare sector #s
      if ((pSoldier->sSectorX != pTargetSoldier->sSectorX) ||
          (pSoldier->sSectorY != pTargetSoldier->sSectorY) ||
          (pSoldier->bSectorZ != pTargetSoldier->bSectorZ)) {
        continue;
      } else if (pTargetSoldier->bLife < OKLIFE) {
        continue;
      } else {
        // well there's someone who could be near
        return (1);
      }
    }

    sDist = SpacesAway(pSoldier->sGridNo, pTargetSoldier->sGridNo);

    if (sDist < sMinDist) {
      sMinDist = sDist;
    }
  }

  return (sMinDist);
}

BOOLEAN InWaterGasOrSmoke(SOLDIERCLASS *pSoldier, INT16 sGridNo) {
  if (WaterTooDeepForAttacks(sGridNo)) {
    return (TRUE);
  }

  // smoke
  if (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_SMOKE) {
    return (TRUE);
  }

  // tear/mustard gas
  if ((gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] &
       (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS)) &&
      (pSoldier->inv[HEAD1POS].usItem != GASMASK && pSoldier->inv[HEAD2POS].usItem != GASMASK)) {
    return (TRUE);
  }

  //***12.08.2014*** огонь
  if (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_CREATUREGAS) {
    return (TRUE);
  }

  return (FALSE);
}

BOOLEAN InGasOrSmoke(SOLDIERCLASS *pSoldier, INT16 sGridNo) {
  // smoke
  if (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_SMOKE) {
    return (TRUE);
  }

  // tear/mustard gas
  if ((gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] &
       (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS)) &&
      (pSoldier->inv[HEAD1POS].usItem != GASMASK && pSoldier->inv[HEAD2POS].usItem != GASMASK)) {
    return (TRUE);
  }

  //***12.08.2014*** огонь
  if (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_CREATUREGAS) {
    return (TRUE);
  }

  return (FALSE);
}

INT16 InWaterOrGas(SOLDIERCLASS *pSoldier, INT16 sGridNo) {
  if (WaterTooDeepForAttacks(sGridNo)) {
    return (TRUE);
  }

  // tear/mustard gas
  if ((gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] &
       (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS)) &&
      (pSoldier->inv[HEAD1POS].usItem != GASMASK && pSoldier->inv[HEAD2POS].usItem != GASMASK)) {
    return (TRUE);
  }

  //***12.08.2014*** огонь
  if (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_CREATUREGAS) {
    return (TRUE);
  }

  return (FALSE);
}

BOOLEAN InGas_old(SOLDIERCLASS *pSoldier, INT16 sGridNo) {
  // tear/mustard gas
  if ((gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] &
       (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS)) &&
      (pSoldier->inv[HEAD1POS].usItem != GASMASK && pSoldier->inv[HEAD2POS].usItem != GASMASK)) {
    return (TRUE);
  }

  return (FALSE);
}

//***19.07.2013*** наличие газа в квадрате 3х3
BOOLEAN InGas(SOLDIERCLASS *pSoldier, INT16 sGridNo) {
  INT16 sXOffset, sYOffset, sNewGridNo;

  for (sYOffset = -1; sYOffset <= 1; sYOffset++) {
    for (sXOffset = -1; sXOffset <= 1; sXOffset++) {
      sNewGridNo = sGridNo + sXOffset + (MAXCOL * sYOffset);
      if (!(sNewGridNo >= 0 && sNewGridNo < WORLD_MAX)) {
        continue;
      }

      // tear/mustard gas
      if ((gpWorldLevelData[sNewGridNo].ubExtFlags[pSoldier->bLevel] &
           (MAPELEMENT_EXT_TEARGAS | MAPELEMENT_EXT_MUSTARDGAS)) &&
          (pSoldier->inv[HEAD1POS].usItem != GASMASK &&
           pSoldier->inv[HEAD2POS].usItem != GASMASK)) {
        return (TRUE);
      }

      //***12.08.2014*** огонь
      if (gpWorldLevelData[sNewGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_CREATUREGAS) {
        return (TRUE);
      }
    }
  }

  return (FALSE);
}

//***21.07.2013***
BOOLEAN InSmoke(SOLDIERCLASS *pSoldier, INT16 sGridNo) {
  // smoke
  if (gpWorldLevelData[sGridNo].ubExtFlags[pSoldier->bLevel] & MAPELEMENT_EXT_SMOKE) {
    return (TRUE);
  }

  return (FALSE);
}

BOOLEAN WearGasMaskIfAvailable(SOLDIERCLASS *pSoldier) {
  INT8 bSlot, bNewSlot;

  bSlot = FindObj(pSoldier, GASMASK);
  if (bSlot == NO_SLOT) {
    return (FALSE);
  }
  if (bSlot == HEAD1POS || bSlot == HEAD2POS) {
    return (FALSE);
  }
  if (pSoldier->inv[HEAD1POS].usItem == NOTHING) {
    bNewSlot = HEAD1POS;
  } else if (pSoldier->inv[HEAD2POS].usItem == NOTHING) {
    bNewSlot = HEAD2POS;
  } else {
    // screw it, going in position 1 anyhow
    bNewSlot = HEAD1POS;
  }

  RearrangePocket(pSoldier, bSlot, bNewSlot, TRUE);
  return (TRUE);
}

BOOLEAN InLightAtNight(INT16 sGridNo, INT8 bLevel) {
  UINT8 ubBackgroundLightLevel;

  // do not consider us to be "in light" if we're in an underground sector
  if (gbWorldSectorZ > 0) {
    return (FALSE);
  }

  ubBackgroundLightLevel = GetTimeOfDayAmbientLightLevel();

  if (ubBackgroundLightLevel < NORMAL_LIGHTLEVEL_DAY + 2) {
    // don't consider it nighttime, too close to daylight levels
    return (FALSE);
  }

  // could've been placed here, ignore the light
  if (InARoom(sGridNo, NULL)) {
    return (FALSE);
  }

  // NB light levels are backwards, so a lower light level means the
  // spot in question is BRIGHTER

  if (LightTrueLevel(sGridNo, bLevel) < ubBackgroundLightLevel) {
    return (TRUE);
  }

  return (FALSE);
}

INT8 CalcMorale(SOLDIERCLASS *pSoldier) {
  UINT32 uiLoop, uiLoop2;
  INT32 iOurTotalThreat = 0, iTheirTotalThreat = 0;
  INT16 sOppThreatValue, sFrndThreatValue, sMorale;
  INT32 iPercent;
  INT8 bMostRecentOpplistValue;
  INT8 bMoraleCategory;
  INT8 *pSeenOpp;  //,*friendOlPtr;
  INT8 *pbPersOL, *pbPublOL;
  SOLDIERCLASS *pOpponent, *pFriend;

  // if army guy has NO weapons left then panic!
  if (pSoldier->bTeam == ENEMY_TEAM) {
    if (FindAIUsableObjClass(pSoldier, IC_WEAPON) == NO_SLOT) {
      return (MORALE_HOPELESS);
    }
  }

  // hang pointers to my personal opplist, my team's public opplist, and my
  // list of previously seen opponents
  pSeenOpp = &(gbSeenOpponents[pSoldier->ubID][0]);

  // loop through every one of my possible opponents
  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pOpponent = MercSlots[uiLoop];

    // if this merc is inactive, at base, on assignment, dead, unconscious
    if (!pOpponent || (pOpponent->bLife < OKLIFE)) continue;  // next merc

    // if this merc is neutral/on same side, he's not an opponent, skip him!
    if (CONSIDERED_NEUTRAL(pSoldier, pOpponent) || (pSoldier->bSide == pOpponent->bSide))
      continue;  // next merc

    // Special stuff for Carmen the bounty hunter
    if (pSoldier->bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != 64) {
      continue;  // next opponent
    }

    pbPersOL = pSoldier->bOppList + pOpponent->ubID;
    pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpponent->ubID;
    pSeenOpp = gbSeenOpponents[pSoldier->ubID] + pOpponent->ubID;

    // if this opponent is unknown to me personally AND unknown to my team, too
    if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN)) {
      // if I have never seen him before anywhere in this sector, either
      if (!(*pSeenOpp)) continue;  // next merc

      // have seen him in the past, so he remains something of a threat
      bMostRecentOpplistValue = 0;  // uses the free slot for 0 opplist
    } else                          // decide which opplist is more current
    {
      // if personal knowledge is more up to date or at least equal
      if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
          (*pbPersOL == *pbPublOL))
        bMostRecentOpplistValue = *pbPersOL;  // use personal
      else
        bMostRecentOpplistValue = *pbPublOL;  // use public
    }

    iPercent = ThreatPercent[bMostRecentOpplistValue - OLDEST_HEARD_VALUE];

    //***30.10.2008*** повышение моральной стойкости атакующих скриптов, боимся только технику
    sOppThreatValue = 1;

    // JZ: 25.03.2015 Замена макроса "TANK( p )" на функцию
    if (pSoldier->bOrders != SEEKENEMY && pSoldier->bOrders != ONCALL || AM_A_ROBOT(pOpponent) ||
        /*TANK(pOpponent)*/ IsTank(pOpponent))
      sOppThreatValue =
          (iPercent * CalcManThreatValue(pOpponent, pSoldier->sGridNo, FALSE, pSoldier)) / 100;

    // sprintf(tempstr,"Known opponent %s, opplist status %d, percent %d, threat = %d",
    //           ExtMen[pOpponent->ubID].name,ubMostRecentOpplistValue,ubPercent,sOppThreatValue);
    // PopMessage(tempstr);

    // ADD this to their running total threatValue (decreases my MORALE)
    iTheirTotalThreat += sOppThreatValue;
    // NumMessage("Their TOTAL threat now = ",sTheirTotalThreat);

    // NOW THE FUN PART: SINCE THIS OPPONENT IS KNOWN TO ME IN SOME WAY,
    // ANY FRIENDS OF MINE THAT KNOW ABOUT HIM BOOST MY MORALE.  SO, LET'S GO
    // THROUGH THEIR PERSONAL OPPLISTS AND CHECK WHICH OF MY FRIENDS KNOW
    // SOMETHING ABOUT HIM AND WHAT THEIR THREAT VALUE TO HIM IS.

    for (uiLoop2 = 0; uiLoop2 < guiNumMercSlots; uiLoop2++) {
      pFriend = MercSlots[uiLoop2];

      // if this merc is inactive, at base, on assignment, dead, unconscious
      if (!pFriend || (pFriend->bLife < OKLIFE)) continue;  // next merc

      // if this merc is not on my side, then he's NOT one of my friends

      // WE CAN'T AFFORD TO CONSIDER THE ENEMY OF MY ENEMY MY FRIEND, HERE!
      // ONLY IF WE ARE ACTUALLY OFFICIALLY CO-OPERATING TOGETHER (SAME SIDE)
      if (pFriend->bNeutral && !(pSoldier->ubCivilianGroup != NON_CIV_GROUP &&
                                 pSoldier->ubCivilianGroup == pFriend->ubCivilianGroup)) {
        continue;  // next merc
      }

      if (pSoldier->bSide != pFriend->bSide) continue;  // next merc

      // THIS TEST IS INVALID IF A COMPUTER-TEAM IS PLAYING CO-OPERATIVELY
      // WITH A NON-COMPUTER TEAM SINCE THE OPPLISTS INVOLVED ARE NOT
      // UP-TO-DATE.  THIS SITUATION IS CURRENTLY NOT POSSIBLE IN HTH/DG.

      // ALSO NOTE THAT WE COUNT US AS OUR (BEST) FRIEND FOR THESE CALCULATIONS

      // subtract HEARD_2_TURNS_AGO (which is negative) to make values start at 0 and
      // be positive otherwise
      iPercent = ThreatPercent[pFriend->bOppList[pOpponent->ubID] - OLDEST_HEARD_VALUE];

      // reduce the percentage value based on how far away they are from the enemy, if they only
      // hear him
      if (pFriend->bOppList[pOpponent->ubID] <= HEARD_LAST_TURN) {
        iPercent -= PythSpacesAway(pSoldier->sGridNo, pFriend->sGridNo) * 2;
        if (iPercent <= 0) {
          // ignore!
          continue;
        }
      }

      sFrndThreatValue =
          (iPercent * CalcManThreatValue(pFriend, pOpponent->sGridNo, FALSE, pSoldier)) / 100;

      // sprintf(tempstr,"Known by friend %s, opplist status %d, percent %d, threat = %d",
      //         ExtMen[pFriend->ubID].name,pFriend->bOppList[pOpponent->ubID],ubPercent,sFrndThreatValue);
      // PopMessage(tempstr);

      // ADD this to our running total threatValue (increases my MORALE)
      // We multiply by sOppThreatValue to PRO-RATE this based on opponent's
      // threat value to ME personally.  Divide later by sum of them all.
      iOurTotalThreat += sOppThreatValue * sFrndThreatValue;
    }

    // this could get slow if I have a lot of friends...
    // KeepInterfaceGoing();
  }

  // if they are no threat whatsoever
  if (!iTheirTotalThreat)
    sMorale = 500;  // our morale is just incredible
  else {
    // now divide sOutTotalThreat by sTheirTotalThreat to get the REAL value
    iOurTotalThreat /= iTheirTotalThreat;

    // calculate the morale (100 is even, < 100 is us losing, > 100 is good)
    sMorale = (INT16)((100 * iOurTotalThreat) / iTheirTotalThreat);
  }

  if (sMorale <= 25)  // odds 1:4 or worse
    bMoraleCategory = MORALE_HOPELESS;
  else if (sMorale <= 50)  // odds between 1:4 and 1:2
    bMoraleCategory = MORALE_WORRIED;
  else if (sMorale <= 150)  // odds between 1:2 and 3:2
    bMoraleCategory = MORALE_NORMAL;
  else if (sMorale <= 300)  // odds between 3:2 and 3:1
    bMoraleCategory = MORALE_CONFIDENT;
  else  // odds better than 3:1
    bMoraleCategory = MORALE_FEARLESS;

  switch (pSoldier->bAttitude) {
    case DEFENSIVE:
      bMoraleCategory--;
      break;
    case BRAVESOLO:
      bMoraleCategory += 2;
      break;
    case BRAVEAID:
      bMoraleCategory += 2;
      break;
    case CUNNINGSOLO:
      break;
    case CUNNINGAID:
      break;
    case AGGRESSIVE:
      bMoraleCategory++;
      break;
  }

  // make idiot administrators much more aggressive
  if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR) {
    bMoraleCategory += 2;
  }

  // if still full of energy
  if (pSoldier->bBreath > 75)
    bMoraleCategory++;
  else {
    // if getting a bit low on breath
    if (pSoldier->bBreath < 40) bMoraleCategory--;

    // if getting REALLY low on breath
    if (pSoldier->bBreath < 10) bMoraleCategory--;
  }

  // if still very healthy
  if (pSoldier->bLife > 75)
    bMoraleCategory++;
  else {
    // if getting a bit low on life
    if (pSoldier->bLife < 40) bMoraleCategory--;

    // if getting REALLY low on life
    if (pSoldier->bLife < 20) bMoraleCategory--;
  }

  // if soldier is currently not under fire
  if (!pSoldier->bUnderFire) bMoraleCategory++;

  // if adjustments made it outside the allowed limits
  if (bMoraleCategory < MORALE_HOPELESS)
    bMoraleCategory = MORALE_HOPELESS;
  else {
    if (bMoraleCategory > MORALE_FEARLESS) bMoraleCategory = MORALE_FEARLESS;
  }

  //***31.03.2008*** для падение морали у AI от заградительного огня
  if (pSoldier->bMorale / 20 < bMoraleCategory) {
    bMoraleCategory = pSoldier->bMorale / 20;
  }

  //***18.09.2008*** у джипа всегда высокая мораль
  if (AM_A_ROBOT(pSoldier)) bMoraleCategory = MORALE_FEARLESS;

  // if only 1/4 of side left, reduce morale
  // and do this after we've capped all those other silly values
  /*
  if ( pSoldier->bTeam == ENEMY_TEAM && gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector <=
  gTacticalStatus.bOriginalSizeOfEnemyForce / 4 )
  {
         bMoraleCategory -= 2;
   if (bMoraleCategory < MORALE_HOPELESS)
     bMoraleCategory = MORALE_HOPELESS;
  }
  */

  // brave guys never get hopeless, at worst they get worried
  if (bMoraleCategory == MORALE_HOPELESS &&
      (pSoldier->bAttitude == BRAVESOLO || pSoldier->bAttitude == BRAVEAID))
    bMoraleCategory = MORALE_WORRIED;

#ifdef DEBUGDECISIONS
  DebugAI(String("Morale = %d (category %d), sOutTotalThreat %d, sTheirTotalThreat %d\n", sMorale,
                 bMoraleCategory, iOurTotalThreat, iTheirTotalThreat));
#endif

  return (bMoraleCategory);
}

INT32 CalcManThreatValue(SOLDIERCLASS *pEnemy, INT16 sMyGrid, UINT8 ubReduceForCover,
                         SOLDIERCLASS *pMe) {
  INT32 iThreatValue = 0;
  BOOLEAN fForCreature = CREATURE_OR_BLOODCAT(pMe);

  // If man is inactive, at base, on assignment, dead, unconscious
  if (!pEnemy->bActive || !pEnemy->bInSector || pEnemy->Unconscious()) {
    // he's no threat at all, return a negative number
    iThreatValue = -999;
    return (iThreatValue);
  }

  // in boxing mode, let only a boxer be considered a threat.
  if ((gTacticalStatus.bBoxingState == BOXING) && !AM_A_BOXER(pEnemy)) {
    iThreatValue = -999;
    return (iThreatValue);
  }

  //***07.06.2013*** находящиеся в воде ничего не боятся
  if (Water(sMyGrid)) {
    iThreatValue = -999;
    return (iThreatValue);
  }

  //***8.11.2007*** бронетехника имеет повышенный уровень угрозы для AI
  // JZ: 25.03.2015 Замена макроса "TANK( p )" на функцию
  if (AM_A_ROBOT(pEnemy) || /*TANK(pEnemy)*/ IsTank(pEnemy)) {
    iThreatValue = 10000;
    return (iThreatValue);
  }

  //***10.11.2010*** солдаты и милиция не оценивают уровень опасности оппонента

  // DIGGLER ON   24.11.2010
  // Это почему они не оценивают-то? Убегают что ль? Дык разберемся, полечим...
  /*if( pMe->bTeam == ENEMY_TEAM || pMe->bTeam == MILITIA_TEAM )
  {
          iThreatValue = 1;
          return( iThreatValue );
  }*/
  // DIGGLER OFF

  if (fForCreature) {
    // health (1-100)
    iThreatValue += pEnemy->bLife;
    // bleeding (more attactive!) (1-100)
    iThreatValue += pEnemy->bBleeding;
    // decrease according to distance
    iThreatValue = (iThreatValue * 10) / (10 + PythSpacesAway(sMyGrid, pEnemy->sGridNo));

  } else {
    // ADD twice the man's level (2-20)
    // DIGGLER ON 02.12.2010  Написано, что уровень прибавляем двойной.
    // iThreatValue += pEnemy->bExpLevel;
    iThreatValue += 2 * pEnemy->bExpLevel;
    // DIGGLER OFF

    // ADD man's total action points (10-35)
    iThreatValue += CalcActionPoints(pEnemy);

    // ADD 1/2 of man's current action points (4-17)
    iThreatValue += (pEnemy->bActionPoints / 2);

    // ADD 1/10 of man's current health (0-10)
    iThreatValue += (pEnemy->bLife / 10);

    if (pEnemy->bAssignment < ON_DUTY) {
      // ADD 1/4 of man's protection percentage (0-25)
      iThreatValue += ArmourPercent(pEnemy) / 4;

      // ADD 1/5 of man's marksmanship skill (0-20)
      iThreatValue += (pEnemy->bMarksmanship / 5);

      if (Item[pEnemy->inv[HANDPOS].usItem].usItemClass & IC_WEAPON) {
        // ADD the deadliness of the item(weapon) he's holding (0-50)
        iThreatValue += Weapon[pEnemy->inv[HANDPOS].usItem].ubDeadliness;
      }
    }

    // SUBTRACT 1/5 of man's bleeding (0-20)
    iThreatValue -= (pEnemy->bBleeding / 5);

    // SUBTRACT 1/10 of man's breath deficiency (0-10)
    iThreatValue -= ((100 - pEnemy->bBreath) / 10);

    // SUBTRACT man's current shock value
    // DIGGLER ON 02.12.2010 Шок - серьезная вещь, его вычет слишком мал
    // iThreatValue -= pEnemy->bShock;
    iThreatValue -= pEnemy->bShock * pEnemy->bShock + pEnemy->bShock;
    // DIGGLER OFF
  }

  // if I have a specifically defined spot where I'm at (sometime I don't!)
  if (sMyGrid != NOWHERE) {
    // ADD 10% if man's already been shooting at me
    if (pEnemy->sLastTarget == sMyGrid) {
      iThreatValue += (iThreatValue / 10);
    } else {
      // ADD 5% if man's already facing me
      if (pEnemy->bDirection == atan8(CenterX(pEnemy->sGridNo), CenterY(pEnemy->sGridNo),
                                      CenterX(sMyGrid), CenterY(sMyGrid))) {
        iThreatValue += (iThreatValue / 20);
      }
    }
  }

  // if this man is conscious
  //	if ( pEnemy->bLife >= OKLIFE)
  if (!pEnemy->Unconscious()) {
    // and we were told to reduce threat for my cover
    if (ubReduceForCover && (sMyGrid != NOWHERE)) {
      // Reduce iThreatValue to same % as the chance HE has shoot through at ME
      // iThreatValue = (iThreatValue * ChanceToGetThrough( pEnemy, myGrid, FAKE, ACTUAL, TESTWALLS,
      // 9999, M9PISTOL, NOT_FOR_LOS)) / 100; iThreatValue = (iThreatValue *
      // SoldierTo3DLocationChanceToGetThrough( pEnemy, myGrid, FAKE, ACTUAL, TESTWALLS, 9999,
      // M9PISTOL, NOT_FOR_LOS)) / 100;
      iThreatValue = (iThreatValue * SoldierToLocationChanceToGetThrough(
                                         pEnemy, sMyGrid, pMe->bLevel, 0, pMe->ubID)) /
                     100;
    }
  } else {
    // if he's still something of a threat
    if (iThreatValue > 0) {
      // drastically reduce his threat value (divide by 5 to 18)
      iThreatValue /= (4 + (OKLIFE - pEnemy->bLife));
    }
  }

  // threat value of any opponent can never drop below 1
  if (iThreatValue < 1) {
    iThreatValue = 1;
  }

  // sprintf(tempstr,"%s's iThreatValue = ",pEnemy->name);
  // NumMessage(tempstr,iThreatValue);

#ifdef BETAVERSION  // unnecessary for real release
  // NOTE: maximum is about 200 for a healthy Mike type with a mortar!
  if (iThreatValue > 250) {
    sprintf(tempstr, "CalcManThreatValue: WARNING - %d has a very high threat value of %d",
            pEnemy->ubID, iThreatValue);

#ifdef RECORDNET
    fprintf(NetDebugFile, "\t%s\n", tempstr);
#endif

#ifdef TESTVERSION
    PopMessage(tempstr);
#endif
  }
#endif

  return (iThreatValue);
}

INT32 SOLDIERCLASS::CalcMyThreatValue() { return CalcManThreatValue(this, NOWHERE, FALSE, this); }

//***13.12.2012*** есть ли поблизости атакуемые друзья
BOOLEAN NearAttackedFriends(SOLDIERCLASS *pSoldier) {
  SOLDIERCLASS *pOpponent;
  UINT32 uiLoop;

  for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++) {
    pOpponent = MercSlots[uiLoop];

    if (pOpponent && pOpponent->bSide == pSoldier->bSide && pOpponent->bLife > 0 &&
        (pOpponent->bAlertStatus == STATUS_BLACK || pOpponent->bUnderFire)) {
      if (PythSpacesAway(pSoldier->sGridNo, pOpponent->sGridNo) <= (pSoldier->bExpLevel + 10))
        return (TRUE);
    }
  }

  return (FALSE);
}

INT16 RoamingRange(SOLDIERCLASS *pSoldier, UINT16 *pusFromGridNo) {
  if (CREATURE_OR_BLOODCAT(pSoldier)) {
    if (pSoldier->bAlertStatus == STATUS_BLACK) {
      *pusFromGridNo = pSoldier->sGridNo;  // from current position!
      return (MAX_ROAMING_RANGE);
    }
  }

  if (pSoldier->bOrders == POINTPATROL || pSoldier->bOrders == RNDPTPATROL) {
    // roam near NEXT PATROL POINT, not from where merc starts out
    *pusFromGridNo = pSoldier->usPatrolGrid[pSoldier->bNextPatrolPnt];
  } else {
    // roam around where mercs started
    //*pusFromGridNo = pSoldier->sInitialGridNo;
    *pusFromGridNo = pSoldier->usPatrolGrid[0];
  }

  //***12.12.2012*** при личном или друзей контакте с противником отсчёт от текущей позиции
  //(удлинняем поводок)
  if (pSoldier->bAlertStatus == STATUS_BLACK || pSoldier->bUnderFire ||
      NearAttackedFriends(pSoldier)) {
    if (!(pSoldier->bOrders == STATIONARY &&
          InARoom(pSoldier->sGridNo, NULL)))  //исключая кемперов в зданиях
      *pusFromGridNo = pSoldier->sGridNo;
  }

  switch (pSoldier->bOrders) {
    // JA2 GOLD: give non-NPCs a 5 tile roam range for cover in combat when being shot at
    case STATIONARY:
      if (pSoldier->ubProfile != NO_PROFILE ||
          (pSoldier->bAlertStatus < STATUS_BLACK && !(pSoldier->bUnderFire)))
        return (0);
      else
        return (5);

    case ONGUARD:
      return (5);

    case CLOSEPATROL:
      return (15);

    case RNDPTPATROL:
    case POINTPATROL:
      return (10);  // from nextPatrolGrid, not whereIWas

    case FARPATROL:
      if (pSoldier->bAlertStatus < STATUS_RED)
        return (25);
      else
        return (50);

    case ONCALL:
      if (pSoldier->bAlertStatus < STATUS_RED)
        return (10);
      else
        return (MAX_ROAMING_RANGE /*30*/);  //***12.12.2012*** увеличиваем радиус на всю карту

    case SEEKENEMY:
      *pusFromGridNo = pSoldier->sGridNo;  // from current position!
      return (MAX_ROAMING_RANGE);

    default:
#ifdef BETAVERSION
      sprintf(tempstr, "%s has invalid orders = %d", pSoldier->name, pSoldier->bOrders);
      PopMessage(tempstr);
#endif
      return (0);
  }
}

void RearrangePocket(SOLDIERCLASS *pSoldier, INT8 bPocket1, INT8 bPocket2, UINT8 bPermanent) {
  // NB there's no such thing as a temporary swap for now...
  SwapObjs(&(pSoldier->inv[bPocket1]), &(pSoldier->inv[bPocket2]));
}

BOOLEAN FindBetterSpotForItem(SOLDIERCLASS *pSoldier, INT8 bSlot) {
  // looks for a place in the slots to put an item in a hand or armour
  // position, and moves it there.
  if (bSlot >= BIGPOCK1POS) {
    return (FALSE);
  }
  if (pSoldier->inv[bSlot].usItem == NOTHING) {
    // well that's just fine then!
    return (TRUE);
  }

  if (Item[pSoldier->inv[bSlot].usItem].ubPerPocket == 0) {
    // then we're looking for a big pocket
    bSlot = FindEmptySlotWithin(pSoldier, BIGPOCK1POS, BIGPOCK4POS);
  } else {
    // try a small pocket first
    bSlot = FindEmptySlotWithin(pSoldier, SMALLPOCK1POS, SMALLPOCK8POS);
    if (bSlot == NO_SLOT) {
      bSlot = FindEmptySlotWithin(pSoldier, BIGPOCK1POS, BIGPOCK4POS);
    }
  }
  if (bSlot == NO_SLOT) {
    return (FALSE);
  }
  RearrangePocket(pSoldier, HANDPOS, bSlot, FOREVER);
  return (TRUE);
}

UINT8 GetTraversalQuoteActionID(INT8 bDirection) {
  switch (bDirection) {
    case NORTHEAST:  // east
      return (QUOTE_ACTION_ID_TRAVERSE_EAST);

    case SOUTHEAST:  // south
      return (QUOTE_ACTION_ID_TRAVERSE_SOUTH);

    case SOUTHWEST:  // west
      return (QUOTE_ACTION_ID_TRAVERSE_WEST);

    case NORTHWEST:  // north
      return (QUOTE_ACTION_ID_TRAVERSE_NORTH);

    default:
      return (0);
  }
}

UINT8 SoldierDifficultyLevel(SOLDIERCLASS *pSoldier) {
  INT8 bDifficultyBase;
  INT8 bDifficulty;

  // difficulty modifier ranges from 0 to 100
  // and we want to end up with a number between 0 and 4 (4=hardest)
  // to a base of 1, divide by 34 to get a range from 1 to 3
  bDifficultyBase = 1 + (CalcDifficultyModifier(pSoldier->ubSoldierClass) / 34);

  switch (pSoldier->ubSoldierClass) {
    case SOLDIER_CLASS_ADMINISTRATOR:
      bDifficulty = bDifficultyBase - 1;
      break;

    case SOLDIER_CLASS_ARMY:
      bDifficulty = bDifficultyBase;
      break;

    case SOLDIER_CLASS_ELITE:
      bDifficulty = bDifficultyBase + 1;
      break;

    // hard code militia;
    case SOLDIER_CLASS_GREEN_MILITIA:
      bDifficulty = 2;
      break;

    case SOLDIER_CLASS_REG_MILITIA:
      bDifficulty = 3;
      break;

    case SOLDIER_CLASS_ELITE_MILITIA:
      bDifficulty = 4;
      break;

    default:
      if (pSoldier->bTeam == CREATURE_TEAM) {
        bDifficulty = bDifficultyBase + pSoldier->bLevel / 4;
      } else  // civ...
      {
        bDifficulty = (bDifficultyBase + pSoldier->bLevel / 4) - 1;
      }
      break;
  }

  bDifficulty = __max(bDifficulty, 0);
  bDifficulty = __min(bDifficulty, 4);

  return ((UINT8)bDifficulty);
}

BOOLEAN ValidCreatureTurn(SOLDIERCLASS *pCreature, INT8 bNewDirection) {
  INT8 bDirChange;
  INT8 bTempDir;
  INT8 bLoop;
  BOOLEAN fFound;

  bDirChange = (INT8)QuickestDirection(pCreature->bDirection, bNewDirection);

  for (bLoop = 0; bLoop < 2; bLoop++) {
    fFound = TRUE;

    bTempDir = pCreature->bDirection;

    do {
      bTempDir += bDirChange;
      if (bTempDir < NORTH) {
        bTempDir = NORTHWEST;
      } else if (bTempDir > NORTHWEST) {
        bTempDir = NORTH;
      }
      if (!InternalIsValidStance(pCreature, bTempDir, ANIM_STAND)) {
        fFound = FALSE;
        break;
      }

    } while (bTempDir != bNewDirection);

    if (fFound) {
      break;
    } else if (bLoop > 0) {
      // can't find a dir!
      return (FALSE);
    } else {
      // try the other direction
      bDirChange = bDirChange * -1;
    }
  }

  return (TRUE);
}

INT32 RangeChangeDesire(SOLDIERCLASS *pSoldier) {
  INT32 iRangeFactorMultiplier;

  iRangeFactorMultiplier = pSoldier->bAIMorale - 1;
  switch (pSoldier->bAttitude) {
    case DEFENSIVE:
      iRangeFactorMultiplier += -1;
      break;
    case BRAVESOLO:
      iRangeFactorMultiplier += 2;
      break;
    case BRAVEAID:
      iRangeFactorMultiplier += 2;
      break;
    case CUNNINGSOLO:
      iRangeFactorMultiplier += 0;
      break;
    case CUNNINGAID:
      iRangeFactorMultiplier += 0;
      break;
    case ATTACKSLAYONLY:
    case AGGRESSIVE:
      iRangeFactorMultiplier += 1;
      break;
  }
  if (gTacticalStatus.bConsNumTurnsWeHaventSeenButEnemyDoes > 0) {
    iRangeFactorMultiplier += gTacticalStatus.bConsNumTurnsWeHaventSeenButEnemyDoes;
  }
  return (iRangeFactorMultiplier);
}

BOOLEAN ArmySeesOpponents(void) {
  INT32 cnt;
  SOLDIERCLASS *pSoldier;

  for (cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;
       cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; cnt++) {
    pSoldier = MercPtrs[cnt];

    if (pSoldier->bActive && pSoldier->bInSector && pSoldier->bLife >= OKLIFE &&
        pSoldier->bOppCnt > 0) {
      return (TRUE);
    }
  }

  return (FALSE);
}

#ifdef DEBUGDECISIONS
void AIPopMessage(STR str) { DebugAI(str); }

void AINumMessage(const STR str, INT32 num) {
  CHAR8 tempstr[500];
  sprintf(tempstr, "%s %d", str, num);
  DebugAI(tempstr);
}

void AINameMessage(SOLDIERCLASS *pSoldier, const STR str, INT32 num) {
  // STR tempstr;
  sprintf(tempstr, "ID %d %s %d", pSoldier->ubID, str, num);
  DebugAI(tempstr);
}
#endif
