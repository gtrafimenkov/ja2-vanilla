#include "SGP/SGPAll.h"
#include "SGP/LibraryDataBase.h"
#include <stdio.h>

INT8 gbLocale = ENGLISH_LANG;

STR8 LocaleNames[LANG_NUMBER] = {"default", "russian", "german", "dutch",
                                 "polish",  "french",  "italian"};

INT8 DetectLocale() {
  INT8 bLoc;
  CHAR8 zPath[MAX_PATH];
  CHAR8 zLocalePath[MAX_PATH];

  GetModuleFileName(NULL, zPath, MAX_PATH);
  strcpy(strrchr(zPath, '\\'), "\\Data\\");

  for (bLoc = RUSSIAN_LANG; bLoc < LANG_NUMBER; bLoc++) {
    strcpy(zLocalePath, zPath);
    strcat(zLocalePath, LocaleNames[bLoc]);
    strcat(zLocalePath, ".slf");

    if (GetFileAttributes(zLocalePath) != INVALID_FILE_ATTRIBUTES) {
      gbLocale = bLoc;
      sprintf(gGameLibaries[LIBRARY_NATIONAL_DATA].sLibraryName, "%s.slf", LocaleNames[gbLocale]);
      break;
    }
  }
  return gbLocale;
}

LibraryInitHeader gGameLibaries[] = {
    // Library Name					Can be	Init at start
    //													on
    // cd
    {"Data.slf", FALSE, TRUE},
    {"Ambient.slf", FALSE, TRUE},
    {"Anims.slf", FALSE, TRUE},
    {"BattleSnds.slf", FALSE, TRUE},
    {"BigItems.slf", FALSE, TRUE},
    {"BinaryData.slf", FALSE, TRUE},
    {"Cursors.slf", FALSE, TRUE},
    {"Faces.slf", FALSE, TRUE},
    {"Fonts.slf", FALSE, TRUE},
    {"Interface.slf", FALSE, TRUE},
    {"Laptop.slf", FALSE, TRUE},
    {"Maps.slf", TRUE, TRUE},
    {"MercEdt.slf", FALSE, TRUE},
    {"Music.slf", TRUE, TRUE},
    {"Npc_Speech.slf", TRUE, TRUE},
    {"NpcData.slf", FALSE, TRUE},
    {"RadarMaps.slf", FALSE, TRUE},
    {"Sounds.slf", FALSE, TRUE},
    {"Speech.slf", TRUE, TRUE},
    //	{ "TileCache.slf",				FALSE, TRUE },
    {"TileSets.slf", TRUE, TRUE},
    {"LoadScreens.slf", TRUE, TRUE},
    {"Intro.slf", TRUE, TRUE},

#ifdef JA2DEMO
    {"DemoAds.slf", FALSE, TRUE},
#endif

#ifdef GERMAN
    {"German.slf", FALSE, TRUE},
#endif

#ifdef POLISH
    {"Polish.slf", FALSE, TRUE},
#endif

#ifdef DUTCH
    {"Dutch.slf", FALSE, TRUE},
#endif

#ifdef ITALIAN
    {"Italian.slf", FALSE, TRUE},
#endif

#ifdef RUSSIAN
    {"Russian.slf", FALSE, TRUE},
#endif

};
