#ifndef __SOLDER_CONTROL_H
#define __SOLDER_CONTROL_H

// Kris:  November 10, 1997
// Please don't change this value from 10.  It will invalidate all of the maps and soldiers.
#define MAXPATROLGRIDS 10  // *** THIS IS A DUPLICATION - MUST BE MOVED !

#include "Tactical/AnimationCache.h"
#include "Utils/TimerControl.h"
#include "SGP/VObject.h"
#include "Tactical/OverheadTypes.h"
#include "Tactical/ItemTypes.h"

// TEMP VALUES FOR NAMES
#define MAXCIVLASTNAMES 30
extern UINT16 CivLastNames[MAXCIVLASTNAMES][10];

// ANDREW: these are defines for OKDestanation usage - please move to approprite file
#define IGNOREPEOPLE 0
#define PEOPLETOO 1
#define ALLPEOPLE 2
#define FALLINGTEST 3

#define LOCKED_NO_NEWGRIDNO 2

#define NO_PROFILE 200

#define BATTLE_SND_LOWER_VOLUME 1

#define TAKE_DAMAGE_GUNFIRE 1
#define TAKE_DAMAGE_BLADE 2
#define TAKE_DAMAGE_HANDTOHAND 3
#define TAKE_DAMAGE_FALLROOF 4
#define TAKE_DAMAGE_BLOODLOSS 5
#define TAKE_DAMAGE_EXPLOSION 6
#define TAKE_DAMAGE_ELECTRICITY 7
#define TAKE_DAMAGE_GAS 8
#define TAKE_DAMAGE_TENTACLES 9
#define TAKE_DAMAGE_STRUCTURE_EXPLOSION 10
#define TAKE_DAMAGE_OBJECT 11

#define SOLDIER_UNBLIT_SIZE (75 * 75 * 2)

#define SOLDIER_IS_TACTICALLY_VALID 0x00000001
#define SOLDIER_SHOULD_BE_TACTICALLY_VALID 0x00000002
#define SOLDIER_MULTI_SELECTED 0x00000004
#define SOLDIER_PC 0x00000008
#define SOLDIER_ATTACK_NOTICED 0x00000010
#define SOLDIER_PCUNDERAICONTROL 0x00000020
#define SOLDIER_UNDERAICONTROL 0x00000040
#define SOLDIER_DEAD 0x00000080
#define SOLDIER_GREEN_RAY 0x00000100
#define SOLDIER_LOOKFOR_ITEMS 0x00000200
#define SOLDIER_ENEMY 0x00000400
#define SOLDIER_ENGAGEDINACTION 0x00000800
#define SOLDIER_ROBOT 0x00001000
#define SOLDIER_MONSTER 0x00002000
#define SOLDIER_ANIMAL 0x00004000
#define SOLDIER_VEHICLE 0x00008000
#define SOLDIER_MULTITILE_NZ 0x00010000
#define SOLDIER_MULTITILE_Z 0x00020000
#define SOLDIER_MULTITILE (SOLDIER_MULTITILE_Z | SOLDIER_MULTITILE_NZ)
#define SOLDIER_RECHECKLIGHT 0x00040000
#define SOLDIER_TURNINGFROMHIT 0x00080000
#define SOLDIER_BOXER 0x00100000
#define SOLDIER_LOCKPENDINGACTIONCOUNTER 0x00200000
#define SOLDIER_COWERING 0x00400000
#define SOLDIER_MUTE 0x00800000
#define SOLDIER_GASSED 0x01000000
#define SOLDIER_OFF_MAP 0x02000000
#define SOLDIER_PAUSEANIMOVE 0x04000000
#define SOLDIER_DRIVER 0x08000000
#define SOLDIER_PASSENGER 0x10000000
#define SOLDIER_NPC_DOING_PUNCH 0x20000000
#define SOLDIER_NPC_SHOOTING 0x40000000
#define SOLDIER_LOOK_NEXT_TURNSOLDIER 0x80000000

/*
#define	SOLDIER_TRAIT_LOCKPICKING		0x0001
#define	SOLDIER_TRAIT_HANDTOHAND		0x0002
#define	SOLDIER_TRAIT_ELECTRONICS		0x0004
#define	SOLDIER_TRAIT_NIGHTOPS			0x0008
#define	SOLDIER_TRAIT_THROWING			0x0010
#define	SOLDIER_TRAIT_TEACHING			0x0020
#define	SOLDIER_TRAIT_HEAVY_WEAPS		0x0040
#define	SOLDIER_TRAIT_AUTO_WEAPS		0x0080
#define	SOLDIER_TRAIT_STEALTHY			0x0100
#define	SOLDIER_TRAIT_AMBIDEXT			0x0200
#define	SOLDIER_TRAIT_THIEF					0x0400
#define	SOLDIER_TRAIT_MARTIALARTS		0x0800
#define	SOLDIER_TRAIT_KNIFING				0x1000
*/
#define HAS_SKILL_TRAIT(s, t) (s->ubSkillTrait1 == t || s->ubSkillTrait2 == t)
#define NUM_SKILL_TRAITS(s, t) \
  ((s->ubSkillTrait1 == t) ? ((s->ubSkillTrait2 == t) ? 2 : 1) : ((s->ubSkillTrait2 == t) ? 1 : 0))
//***20.10.2007***
#define HAS_HEAD_ITEM(p, i) (p->inv[HEAD1POS].usItem == i || p->inv[HEAD2POS].usItem == i)

// DIGGLER ON   19.11.2010
// Всякие полезные макросы. Используются для наглядности кода в DecideActionNew и ишшо где-то
class SOLDIERCLASS;
class ATTACKCLASS;

#define IS_CROUCHED(s) (gAnimControl[s->usAnimState].ubHeight == ANIM_CROUCH)
#define IS_STANDING(s) (gAnimControl[s->usAnimState].ubHeight == ANIM_STAND)
#define IS_PRONED(s) (gAnimControl[s->usAnimState].ubHeight == ANIM_PRONE)
#define CURRENT_STANCE(s) (gAnimControl[s->usAnimState].ubHeight)

#define NOT_CROUCHED(x) (!IS_CROUCHED(x))
#define NOT_STANDING(x) (!IS_STANDING(x))
#define NOT_PRONED(x) (!IS_PRONED(x))

#define AM_A_BOXER(s) ((s)->uiStatusFlags & SOLDIER_BOXER)
#define NOT_A_BOXER(s) (!AM_A_BOXER(s))

extern INT8 MinPtsToMove(SOLDIERCLASS *pSoldier);
extern INT16 FindSpotMaxDistFromOpponents(SOLDIERCLASS *pSoldier);
extern INT8 CanNPCAttack(SOLDIERCLASS *pSoldier);
extern UINT8 MinAPsToAttack(SOLDIERCLASS *pSoldier, INT16 sGridno, UINT8 ubAddTurningCost);
extern INT8 FindAIUsableObjClass(SOLDIERCLASS *pSoldier, UINT32 usItemClass);
extern INT8 ChanceToGetThrough(SOLDIERCLASS *pFirer, FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ);
extern UINT8 AISoldierToSoldierChanceToGetThrough(SOLDIERCLASS *pStartSoldier,
                                                  SOLDIERCLASS *pEndSoldier);
