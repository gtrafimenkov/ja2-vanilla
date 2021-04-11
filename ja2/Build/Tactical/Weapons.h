#ifndef __WEAPONS_H
#define __WEAPONS_H

#include "Tactical/SoldierControl.h"

#define MAXCHANCETOHIT 99
#define BAD_DODGE_POSITION_PENALTY 20

#define GUN_BARREL_RANGE_BONUS 100

//***29.10.2007*** изменены параметры "кровавых" эффектов
// Special deaths can only occur within a limited distance to the target
#define MAX_DISTANCE_FOR_MESSY_DEATH 50  // 7
// If you do a lot of damage with a close-range shot, instant kill
#define MIN_DAMAGE_FOR_INSTANT_KILL 55
// If you happen to kill someone with a close-range shot doing a lot of damage to the head, head
// explosion
#define MIN_DAMAGE_FOR_HEAD_EXPLOSION 25  // 45
// If you happen to kill someone with a close-range shot doing a lot of damage to the chest, chest
// explosion This value is lower than head because of the damage bonus for shooting the head
#define MIN_DAMAGE_FOR_BLOWN_AWAY 25  // 30
// If you happen to hit someone in the legs for enough damage, REGARDLESS of distance, person falls
// down Leg damage is halved for these purposes
#define MIN_DAMAGE_FOR_AUTO_FALL_OVER 15  // 20

// short range at which being prone provides to hit penalty when shooting standing people
#define MIN_PRONE_RANGE 50

// can't miss at this range?
#define POINT_BLANK_RANGE 16

#define BODY_IMPACT_ABSORPTION 20

#define BUCKSHOT_SHOTS 9

#define MIN_MORTAR_RANGE 150  // minimum range of a mortar

// WEAPON CLASSES
enum {
  NOGUNCLASS,
  HANDGUNCLASS,
  SMGCLASS,
  RIFLECLASS,
  MGCLASS,
  SHOTGUNCLASS,
  KNIFECLASS,
  MONSTERCLASS,
  NUM_WEAPON_CLASSES
};

// exact gun types
enum {
  NOT_GUN = 0,
  GUN_PISTOL,
  GUN_M_PISTOL,
  GUN_SMG,
  GUN_RIFLE,
  GUN_SN_RIFLE,
  GUN_AS_RIFLE,
  GUN_LMG,
  GUN_SHOTGUN
};

// ARMOUR CLASSES
enum {
  ARMOURCLASS_HELMET,
  ARMOURCLASS_VEST,
  ARMOURCLASS_LEGGINGS,
  ARMOURCLASS_PLATE,
  ARMOURCLASS_MONST,
  ARMOURCLASS_VEHICLE
};

// Warning: There is a table in weapons.c that indexes using these enumberations...
// BurstSndStrings[]....
enum {
  NOAMMO = 0,
  AMMO38,
  AMMO9,
  AMMO45,
  AMMO357,
  AMMO12G,
  AMMOCAWS,
  AMMO545,
  AMMO556,
  AMMO762N,
  AMMO762W,
  AMMO47,
  AMMO57,
  AMMOMONST,
  AMMOROCKET,
  AMMODART,
  AMMOFLAME,
};

enum {
  AMMO_REGULAR = 0,
  AMMO_HP,
  AMMO_AP,
  AMMO_SUPER_AP,
  AMMO_BUCKSHOT,
  AMMO_FLECHETTE,
  AMMO_GRENADE,
  AMMO_MONSTER,
  AMMO_KNIFE,
  AMMO_HE,
  AMMO_HEAT,
  AMMO_SLEEP_DART,
  AMMO_FLAME,
};

enum {
  EXPLOSV_NORMAL,
  EXPLOSV_STUN,
  EXPLOSV_TEARGAS,
  EXPLOSV_MUSTGAS,
  EXPLOSV_FLARE,
  EXPLOSV_NOISE,
  EXPLOSV_SMOKE,
  EXPLOSV_CREATUREGAS,
};

#define AMMO_DAMAGE_ADJUSTMENT_BUCKSHOT(x) (x / 4)
#define NUM_BUCKSHOT_PELLETS 9

// hollow point bullets do lots of damage to people
#define AMMO_DAMAGE_ADJUSTMENT_HP(x) ((x * 17) / 10)
// but they SUCK at penetrating armour
#define AMMO_ARMOUR_ADJUSTMENT_HP(x) ((x * 3) / 2)
// armour piercing bullets are good at penetrating armour
#define AMMO_ARMOUR_ADJUSTMENT_AP(x) ((x * 11) / 16)  /// ((x * 3) / 4)
// "super" AP bullets are great at penetrating armour
#define AMMO_ARMOUR_ADJUSTMENT_SAP(x) (x / 2)
//***29.10.2007*** ультра супер бронебойные
#define AMMO_ARMOUR_ADJUSTMENT_USAP(x) ((x * 3) / 8)

