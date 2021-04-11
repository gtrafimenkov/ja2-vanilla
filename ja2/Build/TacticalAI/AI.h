#ifndef __AI_H
#define __AI_H

#include "SGP/Types.h"
#include "TileEngine/WorldDef.h"
#include "Tactical/SoldierControl.h"

#define TESTAICONTROL

#define OPPONENT_UNCONSCIOUS(op) ((op->bLife < OKLIFE) && !op->bService)

extern INT8 gubAIPathCosts[19][19];

#define AI_PATHCOST_RADIUS 9

extern BOOLEAN gfDisplayCoverValues;
extern INT16 gsCoverValue[WORLD_MAX];

// AI actions

extern enum {
  CALL_NONE = 0,
  CALL_1_PREY,
  CALL_MULTIPLE_PREY,
  CALL_ATTACKED,
  CALL_CRIPPLED,
  NUM_CREATURE_CALLS
} CreatureCalls;

#define DONTFORCE 0
#define FORCE 1

// ANY NEW ACTIONS ADDED - UPDATE OVERHEAD.C ARRAY WITH ACTION'S STRING VALUE
#define FIRST_MOVEMENT_ACTION AI_ACTION_RANDOM_PATROL
#define LAST_MOVEMENT_ACTION AI_ACTION_MOVE_TO_CLIMB
typedef enum {
  AI_ACTION_NONE = 0,  // maintain current position & facing

  // actions that involve a move to another tile
  AI_ACTION_RANDOM_PATROL,  // move towards a random destination
  AI_ACTION_SEEK_FRIEND,    // move towards friend in trouble
  AI_ACTION_SEEK_OPPONENT,  // move towards a reported opponent
  AI_ACTION_TAKE_COVER,     // run for nearest cover from threat
  AI_ACTION_GET_CLOSER,     // move closer to a strategic location

  AI_ACTION_POINT_PATROL,     // move towards next patrol point
  AI_ACTION_LEAVE_WATER_GAS,  // seek nearest spot of ungassed land
  AI_ACTION_SEEK_NOISE,       // seek most important noise heard
  AI_ACTION_ESCORTED_MOVE,    // go where told to by escortPlayer
  AI_ACTION_RUN_AWAY,         // run away from nearby opponent(s)

  AI_ACTION_KNIFE_MOVE,     // preparing to stab an opponent
  AI_ACTION_APPROACH_MERC,  // move up to a merc in order to talk with them; RT
  AI_ACTION_TRACK,          // track a scent
  AI_ACTION_EAT,            // monster eats corpse
  AI_ACTION_PICKUP_ITEM,    // grab things lying on the ground

  AI_ACTION_SCHEDULE_MOVE,  // move according to schedule
  AI_ACTION_WALK,           // walk somewhere (NPC stuff etc)
  AI_ACTION_RUN,            // run somewhere (NPC stuff etc)
  AI_ACTION_MOVE_TO_CLIMB,  // move to edge of roof/building
  // miscellaneous movement actions
  AI_ACTION_CHANGE_FACING,  // turn to face a different direction

  AI_ACTION_CHANGE_STANCE,  // stand, crouch, or go prone
  // actions related to items and attacks
  AI_ACTION_YELLOW_ALERT,   // tell friends opponent(s) heard
  AI_ACTION_RED_ALERT,      // tell friends opponent(s) seen
  AI_ACTION_CREATURE_CALL,  // creature communication
  AI_ACTION_PULL_TRIGGER,   // go off to activate a panic trigger

  AI_ACTION_USE_DETONATOR,    // grab detonator and set off bomb(s)
  AI_ACTION_FIRE_GUN,         // shoot at nearby opponent
  AI_ACTION_TOSS_PROJECTILE,  // throw grenade at/near opponent(s)
  AI_ACTION_KNIFE_STAB,       // during the actual knifing attack
  AI_ACTION_THROW_KNIFE,      // throw a knife

  AI_ACTION_GIVE_AID,        // help injured/dying friend
  AI_ACTION_WAIT,            // RT: don't do anything for a certain length of time
  AI_ACTION_PENDING_ACTION,  // RT: wait for pending action (pickup, door open, etc) to finish
  AI_ACTION_DROP_ITEM,       // duh
  AI_ACTION_COWER,           // for civilians:  cower in fear and stay there!

  AI_ACTION_STOP_COWERING,       // stop cowering
  AI_ACTION_OPEN_OR_CLOSE_DOOR,  // schedule-provoked; open or close door
  AI_ACTION_UNLOCK_DOOR,         // schedule-provoked; unlock door (don't open)
  AI_ACTION_LOCK_DOOR,           // schedule-provoked; lock door (close if necessary)
  AI_ACTION_LOWER_GUN,           // lower gun prior to throwing knife

  AI_ACTION_ABSOLUTELY_NONE,     // like "none" but can't be converted to a wait by realtime
  AI_ACTION_CLIMB_ROOF,          // climb up or down roof
  AI_ACTION_END_TURN,            // end turn (after final stance change)
  AI_ACTION_END_COWER_AND_MOVE,  // sort of dummy value, special for civilians who are to go
                                 // somewhere at end of battle
  AI_ACTION_TRAVERSE_DOWN,       // move down a level
  AI_ACTION_OFFER_SURRENDER,     // offer surrender to the player
                                 // DIGGLER ON 22.11.2010
// Вводим доп. действия - надо для доработки структуры ИИ
#ifdef NEW_AI_STRUCT
  // для промежуточных решений. При таких решениях не делается return,
  // продолжается обработка функции
  AI_ACTION_NOT_DECIDED_YET,
#endif NEW_AI_STRUCT
  // DIGGLER OFF
} ActionType;