extern INT16 GetAPsToReadyWeapon(SOLDIERCLASS *pSoldier, UINT16 usAnimState);
extern INT8 CalcActionPoints(SOLDIERCLASS *pSold);
extern void CalcNewActionPoints(SOLDIERCLASS *pSoldier);
extern UINT16 GetAPsToChangeStance(SOLDIERCLASS *pSoldier, INT8 bDesiredHeight);
extern UINT8 CalcTotalAPsToAttack(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubAddTurningCost,
                                  INT8 bAimTime);
// DIGGLER OFF

#define SOLDIER_QUOTE_SAID_IN_SHIT 0x0001
#define SOLDIER_QUOTE_SAID_LOW_BREATH 0x0002
#define SOLDIER_QUOTE_SAID_BEING_PUMMELED 0x0004
#define SOLDIER_QUOTE_SAID_NEED_SLEEP 0x0008
#define SOLDIER_QUOTE_SAID_LOW_MORAL 0x0010
#define SOLDIER_QUOTE_SAID_MULTIPLE_CREATURES 0x0020
#define SOLDIER_QUOTE_SAID_ANNOYING_MERC 0x0040
#define SOLDIER_QUOTE_SAID_LIKESGUN 0x0080
#define SOLDIER_QUOTE_SAID_DROWNING 0x0100
#define SOLDIER_QUOTE_SAID_ROTTINGCORPSE 0x0200
#define SOLDIER_QUOTE_SAID_SPOTTING_CREATURE_ATTACK 0x0400
#define SOLDIER_QUOTE_SAID_SMELLED_CREATURE 0x0800
#define SOLDIER_QUOTE_SAID_ANTICIPATING_DANGER 0x1000
#define SOLDIER_QUOTE_SAID_WORRIED_ABOUT_CREATURES 0x2000
#define SOLDIER_QUOTE_SAID_PERSONALITY 0x4000
#define SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE 0x8000

#define SOLDIER_QUOTE_SAID_EXT_HEARD_SOMETHING 0x0001
#define SOLDIER_QUOTE_SAID_EXT_SEEN_CREATURE_ATTACK 0x0002
#define SOLDIER_QUOTE_SAID_EXT_USED_BATTLESOUND_HIT 0x0004
#define SOLDIER_QUOTE_SAID_EXT_CLOSE_CALL 0x0008
#define SOLDIER_QUOTE_SAID_EXT_MIKE 0x0010
#define SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT 0x0020
#define SOLDIER_QUOTE_SAID_BUDDY_1_WITNESSED 0x0040
#define SOLDIER_QUOTE_SAID_BUDDY_2_WITNESSED 0x0080
#define SOLDIER_QUOTE_SAID_BUDDY_3_WITNESSED 0x0100

#define SOLDIER_CONTRACT_RENEW_QUOTE_NOT_USED 0
#define SOLDIER_CONTRACT_RENEW_QUOTE_89_USED 1
#define SOLDIER_CONTRACT_RENEW_QUOTE_115_USED 2

#define SOLDIER_MISC_HEARD_GUNSHOT 0x01
// make sure soldiers (esp tanks) are not hurt multiple times by explosions
#define SOLDIER_MISC_HURT_BY_EXPLOSION 0x02
// should be revealed due to xrays
#define SOLDIER_MISC_XRAYED 0x04

#define MAXBLOOD 40
#define NOBLOOD MAXBLOOD
#define BLOODTIME 5
#define FOOTPRINTTIME 2
#define MIN_BLEEDING_THRESHOLD 12  // you're OK while <4 Yellow life bars

#define BANDAGED(s) (s->bLifeMax - s->bLife - s->bBleeding)

// amount of time a stats is to be displayed differently, due to change
#define CHANGE_STAT_RECENTLY_DURATION 60000

// MACROS
// #######################################################

#define NO_PENDING_ACTION 255
#define NO_PENDING_ANIMATION 32001
#define NO_PENDING_DIRECTION 253
#define NO_PENDING_STANCE 254
#define NO_DESIRED_HEIGHT 255

#define MAX_FULLTILE_DIRECTIONS 3

// ENUMERATIONS FOR ACTIONS
enum {
  MERC_OPENDOOR,
  MERC_OPENSTRUCT,
  MERC_PICKUPITEM,
  MERC_PUNCH,
  MERC_KNIFEATTACK,
  MERC_GIVEAID,
  MERC_GIVEITEM,
  MERC_WAITFOROTHERSTOTRIGGER,
  MERC_CUTFFENCE,
  MERC_DROPBOMB,
  MERC_STEAL,
  MERC_TALK,
  MERC_ENTER_VEHICLE,
  MERC_REPAIR,
  MERC_RELOADROBOT,
  MERC_TAKEBLOOD,
  MERC_ATTACH_CAN,
  MERC_FUEL_VEHICLE,
};

// ENUMERATIONS FOR THROW ACTIONS
enum {
  NO_THROW_ACTION,
  THROW_ARM_ITEM,
  THROW_TARGET_MERC_CATCH,
};

// An enumeration for playing battle sounds
enum {
  BATTLE_SOUND_OK1,
  BATTLE_SOUND_OK2,
  BATTLE_SOUND_COOL1,
  BATTLE_SOUND_CURSE1,
  BATTLE_SOUND_HIT1,
  BATTLE_SOUND_HIT2,
  BATTLE_SOUND_LAUGH1,
  BATTLE_SOUND_ATTN1,
  BATTLE_SOUND_DIE1,
  BATTLE_SOUND_HUMM,
  BATTLE_SOUND_NOTHING,
  BATTLE_SOUND_GOTIT,
  BATTLE_SOUND_LOWMARALE_OK1,
  BATTLE_SOUND_LOWMARALE_OK2,
  BATTLE_SOUND_LOWMARALE_ATTN1,
  BATTLE_SOUND_LOCKED,
  BATTLE_SOUND_ENEMY,
  NUM_MERC_BATTLE_SOUNDS
};

// different kinds of merc
enum {
  MERC_TYPE__PLAYER_CHARACTER,
  MERC_TYPE__AIM_MERC,
  MERC_TYPE__MERC,
  MERC_TYPE__NPC,
  MERC_TYPE__EPC,
  MERC_TYPE__NPC_WITH_UNEXTENDABLE_CONTRACT,
  MERC_TYPE__VEHICLE,
};

// I don't care if this isn't intuitive!  The hand positions go right
// before the big pockets so we can loop through them that way. --CJC
#define NO_SLOT -1

// vehicle/human path structure
struct path {
  UINT32 uiSectorId;
  UINT32 uiEta;
  BOOLEAN fSpeed;
  struct path *pNext;
  struct path *pPrev;
};

typedef struct path PathSt;
typedef PathSt *PathStPtr;
enum {
  HELMETPOS = 0,
  VESTPOS,
  LEGPOS,
  HEAD1POS,
  HEAD2POS,
  HANDPOS,
  SECONDHANDPOS,
  BIGPOCK1POS,
  BIGPOCK2POS,
  BIGPOCK3POS,
  BIGPOCK4POS,
  SMALLPOCK1POS,
  SMALLPOCK2POS,
  SMALLPOCK3POS,
  SMALLPOCK4POS,
  SMALLPOCK5POS,
  SMALLPOCK6POS,
  SMALLPOCK7POS,
  SMALLPOCK8POS,  // = 18, so 19 pockets needed

  NUM_INV_SLOTS,
};

// used for color codes, but also shows the enemy type for debugging purposes
enum {
  SOLDIER_CLASS_NONE,
  SOLDIER_CLASS_ADMINISTRATOR,
  SOLDIER_CLASS_ELITE,
  SOLDIER_CLASS_ARMY,
  SOLDIER_CLASS_GREEN_MILITIA,
  SOLDIER_CLASS_REG_MILITIA,
  SOLDIER_CLASS_ELITE_MILITIA,
  SOLDIER_CLASS_CREATURE,
  SOLDIER_CLASS_MINER,
};

