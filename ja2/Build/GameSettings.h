#ifndef _GAME_SETTINGS__H_
#define _GAME_SETTINGS__H_

#define NO_INI_FILE "..\\noptions.ini"
#define NO_INI_FILE_CF ".\\noptions.ini"

// If you add any options, MAKE sure you add the corresponding string to the Options Screen string
// array
enum {
  TOPTION_SPEECH,
  TOPTION_MUTE_CONFIRMATIONS,
  TOPTION_SUBTITLES,
  TOPTION_KEY_ADVANCE_SPEECH,
  TOPTION_ANIMATE_SMOKE,
  //	TOPTION_HIDE_BULLETS,
  //	TOPTION_CONFIRM_MOVE,
  TOPTION_BLOOD_N_GORE,
  TOPTION_DONT_MOVE_MOUSE,
  TOPTION_OLD_SELECTION_METHOD,
  TOPTION_ALWAYS_SHOW_MOVEMENT_PATH,

  //	TOPTION_TIME_LIMIT_TURNS,			//moved to the game init screen

  TOPTION_SHOW_MISSES,

  TOPTION_RTCONFIRM,

  //	TOPTION_DISPLAY_ENEMY_INDICATOR,		//Displays the number of enemies seen by the
  // merc, ontop of their portrait
  TOPTION_SLEEPWAKE_NOTIFICATION,

  TOPTION_USE_METRIC_SYSTEM,  // If set, uses the metric system

  TOPTION_MERC_ALWAYS_LIGHT_UP,

  TOPTION_SMART_CURSOR,

  TOPTION_SNAP_CURSOR_TO_DOOR,

  TOPTION_GLOW_ITEMS,
  TOPTION_TOGGLE_TREE_TOPS,
  TOPTION_TOGGLE_WIREFRAME,
  TOPTION_3D_CURSOR,

  NUM_GAME_OPTIONS,  // Toggle up this will be able to be Toggled by the player

  // These options will NOT be toggable by the Player
  TOPTION_MERC_CASTS_LIGHT = NUM_GAME_OPTIONS,
  TOPTION_HIDE_BULLETS,
  TOPTION_TRACKING_MODE,

  NUM_ALL_GAME_OPTIONS,
};

typedef struct {
  INT8 bLastSavedGameSlot;  // The last saved game number goes in here

  UINT8 ubMusicVolumeSetting;
  UINT8 ubSoundEffectsVolume;
  UINT8 ubSpeechVolume;

  // The following are set from the status of the toggle boxes in the Options Screen
  UINT8 fOptions[NUM_ALL_GAME_OPTIONS];

  CHAR8 zVersionNumber[14];

  UINT32 uiSettingsVersionNumber;
  UINT32 uiMeanwhileScenesSeenFlags;

  BOOLEAN fHideHelpInAllScreens;

  BOOLEAN fUNUSEDPlayerFinishedTheGame;  // JA2Gold: for UB compatibility
  UINT8 ubSizeOfDisplayCover;
  UINT8 ubSizeOfLOS;

  UINT8 ubFiller[17];

} GAME_SETTINGS;

// Enums for the difficulty levels
enum {
  DIF_LEVEL_ZERO,
  DIF_LEVEL_EASY,
  DIF_LEVEL_MEDIUM,
  DIF_LEVEL_HARD,
  DIF_LEVEL_FOUR,
};

typedef struct {
  BOOLEAN fGunNut;
  BOOLEAN fSciFi;
  UINT8 ubDifficultyLevel;
  BOOLEAN fTurnTimeLimit;
  BOOLEAN fIronManMode;

  UINT8 ubFiller[7];

} GAME_OPTIONS;

// This structure will contain general Ja2 settings  NOT individual game settings.
extern GAME_SETTINGS gGameSettings;

// This structure will contain the Game options set at the beginning of the game.
extern GAME_OPTIONS gGameOptions;

//***18.11.2007*** выбираемые дополнительные опции игры
typedef struct {
  BOOLEAN fEliteAIM;    // AIM в спецназе
  BOOLEAN fHostage;     // заложник Босса
  BOOLEAN fEpidemic;    // эпидемия
  BOOLEAN fOverheat;    // перегрев оружия
  BOOLEAN fJeepAttack;  // атакующие джипы
  BOOLEAN fActiveMilitia;  // активное или пассивное поведение гвардов
  BOOLEAN fAmbush;         // засады
  BOOLEAN fItemShadow;  // тень от картинок предметов в интерфейсе
  // BOOLEAN fAB102rus;		// для звуков в версии АВ 1.02
  BOOLEAN fLoyaltyMilitiaKilled;  // влияние гибели гвардов на лояльность в городах
  BOOLEAN fTeachingMilitia;  // тренировка гвардов "учителями"
  BOOLEAN fBlueMilitia;      // тренировка сразу регулярных гвардов
  BOOLEAN fAltScopeAP;  // альтернативный механизм расчёта затрат на прицеливание с оптикой
  BOOLEAN fDefecation;  // шутка. дефекация от страха у противника
  UINT8 fMaps;  // варианты набора карт: 0 - рандом, 1 - основные, 2 - альтернативные
  BOOLEAN fParatroopers;  // воздушный десант для городских секторов (1) не прикрытых ПВО и ПВО (2)
  INT8 bAddDistVisible;  //параметр для коррекции дальности зрения
  BOOLEAN fPayInventory;       // платный инвентарь
  BOOLEAN fProgressDropItems;  //выпадение предметов в соответствии с прогрессом их появления
  INT8 bFastMoveAI;            // ускоренная анимация передвижения AI
  BOOLEAN fSAITownCounterattack;  // контратака AI городских секторов из ближайших окрестностей
  BOOLEAN fSAITownSectorReinforcement;  // подкрепление AI для городского сектора из других секторов
                                        // города
  BOOLEAN fUseBatteries;  // работа от батареек НП, ПНВ и усилителя звуков
  INT8 bTrainCoefficient;       // коэффициент скорости тренировки
  BOOLEAN fPermanentTurnbased;  // постоянный пошаговый режим
  BOOLEAN fAdrenalin;           //воздействие адреналина
  BOOLEAN fContinueMercMovement;  //продолжение движения начатого до Перехвата
  BOOLEAN fSoftIronMan;  //запись в бою только в реалтайме
  BOOLEAN fUndyingMercs;  //наёмники не гибнущие от пуль и взрывчатки
  BOOLEAN fColorVest;         //цветная одежда
  BOOLEAN fShowCamo;          //показ камуфляжа
  BOOLEAN fCriticalHit;       //критические попадания
  BOOLEAN fUnlimitedMilitia;  //бесконечный пулл ополчения
  INT8 bGunPenaltyMax;  //величина прогрессивного штрафа при стрельбе
} EXTENDED_GAME_OPTIONS;

extern EXTENDED_GAME_OPTIONS gExtGameOptions;
void LoadExtGameOptions(void);

BOOLEAN SaveGameSettings();
BOOLEAN LoadGameSettings();

void InitGameOptions();

BOOLEAN GetCDLocation();

void DisplayGameSettings();

BOOLEAN MeanwhileSceneSeen(UINT8 ubMeanwhile);

BOOLEAN SetMeanwhileSceneSeen(UINT8 ubMeanwhile);

BOOLEAN CanGameBeSaved();

#endif
