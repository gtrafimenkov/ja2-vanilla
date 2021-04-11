#include "SGP/Types.h"
#include "TileEngine/IsometricUtils.h"
#include "Tactical/AnimationControl.h"
#include "Tactical/Overhead.h"
#include "SGP/Random.h"
#include "Tactical/Points.h"

extern BOOLEAN gfTurnBasedAI;

// THIS IS AN ITEM #  - AND FOR NOW JUST COMPLETELY FAKE...

#define MAX_TOSS_SEARCH_DIST 1    // must throw within this of opponent
#define NPC_TOSS_SAFETY_MARGIN 4  // all friends must be this far away

#define ACTING_ON_SCHEDULE(p) ((p)->fAIFlags & AI_CHECK_SCHEDULE)

// the AI should try to have this many APs before climbing a roof, if possible
#define AI_AP_CLIMBROOF 15

#define TEMPORARILY 0
#define FOREVER 1

#define IGNORE_PATH 0
#define ENSURE_PATH 1
#define ENSURE_PATH_COST 2

// Kris:  November 10, 1997
// Please don't change this value from 10.  It will invalidate all of the maps and soldiers.
#define MAXPATROLGRIDS 10

#define NOWATER 0
#define WATEROK 1

#define DONTADDTURNCOST 0
#define ADDTURNCOST 1

enum { URGENCY_LOW = 0, URGENCY_MED, URGENCY_HIGH, NUM_URGENCY_STATES };

#define NOWATER 0
#define WATEROK 1

#define IGNORE_PATH 0
#define ENSURE_PATH 1
#define ENSURE_PATH_COST 2

#define DONTFORCE 0
#define FORCE 1

#define MAX_ROAMING_RANGE WORLD_COLS

#define PTR_CIV_OR_MILITIA (PTR_CIVILIAN || (pSoldier->bTeam == MILITIA_TEAM))

#define REALTIME_AI_DELAY (10000 + Random(1000))
#define REALTIME_CIV_AI_DELAY                                \
  (1000 * (gTacticalStatus.Team[MILITIA_TEAM].bMenInSector + \
           gTacticalStatus.Team[CIV_TEAM].bMenInSector) +    \
   5000 + 2000 * Random(3))
#define REALTIME_CREATURE_AI_DELAY (10000 + 1000 * Random(3))

//#define PLAYINGMODE             0
//#define CAMPAIGNLENGTH          1
//#define LASTUSABLESLOT          2
//#define RANDOMMERCS             3
//#define AVAILABLEMERCS          4
//#define HIRINGKNOWLEDGE         5
//#define EQUIPMENTLEVEL          6
//#define ENEMYTEAMSIZE           7
#define ENEMYDIFFICULTY 8  // this is being used in this module
//#define FOG_OF_WAR              9
//#define TURNLENGTH              10
//#define INCREASEDAP             11
//#define BLOODSTAINS             12
//#define STARTINGBALANCE         13
#define MAXGAMEOPTIONS 14

#define NOSHOOT_WAITABIT -1
#define NOSHOOT_WATER -2
#define NOSHOOT_MYSELF -3
#define NOSHOOT_HURT -4
#define NOSHOOT_NOAMMO -5
#define NOSHOOT_NOLOAD -6
#define NOSHOOT_NOWEAPON -7

// DIGGLER ON 29.11.2010 Оригинальное значение великовато. Переключаться на новые цели надо бы
// поохотней.
#ifndef AI_EXP1
#define PERCENT_TO_IGNORE_THREAT 50  // any less, use threat value
#else
#define PERCENT_TO_IGNORE_THREAT \
  35  // При выборе ИИ цели, если шанс попасть в новую будет больше шанса попасть в старую на это
      // значение, новая цель будет взята за основную без размышлений.
#endif

// моя константа. няхай буде, так код проще комментировать
#define MIN_CHANCE_TO_HIT_WHEN_CALC_BEST_SHOT 2  // значение от Терапевта

#ifdef AI_EXP1
#undef MIN_CHANCE_TO_HIT_WHEN_CALC_BEST_SHOT
#define MIN_CHANCE_TO_HIT_WHEN_CALC_BEST_SHOT 7  // значение от Dirk Diggler
#endif

// DIGGLER OFF

#define ACTION_TIMEOUT_CYCLES 50  // # failed cycles through AI
#define MAX_THREAT_RANGE 400      // 30 tiles worth
#define MIN_PERCENT_BETTER 5      // 5% improvement in cover is good

#define TOSSES_PER_10TURNS 18  // max # of grenades tossable in 10 turns
#define SHELLS_PER_10TURNS 13  // max # of shells   firable  in 10 turns

#define SEE_THRU_COVER_THRESHOLD 5  // min chance to get through