#define SOLDIER_CLASS_ENEMY(bSoldierClass) \
  ((bSoldierClass >= SOLDIER_CLASS_ADMINISTRATOR) && (bSoldierClass <= SOLDIER_CLASS_ARMY))
#define SOLDIER_CLASS_MILITIA(bSoldierClass) \
  ((bSoldierClass >= SOLDIER_CLASS_GREEN_MILITIA) && (bSoldierClass <= SOLDIER_CLASS_ELITE_MILITIA))

// This macro should be used whenever we want to see if someone is neutral
// IF WE ARE CONSIDERING ATTACKING THEM.  Creatures & bloodcats will attack neutrals
// but they can't attack empty vehicles!!
#define CONSIDERED_NEUTRAL(me, them) \
  ((them->bNeutral) && (me->bTeam != CREATURE_TEAM || (them->uiStatusFlags & SOLDIER_VEHICLE)))

typedef struct {
  UINT8 ubKeyID;
  UINT8 ubNumber;
} KEY_ON_RING;

typedef struct {
  float dX;
  float dY;
  float dZ;
  float dForceX;
  float dForceY;
  float dForceZ;
  float dLifeSpan;
  UINT8 ubActionCode;
  UINT32 uiActionData;

} THROW_PARAMS;

#define DELAYED_MOVEMENT_FLAG_PATH_THROUGH_PEOPLE 0x01

// reasons for being unable to continue movement
enum {
  REASON_STOPPED_NO_APS,
  REASON_STOPPED_SIGHT,
};

enum {
  HIT_BY_TEARGAS = 0x01,
  HIT_BY_MUSTARDGAS = 0x02,
  HIT_BY_CREATUREGAS = 0x04,
};

#define HEALTH_INCREASE 0x0001
#define STRENGTH_INCREASE 0x0002
#define DEX_INCREASE 0x0004
#define AGIL_INCREASE 0x0008
#define WIS_INCREASE 0x0010
#define LDR_INCREASE 0x0020

#define MRK_INCREASE 0x0040
#define MED_INCREASE 0x0080
#define EXP_INCREASE 0x0100
#define MECH_INCREASE 0x0200

#define LVL_INCREASE 0x0400

extern enum { WM_NORMAL = 0, WM_BURST, WM_ATTACHED, NUM_WEAPON_MODES } WeaponModes;

// DIGGLER ON 28.11.2010 Новая структура ИИ меняет и структуру Солдата.
class SOLDIERCLASS {
 public:
  // ID
  UINT8 ubID;
  UINT8 bReserved1;

  // DESCRIPTION / STATS, ETC
  UINT8 ubBodyType;
  INT8 bActionPoints;
  INT8 bInitialActionPoints;

  UINT32 uiStatusFlags;

  OBJECTTYPE inv[NUM_INV_SLOTS];
  OBJECTTYPE *pTempObject;
  KEY_ON_RING *pKeyRing;

  INT8 bOldLife;  // life at end of last turn, recorded for monster AI
  // attributes
  UINT8 bInSector;
  INT8 bFlashPortraitFrame;
  INT16 sFractLife;  // fraction of life pts (in hundreths)
  INT8 bBleeding;    // blood loss control variable
  INT8 bBreath;      // current breath value
  INT8 bBreathMax;   // max breath, affected by fatigue/sleep
  INT8 bStealthMode;

  INT16 sBreathRed;  // current breath value
  BOOLEAN fDelayedMovement;

  BOOLEAN fReloading;
  UINT8 ubWaitActionToDo;
  BOOLEAN fPauseAim;
  INT8 ubInsertionDirection;
  INT8 bGunType;
  // skills
  UINT8 ubOppNum;
  INT8 bLastRenderVisibleValue;
  BOOLEAN fInMissionExitNode;
  UINT8 ubAttackingHand;
  INT8 bScientific;
  // traits
  INT16 sWeightCarriedAtTurnStart;
  CHAR16 name[10];

  INT8 bVisible;  // to render or not to render...

  INT8 bActive;

  INT8 bTeam;  // Team identifier

  // NEW MOVEMENT INFORMATION for Strategic Movement
  UINT8 ubGroupID;          // the movement group the merc is currently part of.
  BOOLEAN fBetweenSectors;  // set when the group isn't actually in a sector.
                            // sSectorX and sSectorY will reflect the sector the
                            // merc was at last.

  UINT8 ubMovementNoiseHeard;  // 8 flags by direction

  // 23 bytes so far

  // WORLD POSITION STUFF
  FLOAT dXPos;
  FLOAT dYPos;
  FLOAT dOldXPos;
  FLOAT dOldYPos;
  INT16 sInitialGridNo;
  INT16 sGridNo;
  INT8 bDirection;
  INT16 sHeightAdjustment;
  INT16 sDesiredHeight;
  INT16 sTempNewGridNo;  // New grid no for advanced animations
  INT16 sRoomNo;
  INT8 bOverTerrainType;
  INT8 bOldOverTerrainType;

  INT8 bCollapsed;        // collapsed due to being out of APs
  INT8 bBreathCollapsed;  // collapsed due to being out of APs
  // 50 bytes so far

  UINT8 ubDesiredHeight;
  UINT16 usPendingAnimation;
  UINT8 ubPendingStanceChange;
  UINT16 usAnimState;
  BOOLEAN fNoAPToFinishMove;
  BOOLEAN fPausedMove;
  BOOLEAN fUIdeadMerc;        // UI Flags for removing a newly dead merc
  BOOLEAN fUInewMerc;         // UI Flags for adding newly created merc ( panels, etc )
  BOOLEAN fUICloseMerc;       // UI Flags for closing panels
  BOOLEAN fUIFirstTimeNOAP;   // UI Flag for diming guys when no APs ( dirty flags )
  BOOLEAN fUIFirstTimeUNCON;  // UI FLAG For unconscious dirty

  TIMECOUNTER UpdateCounter;
  TIMECOUNTER DamageCounter;
  TIMECOUNTER ReloadCounter;
  TIMECOUNTER FlashSelCounter;
  TIMECOUNTER AICounter;
  TIMECOUNTER FadeCounter;

  UINT8 ubSkillTrait1;
  UINT8 ubSkillTrait2;

  UINT32 uiAIDelay;
  INT8 bDexterity;  // dexterity (hand coord) value
  INT8 bWisdom;
  INT16 sReloadDelay;
  UINT8 ubAttackerID;
  UINT8 ubPreviousAttackerID;
  BOOLEAN fTurnInProgress;

  BOOLEAN fIntendedTarget;  // intentionally shot?
  BOOLEAN fPauseAllAnimation;

  INT8 bExpLevel;  // general experience level
  INT16 sInsertionGridNo;

  BOOLEAN fContinueMoveAfterStanceChange;

  // 60
  AnimationSurfaceCacheType AnimCache;  // will be 9 bytes once changed to pointers

  INT8 bLife;  // current life (hit points or health)
  UINT8 bSide;
  UINT8 bViewRange;
  INT8 bNewOppCnt;
  INT8 bService;  // first aid, or other time consuming process

  UINT16 usAniCode;
  UINT16 usAniFrame;
  INT16 sAniDelay;