// high explosive damage value (PRIOR to armour subtraction)
#define AMMO_DAMAGE_ADJUSTMENT_HE(x) ((x * 4) / 3)

// but they SUCK at penetrating armour
#define AMMO_STRUCTURE_ADJUSTMENT_HP(x) (x * 2)
// armour piercing bullets are good at penetrating structure
#define AMMO_STRUCTURE_ADJUSTMENT_AP(x) ((x * 11) / 16)  /// ((x * 3) / 4)
// "super" AP bullets are great at penetrating structures
#define AMMO_STRUCTURE_ADJUSTMENT_SAP(x) (x / 2)
//***29.10.2007*** ультра супер бронебойные
#define AMMO_STRUCTURE_ADJUSTMENT_USAP(x) ((x * 3) / 8)

// one quarter of punching damage is "real" rather than breath damage
#define PUNCH_REAL_DAMAGE_PORTION 4

#define AIM_BONUS_SAME_TARGET 10  // chance-to-hit bonus (in %)
#define AIM_BONUS_PER_AP 10       // chance-to-hit bonus (in %) for aim
#define AIM_BONUS_CROUCHING 10
#define AIM_BONUS_PRONE 20
#define AIM_BONUS_TWO_HANDED_PISTOL 5
#define AIM_BONUS_FIRING_DOWN 15
#define AIM_PENALTY_ONE_HANDED_PISTOL 5
#define AIM_PENALTY_DUAL_PISTOLS 20
#define AIM_PENALTY_SMG 5
#define AIM_PENALTY_GASSED 50
#define AIM_PENALTY_GETTINGAID 20
#define AIM_PENALTY_PER_SHOCK 5  // 5% penalty per point of shock
#define AIM_BONUS_TARGET_HATED 20
#define AIM_BONUS_PSYCHO 15
#define AIM_PENALTY_TARGET_BUDDY 20
#define AIM_PENALTY_BURSTING 10
#define AIM_PENALTY_GENTLEMAN 15
#define AIM_PENALTY_TARGET_CROUCHED 20
#define AIM_PENALTY_TARGET_PRONE 40
#define AIM_PENALTY_BLIND 80
#define AIM_PENALTY_FIRING_UP 25

typedef struct {
  UINT8 ubWeaponClass;     // handgun/shotgun/rifle/knife
  UINT8 ubWeaponType;      // exact type (for display purposes)
  UINT8 ubCalibre;         // type of ammunition needed
  UINT8 ubReadyTime;       // APs to ready/unready weapon
  UINT8 ubShotsPer4Turns;  // maximum (mechanical) firing rate
  UINT8 ubShotsPerBurst;
  UINT8 ubBurstPenalty;  // % penalty per shot after first
  UINT8 ubBulletSpeed;   // bullet's travelling speed
  UINT8 ubImpact;        // weapon's max damage impact (size & speed)
  UINT8 ubDeadliness;    // comparative ratings of guns
  INT8 bAccuracy;        // accuracy or penalty, не используется
  UINT8 ubMagSize;
  UINT16 usRange;
  UINT16 usReloadDelay;  //уже не используется, напрямую заменено в коде на число 200
  UINT8 ubAttackVolume;
  UINT8 ubHitVolume;
  UINT16 sSound;       //уже не используется
  UINT16 sBurstSound;  //уже не используется
  UINT16 sReloadSound;
  UINT16 sLocknLoadSound;

} WEAPONTYPE;

//***17.10.2007*** структура расширенных параметров оружия
#define FIRE_SND_STRING_LEN 25
typedef struct {
  INT8 zFireSndString[FIRE_SND_STRING_LEN];  // Часть имени файла звука выстрела
  UINT8 ubHeatCap;                           // Теплоёмкость
  UINT8 ubBurstAP;                           // Раздельные АР на очередь
  UINT8 ubBurstHitStart;  // Выстрел, после которого действует отдача
  UINT16 usIntAttach[4];  // Предустановленные аттачи
  //***13.04.2010***
  UINT8 ubRoomTurn;  // Дополнительные ОД на поворот с оружием в помещении

} WEAPONTYPE_EXT;

typedef struct {
  UINT8 ubCalibre;
  UINT8 ubMagSize;
  UINT8 ubAmmoType;
} MAGTYPE;

typedef struct {
  UINT8 ubArmourClass;
  UINT8 ubProtection;
  UINT8 ubDegradePercent;
} ARMOURTYPE;

typedef struct {
  UINT8 ubType;         // type of explosive
  UINT8 ubDamage;       // damage value
  UINT8 ubStunDamage;   // stun amount / 100
  UINT8 ubRadius;       // radius of effect
  UINT8 ubVolume;       // sound radius of explosion
  UINT8 ubVolatility;   // maximum chance of accidental explosion
  UINT8 ubAnimationID;  // Animation enum to use

} EXPLOSIVETYPE;