#undef min
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef max
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
  SOLDIERCLASS *pOpponent;
  INT16 sGridNo;
  INT32 iValue;
  INT32 iAPs;
  INT32 iCertainty;
  INT32 iOrigRange;
} THREATTYPE;

// define for bAimTime for bursting
#define BURSTING 5

class ATTACKCLASS {
 public:
  UINT8 ubPossible;           // is this attack form possible?  T/F
  UINT8 ubOpponent;           // which soldier is the victim?
  UINT8 ubAimTime;            // how many extra APs to spend on aiming
  UINT8 ubChanceToReallyHit;  // chance to hit * chance to get through cover
  INT32 iAttackValue;         // relative worthiness of this type of attack
  INT16 sTarget;              // target gridno of this attack
  INT8 bTargetLevel;          // target level of this attack
  UINT8 ubAPCost;             // how many APs the attack will use up
  INT8 bWeaponIn;             // the inv slot of the weapon in question
  // DIGGLER ON 30.11.2010
  UINT8 ubAimLocation;  // Куда атака направлена, например AIM_SHOT_TORSO
  INT16 sAttackFromGridNo;  // Откуда атака исходит. Актуально для атак ножом
  ATTACKCLASS() {
    ubPossible = FALSE;
    ubOpponent = NULL;
  }
  // DIGGLER OFF
};

extern THREATTYPE Threat[MAXMERCS];
extern int ThreatPercent[10];
extern UINT8 SkipCoverCheck;
extern INT8 GameOption[MAXGAMEOPTIONS];

typedef enum { SEARCH_GENERAL_ITEMS, SEARCH_AMMO, SEARCH_WEAPONS } ItemSearchReasons;

// go as far as possible flags
#define FLAG_CAUTIOUS 0x01
#define FLAG_STOPSHORT 0x02

#define STOPSHORTDIST 5

INT16 AdvanceToFiringRange(SOLDIERCLASS *pSoldier, INT16 sClosestOpponent);

BOOLEAN AimingGun(SOLDIERCLASS *pSoldier);
void CalcBestShot(SOLDIERCLASS *pSoldier, ATTACKCLASS *pBestShot);
void CalcBestStab(SOLDIERCLASS *pSoldier, ATTACKCLASS *pBestStab, BOOLEAN fBladeAttack);
void CalcBestThrow(SOLDIERCLASS *pSoldier, ATTACKCLASS *pBestThrow);
void CalcTentacleAttack(SOLDIERCLASS *pSoldier, ATTACKCLASS *pBestStab);

INT16 CalcSpreadBurst(SOLDIERCLASS *pSoldier, INT16 sFirstTarget, INT8 bTargetLevel);
INT32 CalcManThreatValue(SOLDIERCLASS *pSoldier, INT16 sMyGrid, UINT8 ubReduceForCover,
                         SOLDIERCLASS *pMe);
INT8 CanNPCAttack(SOLDIERCLASS *pSoldier);
void CheckIfTossPossible(SOLDIERCLASS *pSoldier, ATTACKCLASS *pBestThrow);
BOOLEAN ClimbingNecessary(SOLDIERCLASS *pSoldier, INT16 sDestGridNo, INT8 bDestLevel);
INT8 ClosestPanicTrigger(SOLDIERCLASS *pSoldier);
INT16 ClosestReachableDisturbance(SOLDIERCLASS *pSoldier, UINT8 ubUnconsciousOK,
                                  BOOLEAN *pfChangeLevel);
INT16 ClosestReachableFriendInTrouble(SOLDIERCLASS *pSoldier, BOOLEAN *pfClimbingNecessary);
INT16 ClosestSeenOpponent(SOLDIERCLASS *pSoldier, INT16 *psGridNo, INT8 *pbLevel);
void CreatureCall(SOLDIERCLASS *pCaller);
INT8 CreatureDecideAction(SOLDIERCLASS *pCreature);
void CreatureDecideAlertStatus(SOLDIERCLASS *pCreature);
INT8 CrowDecideAction(SOLDIERCLASS *pSoldier);
void DecideAlertStatus(SOLDIERCLASS *pSoldier);
INT8 DecideAutoBandage(SOLDIERCLASS *pSoldier);
UINT16 DetermineMovementMode(SOLDIERCLASS *pSoldier, INT8 bAction);

// DIGGLER ON 30.11.2010
INT32 EstimateShotDamage(SOLDIERCLASS *pSoldier, SOLDIERCLASS *pOpponent, UINT8 ubChanceToHit,
                         INT8 bAimShotLocation);