  // MOVEMENT TO NEXT TILE HANDLING STUFF
  INT8 bAgility;  // agility (speed) value
  UINT8 ubDelayedMovementCauseMerc;
  INT16 sDelayedMovementCauseGridNo;
  INT16 sReservedMovementGridNo;

  INT8 bStrength;

  // Weapon Stuff
  BOOLEAN fHoldAttackerUntilDone;
  INT16 sTargetGridNo;
  INT8 bTargetLevel;
  INT8 bTargetCubeLevel;
  INT16 sLastTarget;
  INT8 bTilesMoved;
  INT8 bLeadership;
  FLOAT dNextBleed;
  BOOLEAN fWarnedAboutBleeding;
  BOOLEAN fDyingComment;

  UINT8 ubTilesMovedPerRTBreathUpdate;
  UINT16 usLastMovementAnimPerRTBreathUpdate;

  BOOLEAN fTurningToShoot;
  BOOLEAN fTurningToFall;
  BOOLEAN fTurningUntilDone;
  BOOLEAN fGettingHit;
  BOOLEAN fInNonintAnim;
  BOOLEAN fFlashLocator;
  INT16 sLocatorFrame;
  BOOLEAN fShowLocator;
  BOOLEAN fFlashPortrait;
  INT8 bMechanical;
  INT8 bLifeMax;  // maximum life for this merc

  INT32 iFaceIndex;

  // PALETTE MANAGEMENT STUFF
  PaletteRepID HeadPal;   // 30
  PaletteRepID PantsPal;  // 30
  PaletteRepID VestPal;   // 30
  PaletteRepID SkinPal;   // 30
  PaletteRepID MiscPal;   // 30

  // FULL 3-d TILE STUFF ( keep records of three tiles infront )
  UINT16 usFrontArcFullTileList[MAX_FULLTILE_DIRECTIONS];
  INT16 usFrontArcFullTileGridNos[MAX_FULLTILE_DIRECTIONS];

  SGPPaletteEntry *p8BPPPalette;  // 4
  UINT16 *p16BPPPalette;
  UINT16 *pShades[NUM_SOLDIER_SHADES];  // Shading tables
  UINT16 *pGlowShades[20];              //
  UINT16 *pCurrentShade;
  INT8 bMedical;
  BOOLEAN fBeginFade;
  UINT8 ubFadeLevel;
  UINT8 ubServiceCount;
  UINT8 ubServicePartner;
  INT8 bMarksmanship;
  INT8 bExplosive;
  THROW_PARAMS *pThrowParams;
  BOOLEAN fTurningFromPronePosition;
  INT8 bReverse;
  struct TAG_level_node *pLevelNode;
  struct TAG_level_node *pExternShadowLevelNode;
  struct TAG_level_node *pRoofUILevelNode;

  // WALKING STUFF
  INT8 bDesiredDirection;
  INT16 sDestXPos;
  INT16 sDestYPos;
  INT16 sDesiredDest;
  INT16 sDestination;
  INT16 sFinalDestination;
  INT8 bLevel;
  INT8 bStopped;
  INT8 bNeedToLook;

  // PATH STUFF
  UINT16 usPathingData[MAX_PATH_LIST_SIZE];
  UINT16 usPathDataSize;
  UINT16 usPathIndex;
  INT16 sBlackList;
  INT8 bAimTime;
  INT8 bShownAimTime;
  INT8 bPathStored;  // good for AI to reduct redundancy
  INT8 bHasKeys;     // allows AI controlled dudes to open locked doors

  // UNBLIT BACKGROUND
  UINT16 *pBackGround;
  UINT16 *pZBackground;
  UINT16 usUnblitX, usUnblitY;
  UINT16 usUnblitWidth, usUnblitHeight;

  UINT8 ubStrategicInsertionCode;
  UINT16 usStrategicInsertionData;

  INT32 iLight;
  INT32 iMuzFlash;
  INT8 bMuzFlashCount;

  INT16 sX;
  INT16 sY;

  UINT16 usOldAniState;
  INT16 sOldAniCode;

  INT8 bBulletsLeft;
  UINT8 ubSuppressionPoints;

  // STUFF FOR RANDOM ANIMATIONS
  UINT32 uiTimeOfLastRandomAction;
  INT16 usLastRandomAnim;

  // AI STUFF
  INT8 bOppList[MAX_NUM_SOLDIERS];  // AI knowledge database. Содержит число-информацию ИИ о
                                    // солдатах, например HEARD_THIS_TURN или SEEN_2_TURNS_AGO
  INT8 bLastAction;
  INT8 bAction;
  UINT16 usActionData;
  INT8 bNextAction;
  UINT16 usNextActionData;
  INT8 bActionInProgress;
  INT8 bAlertStatus;
  INT8 bOppCnt;
  INT8 bNeutral;
  INT8 bNewSituation;
  INT8 bNextTargetLevel;
  INT8 bOrders;
  INT8 bAttitude;
  INT8 bUnderFire;
  INT8 bShock;
  INT8 bUnderEscort;
  INT8 bBypassToGreen;
  UINT8 ubLastMercToRadio;
  INT8 bDominantDir;                   // AI main direction to face...
  INT8 bPatrolCnt;                     // number of patrol gridnos
  INT8 bNextPatrolPnt;                 // index to next patrol gridno
  INT16 usPatrolGrid[MAXPATROLGRIDS];  // AI list for ptr->orders==PATROL
  INT16 sNoiseGridno;
  UINT8 ubNoiseVolume;
  INT8 bLastAttackHit;
  UINT8 ubXRayedBy;
  FLOAT dHeightAdjustment;
  INT8 bMorale;
  INT8 bTeamMoraleMod;
  INT8 bTacticalMoraleMod;
  INT8 bStrategicMoraleMod;
  INT8 bAIMorale;
  UINT8 ubPendingAction;
  UINT8 ubPendingActionAnimCount;
  UINT32 uiPendingActionData1;
  INT16 sPendingActionData2;
  INT8 bPendingActionData3;
  INT8 ubDoorHandleCode;
  UINT32 uiPendingActionData4;
  INT8 bInterruptDuelPts;
  INT8 bPassedLastInterrupt;
  INT8 bIntStartAPs;
  INT8 bMoved;
  INT8 bHunting;
  UINT8 ubLastCall;
  UINT8 ubCaller;
  INT16 sCallerGridNo;
  UINT8 bCallPriority;
  INT8 bCallActedUpon;
  INT8 bFrenzied;
  INT8 bNormalSmell;
  INT8 bMonsterSmell;
  INT8 bMobility;
  INT8 bRTPCombat;  // видимо, это стиль ведения боя. На текущий момент - неиспользуемая вещь
  INT8 fAIFlags;

  BOOLEAN fDontChargeReadyAPs;
  UINT16 usAnimSurface;
  UINT16 sZLevel;
  BOOLEAN fPrevInWater;
  BOOLEAN fGoBackToAimAfterHit;

  INT16 sWalkToAttackGridNo;
  INT16 sWalkToAttackWalkToCost;

  BOOLEAN fForceRenderColor;
  BOOLEAN fForceNoRenderPaletteCycle;

  INT16 sLocatorOffX;
  INT16 sLocatorOffY;
  BOOLEAN fStopPendingNextTile;

  BOOLEAN fForceShade;
  UINT16 *pForcedShade;