extern enum {
  QUOTE_ACTION_ID_CHECKFORDEST = 1,
  QUOTE_ACTION_ID_TURNTOWARDSPLAYER,
  QUOTE_ACTION_ID_DRAWGUN,
  QUOTE_ACTION_ID_LOWERGUN,
  QUOTE_ACTION_ID_TRAVERSE_EAST,
  QUOTE_ACTION_ID_TRAVERSE_SOUTH,
  QUOTE_ACTION_ID_TRAVERSE_WEST,
  QUOTE_ACTION_ID_TRAVERSE_NORTH,
} QuoteActionType;

#define RTP_COMBAT_AGGRESSIVE 1
#define RTP_COMBAT_CONSERVE 2
#define RTP_COMBAT_REFRAIN 3

// NB THESE THREE FLAGS SHOULD BE REMOVED FROM CODE
#define AI_RTP_OPTION_CAN_RETREAT 0x01
#define AI_RTP_OPTION_CAN_SEEK_COVER 0x02
#define AI_RTP_OPTION_CAN_HELP 0x04

#define AI_CAUTIOUS 0x08
#define AI_HANDLE_EVERY_FRAME 0x10
#define AI_ASLEEP 0x20
#define AI_LOCK_DOOR_INCLUDES_CLOSE 0x40
#define AI_CHECK_SCHEDULE 0x80

#define NOT_NEW_SITUATION 0
#define WAS_NEW_SITUATION 1
#define IS_NEW_SITUATION 2

#define DIFF_ENEMY_EQUIP_MOD 0
#define DIFF_ENEMY_TO_HIT_MOD 1
#define DIFF_ENEMY_INTERRUPT_MOD 2
#define DIFF_RADIO_RED_ALERT 3
#define DIFF_MAX_COVER_RANGE 4
#define MAX_DIFF_PARMS 5  // how many different difficulty variables?

extern INT8 gbDiff[MAX_DIFF_PARMS][5];

void ActionDone(SOLDIERCLASS *pSoldier);
INT16 ActionInProgress(SOLDIERCLASS *pSoldier);

INT8 CalcMorale(SOLDIERCLASS *pSoldier);
INT32 CalcPercentBetter(INT32 iOldValue, INT32 iNewValue, INT32 iOldScale, INT32 iNewScale);
void CallAvailableEnemiesTo(INT16 sGridno);
void CallAvailableKingpinMenTo(INT16 sGridNo);
void CallAvailableTeamEnemiesTo(INT16 sGridno, INT8 bTeam);
void CallEldinTo(INT16 sGridNo);
void CancelAIAction(SOLDIERCLASS *pSoldier, UINT8 ubForce);
void CheckForChangingOrders(SOLDIERCLASS *pSoldier);

INT8 ClosestPanicTrigger(SOLDIERCLASS *pSoldier);

INT16 ClosestKnownOpponent(SOLDIERCLASS *pSoldier, INT16 *psGridNo, INT8 *pbLevel);
INT16 ClosestPC(SOLDIERCLASS *pSoldier, INT16 *psDistance);
BOOLEAN CanAutoBandage(BOOLEAN fDoFullCheck);

void DebugAI(STR szOutput);

INT8 DecideActionEscort(SOLDIERCLASS *pSoldier);

// DIGGLER ON 20.11.2010 вводим новую структуру ИИ
#ifndef NEW_AI_STRUCT
// далее оригинальные объявления. Важно - под переделку попали пока не все функции, поэтому
INT8 DecideAction(SOLDIERCLASS *pSoldier);
INT8 DecideActionBlack(SOLDIERCLASS *pSoldier);
INT8 DecideActionGreen(SOLDIERCLASS *pSoldier);
INT8 DecideActionRed(SOLDIERCLASS *pSoldier, UINT8 ubUnconsciousOK);
INT8 DecideActionYellow(SOLDIERCLASS *pSoldier);
#else  // теперь новые
INT8 DecideActionRealtime(SOLDIERCLASS *pSoldier);
INT8 DecideActionTurnBased(SOLDIERCLASS *pSoldier);
INT8 DecideActionBlackRT(SOLDIERCLASS *pSoldier);
INT8 DecideActionBlackTB(SOLDIERCLASS *pSoldier);
INT8 DecideActionGreenRT(SOLDIERCLASS *pSoldier);
INT8 DecideActionGreenTB(SOLDIERCLASS *pSoldier);
INT8 DecideActionRedRT(SOLDIERCLASS *pSoldier, UINT8 ubUnconsciousOK);
INT8 DecideActionRedTB(SOLDIERCLASS *pSoldier, UINT8 ubUnconsciousOK);
INT8 DecideActionYellowRT(SOLDIERCLASS *pSoldier);
INT8 DecideActionYellowTB(SOLDIERCLASS *pSoldier);
INT8 CowerInFearAction(SOLDIERCLASS *pSoldier);
#endif
// DIGGLER OFF