//***22.11.2010*** дополнительные свойства взрывчатки
typedef struct {
  UINT8 ubShrapnel;        // осколочная
  UINT8 ubShrapnelImpact;  // повреждения от осколка
  UINT8 ubShrapnelRadius;  // радиус разлёта осколков

} EXPLOSIVETYPE_EXT;

// GLOBALS
extern WEAPONTYPE_EXT WeaponExt[MAX_WEAPONS];
extern WEAPONTYPE Weapon[MAX_WEAPONS];
extern ARMOURTYPE Armour[];
extern MAGTYPE Magazine[];
extern EXPLOSIVETYPE Explosive[];
extern EXPLOSIVETYPE_EXT ExplosiveExt[];  //***22.11.2010***

extern INT8 EffectiveArmour(OBJECTTYPE *pObj);
extern INT8 ArmourVersusExplosivesPercent(SOLDIERCLASS *pSoldier);
extern BOOLEAN FireWeapon(SOLDIERCLASS *pSoldier, INT16 sTargetGridNo);
extern void WeaponHit(UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss,
                      UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange,
                      UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation);
extern void StructureHit(INT32 iBullet, UINT16 usWeaponIndex, INT8 bWeaponStatus,
                         UINT8 ubAttackerID, UINT16 sXPos, INT16 sYPos, INT16 sZPos,
                         UINT16 usStructureID, INT32 iImpact, BOOLEAN fStopped);
extern void WindowHit(INT16 sGridNo, UINT16 usStructureID, BOOLEAN fBlowWindowSouth,
                      BOOLEAN fLargeForce);
extern INT32 BulletImpact(SOLDIERCLASS *pFirer, SOLDIERCLASS *pTarget, UINT8 ubHitLocation,
                          INT32 iImpact, INT16 sHitBy, UINT8 *pubSpecial);
extern BOOLEAN InRange(SOLDIERCLASS *pSoldier, INT16 sGridNo);
extern void ShotMiss(UINT8 ubAttackerID, INT32 iBullet);
extern UINT32 CalcChanceToHitGun(SOLDIERCLASS *pSoldier, UINT16 sGridNo, UINT8 ubAimTime,
                                 UINT8 ubAimPos);
extern UINT8 AICalcChanceToHitGun(SOLDIERCLASS *pSoldier, UINT16 sGridNo, UINT8 ubAimTime,
                                  UINT8 ubAimPos);
extern UINT32 CalcChanceToPunch(SOLDIERCLASS *pAttacker, SOLDIERCLASS *pDefender, UINT8 ubAimTime);
extern UINT32 CalcChanceToStab(SOLDIERCLASS *pAttacker, SOLDIERCLASS *pDefender, UINT8 ubAimTime);
UINT32 CalcChanceToSteal(SOLDIERCLASS *pAttacker, SOLDIERCLASS *pDefender, UINT8 ubAimTime);
extern void ReloadWeapon(SOLDIERCLASS *pSoldier, UINT8 ubHandPos);
extern BOOLEAN IsGunBurstCapable(SOLDIERCLASS *pSoldier, UINT8 ubHandPos, BOOLEAN fNotify);
extern INT32 CalcBodyImpactReduction(UINT8 ubAmmoType, UINT8 ubHitLocation);
extern INT32 TotalArmourProtection(SOLDIERCLASS *pFirer, SOLDIERCLASS *pTarget, UINT8 ubHitLocation,
                                   INT32 iImpact, UINT8 ubAmmoType);
extern INT16 ArmourPercent(SOLDIERCLASS *pSoldier);

extern void GetTargetWorldPositions(SOLDIERCLASS *pSoldier, INT16 sTargetGridNo, FLOAT *pdXPos,
                                    FLOAT *pdYPos, FLOAT *pdZPos);

extern BOOLEAN OKFireWeapon(SOLDIERCLASS *pSoldier);
extern BOOLEAN CheckForGunJam(SOLDIERCLASS *pSoldier);

extern INT32 CalcMaxTossRange(SOLDIERCLASS *pSoldier, UINT16 usItem, BOOLEAN fArmed);
extern UINT32 CalcThrownChanceToHit(SOLDIERCLASS *pSoldier, INT16 sGridNo, UINT8 ubAimTime,
                                    UINT8 ubAimPos);

extern void ChangeWeaponMode(SOLDIERCLASS *pSoldier);

extern BOOLEAN UseHandToHand(SOLDIERCLASS *pSoldier, INT16 sTargetGridNo, BOOLEAN fStealing);

void DishoutQueenSwipeDamage(SOLDIERCLASS *pQueenSoldier);

INT32 HTHImpact(SOLDIERCLASS *pSoldier, SOLDIERCLASS *pTarget, INT32 iHitBy, BOOLEAN fBladeAttack);

UINT16 GunRange(OBJECTTYPE *pObj);

//***23.12.2008***
BOOLEAN FindSupport(SOLDIERCLASS *pSoldier);

#endif