  INT8 bDisplayDamageCount;
  INT8 fDisplayDamage;
  INT16 sDamage;
  INT16 sDamageX;
  INT16 sDamageY;
  INT8 bDamageDir;
  INT8 bDoBurst;  // нулевое значение означает одиночный выстрел, ненулевое - порядковый номер в
                  // очереди...
  INT16 usUIMovementMode;
  INT8 bUIInterfaceLevel;
  BOOLEAN fUIMovementFast;

  TIMECOUNTER BlinkSelCounter;
  TIMECOUNTER PortraitFlashCounter;
  BOOLEAN fDeadSoundPlayed;
  UINT8 ubProfile;
  UINT8 ubQuoteRecord;
  UINT8 ubQuoteActionID;
  UINT8 ubBattleSoundID;

  BOOLEAN fClosePanel;
  BOOLEAN fClosePanelToDie;
  UINT8 ubClosePanelFrame;
  BOOLEAN fDeadPanel;
  UINT8 ubDeadPanelFrame;
  BOOLEAN fOpenPanel;
  INT8 bOpenPanelFrame;

  INT16 sPanelFaceX;
  INT16 sPanelFaceY;

  // QUOTE STUFF
  INT8 bNumHitsThisTurn;
  UINT16 usQuoteSaidFlags;
  INT8 fCloseCall;
  INT8 bLastSkillCheck;
  INT8 ubSkillCheckAttempts;

  INT8 bVocalVolume;  // verbal sounds need to differ in volume

  INT8 bStartFallDir;
  INT8 fTryingToFall;

  UINT8 ubPendingDirection;
  UINT32 uiAnimSubFlags;

  UINT8 bAimShotLocation;
  UINT8 ubHitLocation;

  UINT16 *pEffectShades[NUM_SOLDIER_EFFECTSHADES];  // Shading tables for effects

  UINT8 ubPlannedUIAPCost;
  INT16 sPlannedTargetX;
  INT16 sPlannedTargetY;

  INT16 sSpreadLocations[6];
  BOOLEAN fDoSpread;
  INT16 sStartGridNo;
  INT16 sEndGridNo;
  INT16 sForcastGridno;
  INT16 sZLevelOverride;
  INT8 bMovedPriorToInterrupt;
  INT32 iEndofContractTime;  // time, in global time(resolution, minutes) that merc will leave, or
                             // if its a M.E.R.C. merc it will be set to -1.  -2 for NPC and player
                             // generated
  INT32 iStartContractTime;
  INT32 iTotalContractLength;    // total time of AIM mercs contract	or the time since last paid
                                 // for a M.E.R.C. merc
  INT32 iNextActionSpecialData;  // AI special action data record for the next action
  UINT8 ubWhatKindOfMercAmI;     // Set to the type of character it is
  INT8 bAssignment;              // soldiers current assignment
  INT8 bOldAssignment;           // old assignment, for autosleep purposes
  BOOLEAN fForcedToStayAwake;    // forced by player to stay awake, reset to false, the moment they
                                 // are set to rest or sleep
  INT8 bTrainStat;               // current stat soldier is training
  INT16 sSectorX;                // X position on the Stategic Map
  INT16 sSectorY;                // Y position on the Stategic Map
  INT8 bSectorZ;                 // Z sector location
  INT32 iVehicleId;              // the id of the vehicle the char is in
  PathStPtr pMercPath;           // Path Structure
  UINT8 fHitByGasFlags;          // flags
  UINT16 usMedicalDeposit;       // is there a medical deposit on merc
  UINT16 usLifeInsurance;        // is there life insurance taken out on merc

  // DEF:  Used for the communications
  UINT32 uiStartMovementTime;    // the time since the merc first started moving
  UINT32 uiOptimumMovementTime;  // everytime in ececute overhead the time for the current ani will
                                 // be added to this total
  UINT32 usLastUpdateTime;       // The last time the soldier was in ExecuteOverhead

  BOOLEAN fIsSoldierMoving;   // ie.  Record time is on
  BOOLEAN fIsSoldierDelayed;  // Is the soldier delayed Soldier
  BOOLEAN fSoldierUpdatedFromNetwork;
  UINT32 uiSoldierUpdateNumber;
  BYTE ubSoldierUpdateType;
  // END

  INT32 iStartOfInsuranceContract;
  UINT32 uiLastAssignmentChangeMin;  // timestamp of last assignment change in minutes
  INT32 iTotalLengthOfInsuranceContract;

  UINT8 ubSoldierClass;  // admin, elite, troop (creature types?)
  UINT8 ubAPsLostToSuppression;
  BOOLEAN fChangingStanceDueToSuppression;
  UINT8 ubSuppressorID;

  // Squad merging vars
  UINT8 ubDesiredSquadAssignment;
  UINT8 ubNumTraversalsAllowedToMerge;

  UINT16 usPendingAnimation2;
  UINT8 ubCivilianGroup;

  // time changes...when a stat was changed according to GetJA2Clock();
  UINT32 uiChangeLevelTime;
  UINT32 uiChangeHealthTime;
  UINT32 uiChangeStrengthTime;
  UINT32 uiChangeDexterityTime;
  UINT32 uiChangeAgilityTime;
  UINT32 uiChangeWisdomTime;
  UINT32 uiChangeLeadershipTime;
  UINT32 uiChangeMarksmanshipTime;
  UINT32 uiChangeExplosivesTime;
  UINT32 uiChangeMedicalTime;
  UINT32 uiChangeMechanicalTime;

  UINT32 uiUniqueSoldierIdValue;  // the unique value every instance of a soldier gets - 1 is the
                                  // first valid value
  INT8 bBeingAttackedCount;       // Being attacked counter

  INT8 bNewItemCount[NUM_INV_SLOTS];
  INT8 bNewItemCycleCount[NUM_INV_SLOTS];
  BOOLEAN fCheckForNewlyAddedItems;
  INT8 bEndDoorOpenCode;

  UINT8 ubScheduleID;
  INT16 sEndDoorOpenCodeData;
  TIMECOUNTER NextTileCounter;
  BOOLEAN fBlockedByAnotherMerc;
  INT8 bBlockedByAnotherMercDirection;
  UINT16 usAttackingWeapon;
  INT8 bWeaponMode;
  UINT8 ubTargetID;
  INT8 bAIScheduleProgress;
  INT16 sOffWorldGridNo;
  struct TAG_anitile *pAniTile;
  INT8 bCamo;
  INT16 sAbsoluteFinalDestination;
  UINT8 ubHiResDirection;
  UINT8 ubHiResDesiredDirection;
  UINT8 ubLastFootPrintSound;
  INT8 bVehicleID;
  INT8 fPastXDest;
  INT8 fPastYDest;
  INT8 bMovementDirection;
  INT16 sOldGridNo;
  UINT16 usDontUpdateNewGridNoOnMoveAnimChange;
  INT16 sBoundingBoxWidth;
  INT16 sBoundingBoxHeight;
  INT16 sBoundingBoxOffsetX;
  INT16 sBoundingBoxOffsetY;
  UINT32 uiTimeSameBattleSndDone;
  INT8 bOldBattleSnd;
  BOOLEAN fReactingFromBeingShot;
  BOOLEAN fContractPriceHasIncreased;
  INT32 iBurstSoundID;
  BOOLEAN fFixingSAMSite;
  BOOLEAN fFixingRobot;
  INT8 bSlotItemTakenFrom;
  BOOLEAN fSignedAnotherContract;
  UINT8 ubAutoBandagingMedic;
  BOOLEAN fDontChargeTurningAPs;
  UINT8 ubRobotRemoteHolderID;
  UINT32 uiTimeOfLastContractUpdate;
  INT8 bTypeOfLastContract;
  INT8 bTurnsCollapsed;
  INT8 bSleepDrugCounter;
  UINT8 ubMilitiaKills;