INT16 DistanceToClosestFriend(SOLDIERCLASS *pSoldier);

void EndAIDeadlock(void);
void EndAIGuysTurn(SOLDIERCLASS *pSoldier);

INT8 ExecuteAction(SOLDIERCLASS *pSoldier);

INT16 FindAdjacentSpotBeside(SOLDIERCLASS *pSoldier, INT16 sGridno);
INT16 FindBestNearbyCover(SOLDIERCLASS *pSoldier, INT32 morale, INT32 *pPercentBetter);
INT16 FindClosestDoor(SOLDIERCLASS *pSoldier);
INT16 FindNearbyPointOnEdgeOfMap(SOLDIERCLASS *pSoldier, INT8 *pbDirection);
INT16 FindNearestEdgePoint(INT16 sGridNo);

// Kris:  Added these as I need specific searches on certain sides.
enum {
  NORTH_EDGEPOINT_SEARCH,
  EAST_EDGEPOINT_SEARCH,
  SOUTH_EDGEPOINT_SEARCH,
  WEST_EDGEPOINT_SEARCH,
};
INT16 FindNearestEdgepointOnSpecifiedEdge(INT16 sGridNo, INT8 bEdgeCode);

INT16 FindNearestUngassedLand(SOLDIERCLASS *pSoldier);
BOOLEAN FindRoofClimbingPoints(SOLDIERCLASS *pSoldier, INT16 sDesiredSpot);
INT16 FindSpotMaxDistFromOpponents(SOLDIERCLASS *pSoldier);
INT16 FindSweetCoverSpot(SOLDIERCLASS *pSoldier);

void FreeUpNPCFromAttacking(UINT8 ubID);
void FreeUpNPCFromPendingAction(SOLDIERCLASS *pSoldier);
void FreeUpNPCFromTurning(SOLDIERCLASS *pSoldier, INT8 bLook);
void FreeUpNPCFromStanceChange(SOLDIERCLASS *pSoldier);
void FreeUpNPCFromLoweringGun(SOLDIERCLASS *pSoldier);
void FreeUpNPCFromRoofClimb(SOLDIERCLASS *pSoldier);

UINT8 GetClosestOpponent(SOLDIERCLASS *pSoldier);
UINT8 GetMostThreateningOpponent(SOLDIERCLASS *pSoldier);

void HandleSoldierAI(SOLDIERCLASS *pSoldier);
void HandleInitialRedAlert(INT8 bTeam, UINT8 ubCommunicate);

void InitPanicSystem();
INT16 InWaterOrGas(SOLDIERCLASS *pSoldier, INT16 sGridno);
BOOLEAN IsActionAffordable(SOLDIERCLASS *pSoldier);
BOOLEAN InitAI(void);

void MakeClosestEnemyChosenOne();
void ManChecksOnFriends(SOLDIERCLASS *pSoldier);

void NewDest(SOLDIERCLASS *pSoldier, UINT16 sGridno);
INT16 NextPatrolPoint(SOLDIERCLASS *pSoldier);

INT8 PanicAI(SOLDIERCLASS *pSoldier, UINT8 ubCanMove);
void HaltMoveForSoldierOutOfPoints(SOLDIERCLASS *pSoldier);

INT16 RandDestWithinRange(SOLDIERCLASS *pSoldier);
INT16 RandomFriendWithin(SOLDIERCLASS *pSoldier);
INT16 RoamingRange(SOLDIERCLASS *pSoldier, UINT16 *pusFromGridno);

void SetCivilianDestination(UINT8 ubWho, INT16 sGridno);
void SetNewSituation(SOLDIERCLASS *pSoldier);

UINT8 SoldierDifficultyLevel(SOLDIERCLASS *pSoldier);
void SoldierTriesToContinueAlongPath(SOLDIERCLASS *pSoldier);
void StartNPCAI(SOLDIERCLASS *pSoldier);
void TempHurt(SOLDIERCLASS *pVictim, SOLDIERCLASS *pAttacker);
int TryToResumeMovement(SOLDIERCLASS *pSoldier, INT16 sGridno);

BOOLEAN ValidCreatureTurn(SOLDIERCLASS *pCreature, INT8 bNewDirection);

BOOLEAN WearGasMaskIfAvailable(SOLDIERCLASS *pSoldier);
INT16 WhatIKnowThatPublicDont(SOLDIERCLASS *pSoldier, UINT8 ubInSightOnly);

#ifdef DEBUGDECISIONS
extern char tempstr[256];
void AIPopMessage(STR str);

void AINumMessage(const STR str, INT32 num);

void AINameMessage(SOLDIERCLASS *pSoldier, const STR str, INT32 num);

#endif

#endif