// DIGGLER OFF
INT32 EstimateShotDamage(SOLDIERCLASS *pSoldier, SOLDIERCLASS *pOpponent, UINT8 ubChanceToHit);
INT32 EstimateStabDamage(SOLDIERCLASS *pSoldier, SOLDIERCLASS *pOpponent, UINT8 ubChanceToHit,
                         BOOLEAN fBladeAttack);
INT32 EstimateThrowDamage(SOLDIERCLASS *pSoldier, UINT8 ubItemPos, SOLDIERCLASS *pOpponent,
                          INT16 sGridno);
INT16 EstimatePathCostToLocation(SOLDIERCLASS *pSoldier, INT16 sDestGridNo, INT8 bDestLevel,
                                 BOOLEAN fAddCostAfterClimbingUp, BOOLEAN *pfClimbingNecessary,
                                 INT16 *psClimbGridNo);

BOOLEAN FindBetterSpotForItem(SOLDIERCLASS *pSoldier, INT8 bSlot);
INT16 FindClosestClimbPointAvailableToAI(SOLDIERCLASS *pSoldier, INT16 sStartGridNo,
                                         INT16 sDesiredGridNo, BOOLEAN fClimbUp);
INT16 FindRouteBackOntoMap(SOLDIERCLASS *pSoldier, INT16 sDestGridNo);
INT16 FindClosestBoxingRingSpot(SOLDIERCLASS *pSoldier, BOOLEAN fInRing);
INT16 GetInterveningClimbingLocation(SOLDIERCLASS *pSoldier, INT16 sDestGridNo, INT8 bDestLevel,
                                     BOOLEAN *pfClimbingNecessary);
UINT8 GetTraversalQuoteActionID(INT8 bDirection);
INT16 GoAsFarAsPossibleTowards(SOLDIERCLASS *pSoldier, INT16 sDesGrid, INT8 bAction);

INT8 HeadForTheStairCase(SOLDIERCLASS *pSoldier);

BOOLEAN InGas(SOLDIERCLASS *pSoldier, INT16 sGridNo);
BOOLEAN InGasOrSmoke(SOLDIERCLASS *pSoldier, INT16 sGridNo);
BOOLEAN InWaterGasOrSmoke(SOLDIERCLASS *pSoldier, INT16 sGridNo);

void InitAttackType(ATTACKCLASS *pAttack);

INT16 InternalGoAsFarAsPossibleTowards(SOLDIERCLASS *pSoldier, INT16 sDesGrid, INT8 bReserveAPs,
                                       INT8 bAction, INT8 fFlags);

int LegalNPCDestination(SOLDIERCLASS *pSoldier, INT16 sGridno, UINT8 ubPathMode, UINT8 ubWaterOK,
                        UINT8 fFlags);
void LoadWeaponIfNeeded(SOLDIERCLASS *pSoldier);
INT16 MostImportantNoiseHeard(SOLDIERCLASS *pSoldier, INT32 *piRetValue,
                              BOOLEAN *pfClimbingNecessary, BOOLEAN *pfReachable);
INT16 NPCConsiderInitiatingConv(SOLDIERCLASS *pNPC, UINT8 *pubDesiredMerc);
void NPCDoesAct(SOLDIERCLASS *pSoldier);
void NPCDoesNothing(SOLDIERCLASS *pSoldier);
INT8 OKToAttack(SOLDIERCLASS *ptr, int target);
BOOLEAN NeedToRadioAboutPanicTrigger(void);
INT8 PointPatrolAI(SOLDIERCLASS *pSoldier);
void PossiblyMakeThisEnemyChosenOne(SOLDIERCLASS *pSoldier);
INT8 RandomPointPatrolAI(SOLDIERCLASS *pSoldier);
INT32 RangeChangeDesire(SOLDIERCLASS *pSoldier);
UINT16 RealtimeDelay(SOLDIERCLASS *pSoldier);
void RearrangePocket(SOLDIERCLASS *pSoldier, INT8 bPocket1, INT8 bPocket2, UINT8 bPermanent);
void RTHandleAI(SOLDIERCLASS *pSoldier);
UINT16 RunAway(SOLDIERCLASS *pSoldier);
INT8 SearchForItems(SOLDIERCLASS *pSoldier, INT8 bReason, UINT16 usItem);
UINT8 ShootingStanceChange(SOLDIERCLASS *pSoldier, ATTACKCLASS *pAttack, INT8 bDesiredDirection);
UINT8 StanceChange(SOLDIERCLASS *pSoldier, UINT8 ubAttackAPCost);
INT16 TrackScent(SOLDIERCLASS *pSoldier);
void RefreshAI(SOLDIERCLASS *pSoldier);
BOOLEAN InLightAtNight(INT16 sGridNo, INT8 bLevel);
INT16 FindNearbyDarkerSpot(SOLDIERCLASS *pSoldier);

BOOLEAN ArmySeesOpponents(void);
