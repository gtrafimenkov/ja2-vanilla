#ifndef _JA2_LIBS_H_
#define _JA2_LIBS_H_

enum {
  ENGLISH_LANG,
  RUSSIAN_LANG,
  GERMAN_LANG,
  DUTCH_LANG,
  POLISH_LANG,
  FRENCH_LANG,
  ITALIAN_LANG,

  LANG_NUMBER
};

extern INT8 gbLocale;

INT8 DetectLocale();

extern STR8 LocaleNames[LANG_NUMBER];

#define NUMBER_OF_LIBRARIES \
  ((gbLocale != ENGLISH_LANG) ? FULL_NUMBER_OF_LIBRARIES : (FULL_NUMBER_OF_LIBRARIES - 1))
// enums used for accessing the libraries
enum {
  LIBRARY_DATA,
  LIBRARY_AMBIENT,
  LIBRARY_ANIMS,
  LIBRARY_BATTLESNDS,
  LIBRARY_BIGITEMS,
  LIBRARY_BINARY_DATA,
  LIBRARY_CURSORS,
  LIBRARY_FACES,
  LIBRARY_FONTS,
  LIBRARY_INTERFACE,
  LIBRARY_LAPTOP,
  LIBRARY_MAPS,
  LIBRARY_MERCEDT,
  LIBRARY_MUSIC,
  LIBRARY_NPC_SPEECH,
  LIBRARY_NPC_DATA,
  LIBRARY_RADAR_MAPS,
  LIBRARY_SOUNDS,
  LIBRARY_SPEECH,
  //	LIBRARY_TILE_CACHE,
  LIBRARY_TILESETS,
  LIBRARY_LOADSCREENS,
  LIBRARY_INTRO,
//***21.01.2010***
#ifdef JA2EDITOR
  LIBRARY_EDITOR_DATA,
#endif  ///
  LIBRARY_NATIONAL_DATA,

  FULL_NUMBER_OF_LIBRARIES
};

/* 21.01.2010 старое
        //enums used for accessing the libraries
        enum
        {
                LIBRARY_DATA,
                LIBRARY_AMBIENT,
                LIBRARY_ANIMS,
                LIBRARY_BATTLESNDS,
                LIBRARY_BIGITEMS,
                LIBRARY_BINARY_DATA,
                LIBRARY_CURSORS,
                LIBRARY_FACES,
                LIBRARY_FONTS,
                LIBRARY_INTERFACE,
                LIBRARY_LAPTOP,
                LIBRARY_MAPS,
                LIBRARY_MERCEDT,
                LIBRARY_MUSIC,
                LIBRARY_NPC_SPEECH,
                LIBRARY_NPC_DATA,
                LIBRARY_RADAR_MAPS,
                LIBRARY_SOUNDS,
                LIBRARY_SPEECH,
//		LIBRARY_TILE_CACHE,
                LIBRARY_TILESETS,
                LIBRARY_LOADSCREENS,
                LIBRARY_INTRO,
#ifdef JA2DEMO
                LIBRARY_DEMO_ADS,
#endif

#ifdef GERMAN
                LIBRARY_GERMAN_DATA,
#endif

#ifdef DUTCH
                LIBRARY_DUTCH_DATA,
#endif

#ifdef POLISH
                LIBRARY_POLISH_DATA,
#endif

#ifdef ITALIAN
                LIBRARY_ITALIAN_DATA,
#endif

#ifdef RUSSIAN
                LIBRARY_RUSSIAN_DATA,
#endif
//***25.03.2008***
#ifdef JA2EDITOR
        LIBRARY_EDITOR_DATA,
#endif

                NUMBER_OF_LIBRARIES
        };
*/

#endif