  INT8 bFutureDrugEffect[2];    // value to represent effect of a needle
  INT8 bDrugEffectRate[2];      // represents rate of increase and decrease of effect
  INT8 bDrugEffect[2];          // value that affects AP & morale calc ( -ve is poorly )
  INT8 bDrugSideEffectRate[2];  // duration of negative AP and morale effect
  INT8 bDrugSideEffect[2];      // duration of negative AP and morale effect
  INT8 bTimesDrugUsedSinceSleep[2];

  INT8 bBlindedCounter;
  BOOLEAN fMercCollapsedFlag;
  BOOLEAN fDoneAssignmentAndNothingToDoFlag;
  BOOLEAN fMercAsleep;
  BOOLEAN fDontChargeAPsForStanceChange;

  UINT8 ubHoursOnAssignment;  // used for assignments handled only every X hours

  UINT8
  ubMercJustFired;  // the merc was just fired..there may be dialogue events occuring, this flag
                    // will prevent any interaction with contracts until after the merc leaves
  UINT8 ubTurnsUntilCanSayHeardNoise;
  UINT16 usQuoteSaidExtFlags;

  UINT16 sContPathLocation;
  INT8 bGoodContPath;
  UINT8 ubPendingActionInterrupted;
  INT8 bNoiseLevel;
  INT8 bRegenerationCounter;
  INT8 bRegenBoostersUsedToday;
  INT8 bNumPelletsHitBy;
  INT16 sSkillCheckGridNo;
  UINT8 ubLastEnemyCycledID;

  UINT8 ubPrevSectorID;
  UINT8 ubNumTilesMovesSinceLastForget;
  INT8 bTurningIncrement;
  UINT32 uiBattleSoundID;

  BOOLEAN fSoldierWasMoving;
  BOOLEAN fSayAmmoQuotePending;
  UINT16 usValueGoneUp;

  UINT8 ubNumLocateCycles;
  UINT8 ubDelayedMovementFlags;
  BOOLEAN fMuzzleFlash;
  UINT8 ubCTGTTargetID;

  TIMECOUNTER PanelAnimateCounter;
  UINT32 uiMercChecksum;

  INT8 bCurrentCivQuote;
  INT8 bCurrentCivQuoteDelta;
  UINT8 ubMiscSoldierFlags;
  UINT8 ubReasonCantFinishMove;

  INT16 sLocationOfFadeStart;
  UINT8 bUseExitGridForReentryDirection;

  UINT32 uiTimeSinceLastSpoke;
  UINT8 ubContractRenewalQuoteCode;
  INT16 sPreTraversalGridNo;
  UINT32 uiXRayActivatedTime;
  INT8 bTurningFromUI;
  INT8 bPendingActionData5;

  INT8 bDelayedStrategicMoraleMod;
  UINT8 ubDoorOpeningNoise;

  struct GROUP *pGroup;
  UINT8 ubLeaveHistoryCode;
  BOOLEAN fDontUnsetLastTargetFromTurn;
  INT8 bOverrideMoveSpeed;
  BOOLEAN fUseMoverrideMoveSpeed;

  UINT32 uiTimeSoldierWillArrive;
  BOOLEAN fDieSoundUsed;
  BOOLEAN fUseLandingZoneForArrival;
  BOOLEAN fFallClockwise;
  INT8 bVehicleUnderRepairID;
  INT32 iTimeCanSignElsewhere;
  INT8 bHospitalPriceModifier;
  INT8 bFillerBytes[3];
  UINT32 uiStartTimeOfInsuranceContract;
  BOOLEAN fRTInNonintAnim;
  BOOLEAN fDoingExternalDeath;
  INT8 bCorpseQuoteTolerance;
  INT8 bYetAnotherPaddingSpace;
  INT32 iPositionSndID;
  INT32 iTuringSoundID;
  UINT8 ubLastDamageReason;
  BOOLEAN fComplainedThatTired;
  INT16 sLastTwoLocations[2];
  INT16 bFillerDude;
  INT32 uiTimeSinceLastBleedGrunt;
  UINT8 ubNextToPreviousAttackerID;

  //***30.10.2010*** приводим в порядок бардак с bFiller
  INT8 bSteps;       // bFiller[0]
  INT8 bSpeed;       // bFiller[1]
  INT8 bThrowAngle;  // bFiller[2]

  UINT8 ubActiveScope;
  UINT8 ubLastScope;

  UINT8 bFiller[39 - 5];

  // DIGGLER 27.11.2010 Методы
  BOOLEAN Unconscious() { return ((bLife < OKLIFE) && !bService); }
  void ChangeStance(UINT8 ubDesiredStance);

  INT8 AP_MinPtsToMove() { return MinPtsToMove(this); }
  UINT8 AP_CalcTotalAPsToAttack(INT16 sGridNo, UINT8 ubAddTurningCost, INT8 bAimTime) {
    return CalcTotalAPsToAttack(this, sGridNo, ubAddTurningCost, bAimTime);
  }
  UINT8 AP_MinAPsToAttack(INT16 sGridno, UINT8 ubAddTurningCost) {
    return MinAPsToAttack(this, sGridno, ubAddTurningCost);
  }
  INT16 AP_GetAPsToReadyWeapon() { return GetAPsToReadyWeapon(this, this->usAnimState); }
  INT8 AP_CalcActionPoints() { return CalcActionPoints(this); }
  void AP_CalcAndSetNewTurnActionPoints() { CalcNewActionPoints(this); }
  UINT16 AP_CalcAPsToBurst();  //} // сколько надо AP на выстрел из того, что в руках
  UINT16 AP_GetAPsToChangeStance(INT8 bDesiredHeight) {
    return GetAPsToChangeStance(this, bDesiredHeight);
  }

  OBJECTTYPE *Inv_pAttackingWeapon() { return &(this->inv[this->ubAttackingHand]); }

  FLOAT Coords_ZTargetPos(UINT8 ubAimPos);
  FLOAT Coords_AbsoluteZTargetPos(UINT8 ubAimPos);

  UINT8 CanMove() { return (this->bActionPoints >= MinPtsToMove(this)); }
  INT16 FindGridNoMaxDistantFromOpponents() { return FindSpotMaxDistFromOpponents(this); }
  INT8 CanAttack() { return CanNPCAttack(this); }
  INT32 CalcMyThreatValue();

  UINT8 AI_SoldierToSoldierChanceToGetThrough(SOLDIERCLASS *pOpponent) {
    return AISoldierToSoldierChanceToGetThrough(this, pOpponent);
  }
  INT8 AI_FindUsableObjClass(UINT32 usItemClass) {
    return (FindAIUsableObjClass(this, usItemClass));
  }
  INT16 AI_FindReachableTiles(INT8 bMode, INT16 uiAPBudget);
  UINT8 AI_ChanceToSurviveAfterAttack(ATTACKCLASS *pAttack);
  UINT8 AI_FillThreatsArray(INT16 iRange = 0);

  INT8 Chance_ToGetThrough(FLOAT dEndX, FLOAT dEndY, FLOAT dEndZ) {
    return ChanceToGetThrough(this, dEndX, dEndY, dEndZ);
  }
  BOOLEAN IsOnPlayerSide() { return (bSide == PLAYER_SIDE); }
  BOOLEAN IsInPlayerTeam() { return (bTeam == PLAYER_TEAM); }
  //	BOOLEAN WearGasMaskIfAvail( void ) { WearGasMaskIfAvailable(this); }
};

