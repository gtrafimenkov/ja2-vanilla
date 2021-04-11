#include "Tactical/TacticalAll.h"
#ifdef PRECOMPILEDHEADERS
#else
#include "SGP/SGP.h"
#include "Strategic/GameClock.h"
#include "Utils/FontControl.h"
#include "TileEngine/RenderDirty.h"
#include "Utils/TimerControl.h"
#include "Tactical/Overhead.h"
#include "TileEngine/Environment.h"
#include "Strategic/GameClock.h"
#include "Utils/Message.h"
#include "TileEngine/WorldDef.h"
#include "Tactical/RottingCorpses.h"
#include "Tactical/SoldierCreate.h"
#include "Tactical/SoldierAdd.h"
#include "Strategic/StrategicTurns.h"
#include "TileEngine/IsometricUtils.h"
#include "Tactical/AnimationData.h"
#include "Tactical/AnimationControl.h"
#include "Tactical/TacticalTurns.h"
#include "Tactical/Points.h"
#include "TileEngine/Smell.h"
#include "Tactical/OppList.h"
#include "Strategic/QueenCommand.h"
#include "Tactical/DialogueControl.h"
#include "TileEngine/SmokeEffects.h"
#include "TileEngine/LightEffects.h"
#include "Tactical/Campaign.h"
#include "Tactical/SoldierMacros.h"
#endif

extern void DecayPublicOpplist(INT8 bTeam);

// not in overhead.h!
extern UINT8 NumEnemyInSector();

void HandleRPCDescription() {
  UINT8 ubMercsInSector[20] = {0};
  UINT8 ubNumMercs = 0;
  UINT8 ubChosenMerc;
  SOLDIERCLASS *pTeamSoldier;
  INT32 cnt2;
  BOOLEAN fSAMSite = FALSE;

  if (!gTacticalStatus.fCountingDownForGuideDescription) {
    return;
  }

  // ATE: postpone if we are not in tactical
  if (guiCurrentScreen != GAME_SCREEN) {
    return;
  }

  if ((gTacticalStatus.uiFlags & ENGAGED_IN_CONV)) {
    return;
  }

  // Are we a SAM site?
  if (gTacticalStatus.ubGuideDescriptionToUse == 27 ||
      gTacticalStatus.ubGuideDescriptionToUse == 30 ||
      gTacticalStatus.ubGuideDescriptionToUse == 32 ||
      gTacticalStatus.ubGuideDescriptionToUse == 25 ||
      gTacticalStatus.ubGuideDescriptionToUse == 31) {
    fSAMSite = TRUE;
    gTacticalStatus.bGuideDescriptionCountDown = 1;
  }

  // ATE; Don't do in combat
  if ((gTacticalStatus.uiFlags & INCOMBAT) && !fSAMSite) {
    return;
  }

  // Don't do if enemy in sector
  if (NumEnemyInSector() && !fSAMSite) {
    return;
  }

  gTacticalStatus.bGuideDescriptionCountDown--;

  if (gTacticalStatus.bGuideDescriptionCountDown == 0) {
    gTacticalStatus.fCountingDownForGuideDescription = FALSE;

    // OK, count how many rpc guys we have....
    // set up soldier ptr as first element in mercptrs list
    cnt2 = gTacticalStatus.Team[PLAYER_TEAM].bFirstID;

    // run through list
    for (pTeamSoldier = MercPtrs[cnt2]; cnt2 <= gTacticalStatus.Team[PLAYER_TEAM].bLastID;
         cnt2++, pTeamSoldier++) {
      // Add guy if he's a candidate...
      if (RPC_RECRUITED(pTeamSoldier)) {
        if (pTeamSoldier->bLife >= OKLIFE && pTeamSoldier->bActive &&
            pTeamSoldier->sSectorX == gTacticalStatus.bGuideDescriptionSectorX &&
            pTeamSoldier->sSectorY == gTacticalStatus.bGuideDescriptionSectorY &&
            pTeamSoldier->bSectorZ == gbWorldSectorZ && !pTeamSoldier->fBetweenSectors) {
          if (pTeamSoldier->ubProfile == IRA || pTeamSoldier->ubProfile == MIGUEL ||
              pTeamSoldier->ubProfile == CARLOS || pTeamSoldier->ubProfile == DIMITRI) {
            ubMercsInSector[ubNumMercs] = (UINT8)cnt2;
            ubNumMercs++;
          }
        }
      }
    }

    // If we are > 0
    if (ubNumMercs > 0) {
      ubChosenMerc = (UINT8)Random(ubNumMercs);

      TacticalCharacterDialogueWithSpecialEvent(MercPtrs[ubMercsInSector[ubChosenMerc]],
                                                gTacticalStatus.ubGuideDescriptionToUse,
                                                DIALOGUE_SPECIAL_EVENT_USE_ALTERNATE_FILES, 0, 0);
    }
  }
}