// DIGGLER OFF
// TYPEDEFS FOR ANIMATION PROFILES
typedef struct {
  UINT16 usTileFlags;
  INT8 bTileX;
  INT8 bTileY;

} ANIM_PROF_TILE;

typedef struct {
  UINT8 ubNumTiles;
  ANIM_PROF_TILE *pTiles;

} ANIM_PROF_DIR;

typedef struct ANIM_PROF {
  ANIM_PROF_DIR Dirs[8];

} ANIM_PROF;

// Globals
//////////

// VARIABLES FOR PALETTE REPLACEMENTS FOR HAIR, ETC
extern UINT32 guiNumPaletteSubRanges;
extern UINT8 *gubpNumReplacementsPerRange;
extern PaletteSubRangeType *gpPaletteSubRanges;
extern UINT32 guiNumReplacements;
extern PaletteReplacementType *gpPalRep;

extern UINT8 bHealthStrRanges[];

// Functions
////////////

// CREATION FUNCTIONS
BOOLEAN DeleteSoldier(SOLDIERCLASS *pSoldier);
BOOLEAN CreateSoldierLight(SOLDIERCLASS *pSoldier);
BOOLEAN DeleteSoldierLight(SOLDIERCLASS *pSoldier);

BOOLEAN CreateSoldierCommon(UINT8 ubBodyType, SOLDIERCLASS *pSoldier, UINT16 usSoldierID,
                            UINT16 usState);

// Soldier Management functions, called by Event Pump.c
BOOLEAN EVENT_InitNewSoldierAnim(SOLDIERCLASS *pSoldier, UINT16 usNewState,
                                 UINT16 usStartingAniCode, BOOLEAN fForce);

BOOLEAN ChangeSoldierState(SOLDIERCLASS *pSoldier, UINT16 usNewState, UINT16 usStartingAniCode,
                           BOOLEAN fForce);
void EVENT_SetSoldierPosition(SOLDIERCLASS *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos);
void EVENT_SetSoldierDestination(SOLDIERCLASS *pSoldier, UINT16 usNewDirection);
void EVENT_GetNewSoldierPath(SOLDIERCLASS *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim);
BOOLEAN EVENT_InternalGetNewSoldierPath(SOLDIERCLASS *pSoldier, UINT16 sDestGridNo,
                                        UINT16 usMovementAnim, BOOLEAN fFromUI,
                                        BOOLEAN fForceRestart);

void EVENT_SetSoldierDirection(SOLDIERCLASS *pSoldier, UINT16 usNewDirection);
void EVENT_SetSoldierDesiredDirection(SOLDIERCLASS *pSoldier, UINT16 usNewDirection);
void EVENT_FireSoldierWeapon(SOLDIERCLASS *pSoldier, INT16 sTargetGridNo);
void EVENT_SoldierGotHit(SOLDIERCLASS *pSoldier, UINT16 usWeaponIndex, INT16 ubDamage,
                         INT16 sBreathLoss, UINT16 bDirection, UINT16 sRange, UINT8 ubAttackerID,
                         UINT8 ubSpecial, UINT8 ubHitLocation, INT16 sSubsequent,
                         INT16 sLocationGridNo);
void EVENT_SoldierBeginBladeAttack(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);
void EVENT_SoldierBeginPunchAttack(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);
void EVENT_SoldierBeginFirstAid(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);
void EVENT_StopMerc(SOLDIERCLASS *pSoldier, INT16 sGridNo, INT8 bDirection);
void EVENT_SoldierBeginCutFence(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);
void EVENT_SoldierBeginRepair(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);
void EVENT_SoldierBeginRefuel(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);

BOOLEAN SoldierReadyWeapon(SOLDIERCLASS *pSoldier, INT16 sTargetXPos, INT16 sTargetYPos,
                           BOOLEAN fEndReady);
void SetSoldierHeight(SOLDIERCLASS *pSoldier, FLOAT dNewHeight);
void BeginSoldierClimbUpRoof(SOLDIERCLASS *pSoldier);
void BeginSoldierClimbDownRoof(SOLDIERCLASS *pSoldier);
void BeginSoldierClimbFence(SOLDIERCLASS *pSoldier);
void SetSoldierGridNo(SOLDIERCLASS *pSoldier, INT16 sNewGridNo, BOOLEAN fForceRemove);

BOOLEAN CheckSoldierHitRoof(SOLDIERCLASS *pSoldier);
void BeginSoldierGetup(SOLDIERCLASS *pSoldier);
BOOLEAN ReCreateSelectedSoldierLight();

// Soldier Management functions called by Overhead.c
BOOLEAN ConvertAniCodeToAniFrame(SOLDIERCLASS *pSoldier, UINT16 usAniFrame);
void TurnSoldier(SOLDIERCLASS *pSold);
void EVENT_BeginMercTurn(SOLDIERCLASS *pSoldier, BOOLEAN fFromRealTime, INT32 iRealTimeCounter);
void ChangeSoldierStance(SOLDIERCLASS *pSoldier, UINT8 ubDesiredStance);
void ModifySoldierAniSpeed(SOLDIERCLASS *pSoldier);
void StopSoldier(SOLDIERCLASS *pSoldier);
UINT8 SoldierTakeDamage(SOLDIERCLASS *pSoldier, INT8 bHeight, INT16 sLifeDeduct,
                        INT16 sBreathDeduct, UINT8 ubReason, UINT8 ubAttacker, INT16 sSourceGrid,
                        INT16 sSubsequent, BOOLEAN fShowDamage);
void RevivePlayerTeam();
void ReviveSoldier(SOLDIERCLASS *pSoldier);

// Palette functions for soldiers
BOOLEAN CreateSoldierPalettes(SOLDIERCLASS *pSoldier);
BOOLEAN GetPaletteRepIndexFromID(PaletteRepID aPalRep, UINT8 *pubPalIndex);
BOOLEAN SetPaletteReplacement(SGPPaletteEntry *p8BPPPalette, PaletteRepID aPalRep);
BOOLEAN LoadPaletteData();
BOOLEAN DeletePaletteData();

// UTILITY FUNCTUIONS
void MoveMerc(SOLDIERCLASS *pSoldier, FLOAT dMovementChange, FLOAT dAngle, BOOLEAN fCheckRange);
void MoveMercFacingDirection(SOLDIERCLASS *pSoldier, BOOLEAN fReverse, FLOAT dMovementDist);
INT16 GetDirectionFromXY(INT16 sXPos, INT16 sYPos, SOLDIERCLASS *pSoldier);
INT16 GetDirectionFromGridNo(INT16 sGridNo, SOLDIERCLASS *pSoldier);
UINT8 atan8(INT16 sXPos, INT16 sYPos, INT16 sXPos2, INT16 sYPos2);
UINT8 atan8FromAngle(DOUBLE dAngle);
INT8 CalcActionPoints(SOLDIERCLASS *pSold);
BOOLEAN IsActionInterruptable(SOLDIERCLASS *pSoldier);
INT16 GetDirectionToGridNoFromGridNo(INT16 sGridNoDest, INT16 sGridNoSrc);
void ReleaseSoldiersAttacker(SOLDIERCLASS *pSoldier);
BOOLEAN MercInWater(SOLDIERCLASS *pSoldier);
UINT16 GetNewSoldierStateFromNewStance(SOLDIERCLASS *pSoldier, UINT8 ubDesiredStance);
UINT16 GetMoveStateBasedOnStance(SOLDIERCLASS *pSoldier, UINT8 ubStanceHeight);
void SoldierGotoStationaryStance(SOLDIERCLASS *pSoldier);
BOOLEAN ReCreateSoldierLight(SOLDIERCLASS *pSoldier);

BOOLEAN DoMercBattleSound(SOLDIERCLASS *pSoldier, UINT8 ubBattleSoundID);
BOOLEAN InternalDoMercBattleSound(SOLDIERCLASS *pSoldier, UINT8 ubBattleSoundID, INT8 bSpecialCode);

UINT32 SoldierDressWound(SOLDIERCLASS *pSoldier, SOLDIERCLASS *pVictim, INT16 sKitPts,
                         INT16 sStatus);
void ReceivingSoldierCancelServices(SOLDIERCLASS *pSoldier);
void GivingSoldierCancelServices(SOLDIERCLASS *pSoldier);
void InternalReceivingSoldierCancelServices(SOLDIERCLASS *pSoldier, BOOLEAN fPlayEndAnim);
void InternalGivingSoldierCancelServices(SOLDIERCLASS *pSoldier, BOOLEAN fPlayEndAnim);

// WRAPPERS FOR SOLDIER EVENTS
void SendSoldierPositionEvent(SOLDIERCLASS *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos);
void SendSoldierDestinationEvent(SOLDIERCLASS *pSoldier, UINT16 usNewDestination);
void SendGetNewSoldierPathEvent(SOLDIERCLASS *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim);
void SendSoldierSetDirectionEvent(SOLDIERCLASS *pSoldier, UINT16 usNewDirection);
void SendSoldierSetDesiredDirectionEvent(SOLDIERCLASS *pSoldier, UINT16 usDesiredDirection);
void SendChangeSoldierStanceEvent(SOLDIERCLASS *pSoldier, UINT8 ubNewStance);
void SendBeginFireWeaponEvent(SOLDIERCLASS *pSoldier, INT16 sTargetGridNo);

void HandleAnimationProfile(SOLDIERCLASS *pSoldier, UINT16 usAnimState, BOOLEAN fRemove);
BOOLEAN GetProfileFlagsFromGridno(SOLDIERCLASS *pSoldier, UINT16 usAnimState, UINT16 sTestGridNo,
                                  UINT16 *usFlags);

void HaultSoldierFromSighting(SOLDIERCLASS *pSoldier, BOOLEAN fFromSightingEnemy);
void ReLoadSoldierAnimationDueToHandItemChange(SOLDIERCLASS *pSoldier, UINT16 usOldItem,
                                               UINT16 usNewItem);

BOOLEAN CheckForBreathCollapse(SOLDIERCLASS *pSoldier);

BOOLEAN PreloadSoldierBattleSounds(SOLDIERCLASS *pSoldier, BOOLEAN fRemove);

#define PTR_CIVILIAN (pSoldier->bTeam == CIV_TEAM)
#define PTR_CROUCHED (gAnimControl[pSoldier->usAnimState].ubHeight == ANIM_CROUCH)
#define PTR_STANDING (gAnimControl[pSoldier->usAnimState].ubHeight == ANIM_STAND)
#define PTR_PRONE (gAnimControl[pSoldier->usAnimState].ubHeight == ANIM_PRONE)

void EVENT_SoldierBeginGiveItem(SOLDIERCLASS *pSoldier);

void DoNinjaAttack(SOLDIERCLASS *pSoldier);

BOOLEAN SoldierCarriesTwoHandedWeapon(SOLDIERCLASS *pSoldier);
BOOLEAN InternalSoldierReadyWeapon(SOLDIERCLASS *pSoldier, UINT8 sFacingDir, BOOLEAN fEndReady);

void RemoveSoldierFromGridNo(SOLDIERCLASS *pSoldier);

void PositionSoldierLight(SOLDIERCLASS *pSoldier);

void SetCheckSoldierLightFlag(SOLDIERCLASS *pSoldier);

void EVENT_InternalSetSoldierDestination(SOLDIERCLASS *pSoldier, UINT16 usNewDirection,
                                         BOOLEAN fFromMove, UINT16 usAnimState);

void ChangeToFlybackAnimation(SOLDIERCLASS *pSoldier, INT8 bDirection);
void ChangeToFallbackAnimation(SOLDIERCLASS *pSoldier, INT8 bDirection);

// reset soldier timers
void ResetSoldierChangeStatTimer(SOLDIERCLASS *pSoldier);

void EVENT_SoldierBeginKnifeThrowAttack(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);
void EVENT_SoldierBeginUseDetonator(SOLDIERCLASS *pSoldier);
void EVENT_SoldierBeginDropBomb(SOLDIERCLASS *pSoldier);
void EVENT_SoldierEnterVehicle(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);

void SetSoldierCowerState(SOLDIERCLASS *pSoldier, BOOLEAN fOn);

BOOLEAN PlayerSoldierStartTalking(SOLDIERCLASS *pSoldier, UINT8 ubTargetID, BOOLEAN fValidate);

void EVENT_InternalSetSoldierPosition(SOLDIERCLASS *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos,
                                      BOOLEAN fUpdateDest, BOOLEAN fUpdateFinalDest,
                                      BOOLEAN fForceDelete);

void InternalRemoveSoldierFromGridNo(SOLDIERCLASS *pSoldier, BOOLEAN fForce);

void EVENT_SetSoldierPositionAndMaybeFinalDest(SOLDIERCLASS *pSoldier, FLOAT dNewXPos,
                                               FLOAT dNewYPos, BOOLEAN fUpdateFinalDest);

void EVENT_SetSoldierPositionForceDelete(SOLDIERCLASS *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos);

void CalcNewActionPoints(SOLDIERCLASS *pSoldier);

BOOLEAN InternalIsValidStance(SOLDIERCLASS *pSoldier, INT8 bDirection, INT8 bNewStance);

void AdjustNoAPToFinishMove(SOLDIERCLASS *pSoldier, BOOLEAN fSet);

void UpdateRobotControllerGivenController(SOLDIERCLASS *pSoldier);
void UpdateRobotControllerGivenRobot(SOLDIERCLASS *pSoldier);
SOLDIERCLASS *GetRobotController(SOLDIERCLASS *pSoldier);
BOOLEAN CanRobotBeControlled(SOLDIERCLASS *pSoldier);
BOOLEAN ControllingRobot(SOLDIERCLASS *pSoldier);

void EVENT_SoldierBeginReloadRobot(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection,
                                   UINT8 ubMercSlot);

void EVENT_SoldierBeginTakeBlood(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);

void EVENT_SoldierBeginAttachCan(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubDirection);

void HandleSoldierTakeDamageFeedback(SOLDIERCLASS *pSoldier);

void PickDropItemAnimation(SOLDIERCLASS *pSoldier);

BOOLEAN IsValidSecondHandShot(SOLDIERCLASS *pSoldier);
BOOLEAN IsValidSecondHandShotForReloadingPurposes(SOLDIERCLASS *pSoldier);

void CrowsFlyAway(UINT8 ubTeam);

void DebugValidateSoldierData();

void BeginTyingToFall(SOLDIERCLASS *pSoldier);

void SetSoldierAsUnderAiControl(SOLDIERCLASS *pSoldier);
void HandlePlayerTogglingLightEffects(BOOLEAN fToggleValue);

#endif