extern void CalcPlayerSeeGridNo(void);
extern void DecreaseBattery(SOLDIERCLASS *pSoldier);
void HandleTacticalEndTurn() {
  UINT32 cnt;
  SOLDIERCLASS *pSoldier;
  UINT32 uiTime;
  static UINT32 uiTimeSinceLastStrategicUpdate = 0;

  static UINT32 iUpdateCounter = 0;

  //***15.12.2009*** батарейное питание у НП, ПНВ и усилителя звуков
  if (guiCurrentScreen == GAME_SCREEN) {
    //***12.11.2009*** заполнение массива просматриваемости тайлов сектора командой игрока в
    //реалтайме
    if (gTacticalStatus.fEnemyInSector) CalcPlayerSeeGridNo();

    iUpdateCounter++;  // 1 тик = 5 секунд
    if (iUpdateCounter >= 12) {
      cnt = gTacticalStatus.Team[PLAYER_TEAM].bFirstID;
      for (pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[PLAYER_TEAM].bLastID;
           cnt++, pSoldier++) {
        if (pSoldier->bActive && pSoldier->bLife > 0 &&
            !(pSoldier->uiStatusFlags & SOLDIER_VEHICLE)) {
          DecreaseBattery(pSoldier);
        }
      }
      iUpdateCounter = 0;
    }
  }

  // OK, Do a number of things here....
  // Every few turns......

  // Get time elasped
  uiTime = GetWorldTotalSeconds();

  if ((uiTimeSinceLastStrategicUpdate - uiTime) > 1200) {
    HandleRottingCorpses();
    // DecayTacticalMoraleModifiers();

    uiTimeSinceLastStrategicUpdate = uiTime;
  }

  DecayBombTimers();

  DecaySmokeEffects(uiTime);

  DecayLightEffects(uiTime);

  // Decay smells
  // DecaySmells();

  // Decay blood
  DecayBloodAndSmells(uiTime);

  // decay AI warning values from corpses
  DecayRottingCorpseAIWarnings();

  // Check for enemy pooling (add enemies if there happens to be more than the max in the
  // current battle.  If one or more slots have freed up, we can add them now.
  AddPossiblePendingEnemiesToBattle();

  // Loop through each active team and decay public opplist...
  // May want this done every few times too
  NonCombatDecayPublicOpplist(uiTime);
  /*
  for( cnt = 0; cnt < MAXTEAMS; cnt++ )
  {
          if ( gTacticalStatus.Team[ cnt ].bMenInSector > 0 )
          {
                  // decay team's public opplist
                  DecayPublicOpplist( (INT8)cnt );
          }
  }
*/

  // First pass:
  // Loop through our own mercs:
  //	Check things like ( even if not in our sector )
  //		1 ) All updates of breath, shock, bleeding, etc
  //    2 ) Updating First AID, etc
  //  ( If in our sector: )
  //		3 ) Update things like decayed opplist, etc

  // Second pass:
  //  Loop through all mercs in tactical engine
  //  If not a player merc ( ubTeam ) , do things like 1 , 2 , 3 above

  // First exit if we are not in realtime combat or realtime noncombat
  if (!(gTacticalStatus.uiFlags & TURNBASED) || !(gTacticalStatus.uiFlags & INCOMBAT)) {
    BeginLoggingForBleedMeToos(TRUE);

    cnt = gTacticalStatus.Team[PLAYER_TEAM].bFirstID;
    for (pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[PLAYER_TEAM].bLastID;
         cnt++, pSoldier++) {
      if (pSoldier->bActive && pSoldier->bLife > 0 &&
          !(pSoldier->uiStatusFlags & SOLDIER_VEHICLE) && !(AM_A_ROBOT(pSoldier))) {
        // Handle everything from getting breath back, to bleeding, etc
        EVENT_BeginMercTurn(pSoldier, TRUE, 0);

        // Handle Player services
        HandlePlayerServices(pSoldier);

        // if time is up, turn off xray
        if (pSoldier->uiXRayActivatedTime && uiTime > pSoldier->uiXRayActivatedTime + XRAY_TIME) {
          TurnOffXRayEffects(pSoldier);
        }

        // Handle stat changes if ness.
        // if ( fCheckStats )
        //{
        ////	UpdateStats( pSoldier );
        //}
      }
    }

    BeginLoggingForBleedMeToos(FALSE);

    // OK, loop through the mercs to perform 'end turn' events on each...
    // We're looping through only mercs in tactical engine, ignoring our mercs
    // because they were done earilier...
    for (cnt = 0; cnt < guiNumMercSlots; cnt++) {
      pSoldier = MercSlots[cnt];

      if (pSoldier != NULL) {
        if (pSoldier->bTeam != PLAYER_TEAM) {
          // Handle everything from getting breath back, to bleeding, etc
          EVENT_BeginMercTurn(pSoldier, TRUE, 0);

          // Handle Player services
          HandlePlayerServices(pSoldier);
        }
      }
    }
  }

  HandleRPCDescription();
}
